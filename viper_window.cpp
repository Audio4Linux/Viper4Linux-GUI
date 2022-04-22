#include "viper_window.h"
#include "ui_viper_window.h"

#include "misc/overlaymsgproxy.h"

#include "misc/versioncontainer.h"
#include "dialog/liquidequalizerwidget.h"
#include "misc/eventfilter.h"
#include "dialog/logdlg.h"
#include "misc/stylehelper.h"
#include "config/appconfigwrapper.h"
#include "misc/mathfunctions.h"
#include "misc/loghelper.h"
#include "misc/presetprovider.h"
#include "config/io.h"
#include "config/container.h"
#include "dialog/settingsdlg.h"
#include "dialog/presetdlg.h"
#include "dialog/convolverdlg.h"
#include "misc/converter.h"
#include "misc/common.h"
#include "config/container.h"
#include "config/io.h"

#ifndef VIPER_PLUGINMODE
#include "dbus/serveradaptor.h"
#include "dbus/clientproxy.h"
#include "dialog/firstlaunchwizard.h"
#include "dialog/statusfragment.h"
#include <phantomstyle.h>
#endif
#include "misc/GstRegistryHelper.h"


#include "3rdparty/WAF/Animation/Animation.h"

#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QWhatsThis>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QFrame>
#include <QGraphicsOpacityEffect>
#include <QDebug>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

#include <dialog/qmessageoverlay.h>


using namespace std;
#ifdef VIPER_PLUGINMODE
ViperWindow::ViperWindow(QString working_dir, QWidget *parent) :
    QWidget(parent),
    #else
ViperWindow::ViperWindow(QString exepath, bool statupInTray, bool allowMultipleInst, QWidget *parent) :
    QWidget(parent),
    #endif
    ui(new Ui::ViperWindow)
{
    Q_INIT_RESOURCE(v4l_frontend_resources);
    Q_INIT_RESOURCE(v4l_frontend_styles);

    ui->setupUi(this);
    bool aboutToQuit = false;

    LogHelper::clearLog();
    LogHelper::writeLog("UI launched...");

    connect(ui->popupButton, &QAbstractButton::clicked, this, &ViperWindow::popupButtonPressed);

#ifdef VIPER_PLUGINMODE
    /* Attach animation parent to the topmost widget */
    this->setProperty("animation_filter", true);
    this->layout()->setMargin(0);

    ui->popupButton->setIcon(QPixmap(":/resources/icon/checkout.svg"));
#else
    ui->popupButton->hide();


    m_exepath = exepath;
    m_startupInTraySwitch = statupInTray;

    tray_disableAction = new QAction();

    msg_launchfail = new OverlayMsgProxy(this);
    msg_versionmismatch = new OverlayMsgProxy(this);
#endif
    msg_notrunning = new OverlayMsgProxy(this);

    conf = new ConfigContainer();
    m_stylehelper = new StyleHelper(this);

#ifndef VIPER_PLUGINMODE
    m_dbus = new DBusProxy();
    m_appwrapper = new AppConfigWrapper(m_stylehelper);
#else
    m_appwrapper = new AppConfigWrapper(m_stylehelper, working_dir);
#endif
    m_appwrapper->loadAppConfig();

#ifndef VIPER_PLUGINMODE
    InitializeSpectrum();

    /* Load audio.conf */
    conf->setConfigMap(readConfig());
    LoadConfig();
#else
    /* Load defaults and wait for update from proxy */
    conf->setConfigMap(ConfigIO::readString(QString::fromStdString(default_config)));
    LoadConfig();
#endif

    conv_dlg = new ConvolverDlg(this,this);
    preset_dlg = new PresetDlg(this);

#ifndef VIPER_PLUGINMODE
    createTrayIcon();
    initGlobalTrayActions();
#endif

    settings_dlg = new SettingsDlg(this,this);
    log_dlg = new LogDlg(this);

#ifndef VIPER_PLUGINMODE
    //This section checks if another instance is already running and switches to it.
    new GuiAdaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool serviceRegistrationSuccessful = connection.registerObject("/Gui", this);
    bool objectRegistrationSuccessful = connection.registerService("cf.thebone.viper4linux.Gui");
    if(serviceRegistrationSuccessful && objectRegistrationSuccessful)
        LogHelper::writeLog("DBus service registration successful");
    else{
        LogHelper::writeLog("DBus service registration failed. Name already aquired by other instance");
        if(!allowMultipleInst){
            LogHelper::writeLog("Attempting to switch to this instance...");
            auto m_dbInterface = new cf::thebone::viper4linux::Gui("cf.thebone.viper4linux.Gui", "/Gui",
                                                                   QDBusConnection::sessionBus(), this);
            if(!m_dbInterface->isValid())
                LogHelper::writeLog("Critical: Unable to connect to other DBus instance. Continuing anyway...");
            else{
                QDBusPendingReply<> msg = m_dbInterface->raiseWindow();
                if(msg.isError() || !msg.isValid()){
                    LogHelper::writeLog("Critical: Other DBus instance returned invalid or error message. Continuing anyway...");
                }
                else{
                    aboutToQuit = true;
                    LogHelper::writeLog("Success! Waiting for event loop to exit...");
                    QTimer::singleShot(0, qApp, &QCoreApplication::quit);
                }
            }
        }
    }
#endif

    //Cancel constructor if quitting soon
    if(aboutToQuit) return;

    QMenu *menu = new QMenu();

#ifndef VIPER_PLUGINMODE
    spectrum = new QAction(tr("Reload spectrum"),this);
    connect(spectrum,&QAction::triggered,this,&ViperWindow::RestartSpectrum);

    menu->addAction(tr("Reload viper"), this,SLOT(Restart()));
    menu->addAction(spectrum);

    menu->addAction(tr("Driver status"), this,[this](){
        if(!GstRegistryHelper::HasDBusSupport()){
            OverlayMsgProxy* msg = new OverlayMsgProxy(this);
            msg->openError(tr("Unavailable"),
                           tr("Viper4Linux Legacy does not support this feature.\n"
                              "Please upgrade to a later version of Viper4Linux\n"
                              "to access this dialog (Audio4Linux version)."),
                           tr("Go back"));
            return;
        }

        if(!m_dbus->isValid()){
            ShowDBusError();
            return;
        }

        StatusDialog* sd = new StatusDialog(m_dbus);
        QWidget* host = new QWidget(this);
        host->setProperty("menu", false);
        QVBoxLayout* hostLayout = new QVBoxLayout(host);
        hostLayout->addWidget(sd);
        host->hide();
        host->setAutoFillBackground(true);
        connect(sd,&StatusDialog::closePressed,this,[host](){
            WAF::Animation::sideSlideOut(host, WAF::BottomSide);
        });
        WAF::Animation::sideSlideIn(host, WAF::BottomSide);
    });
#endif

    menu->addAction(tr("Load from file"), this,SLOT(LoadExternalFile()));
    menu->addAction(tr("Save to file"), this,SLOT(SaveExternalFile()));
#ifndef VIPER_PLUGINMODE
    menu->addAction(tr("View logs"), this,SLOT(OpenLog()));
#endif
    menu->addAction(tr("Context help"), this,[](){QWhatsThis::enterWhatsThisMode();});

    ui->toolButton->setMenu(menu);

    m_stylehelper->SetStyle();
    ui->eq_widget->setAccentColor(palette().highlight().color());

    ConnectActions();

#ifndef VIPER_PLUGINMODE
    if(m_appwrapper->getTrayMode() || m_startupInTraySwitch) trayIcon->show();
    else trayIcon->hide();

    connect(m_dbus, &DBusProxy::propertiesCommitted, this, [this](){
        if(m_appwrapper->getSyncDisabled())
            return;

        conf->setConfigMap(m_dbus->FetchPropertyMap());
        LoadConfig(Context::DBus);
    });
#endif
    connect(m_appwrapper,&AppConfigWrapper::styleChanged,this,[this](){
        /* Make sure the QToolButton looks the same as the QPushButton */
        ui->toolButton->setMaximumSize(ui->set->width(),
                                       ui->set->height());

        ui->frame->setStyleSheet(QString("QFrame#frame{background-color: %1;}").arg(this->palette().window().color().lighter().name()));
        ui->tabhost->setStyleSheet(QString("QWidget#tabhostPage1,QWidget#tabhostPage2,QWidget#tabhostPage3,QWidget#tabhostPage4,QWidget#tabhostPage5,QWidget#tabhostPage6,QWidget#tabhostPage7{background-color: %1;}").arg(this->palette().window().color().lighter().name()));
        ui->tabbar->redrawTabBar();
#ifndef VIPER_PLUGINMODE
        RestartSpectrum();
#endif
        ui->eq_widget->setAccentColor(palette().highlight().color());
    });

    ui->eq_widget->setAlwaysDrawHandles(m_appwrapper->getEqualizerPermanentHandles());
    connect(m_appwrapper,&AppConfigWrapper::eqChanged,this,[this](){
        ui->eq_widget->setAlwaysDrawHandles(m_appwrapper->getEqualizerPermanentHandles());
    });

    connect(m_appwrapper, &AppConfigWrapper::languageChanged, this, [this](){
        auto result = QMessageBox::information(this, "Warning", "Please restart this application to apply the updated language settings. Press 'Yes' to quit now.",
                                 QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::Cancel);
        if(result == QMessageBox::StandardButton::Yes)
        {
            this->close();
            exit(0);
        }
    });

#ifndef VIPER_PLUGINMODE
    ToggleSpectrum(m_appwrapper->getSpetrumEnable(),true);

    if(!m_appwrapper->getIntroShown())
        LaunchFirstRunSetup();
    else
        QTimer::singleShot(300,this,[this]{
            RunDiagnosticChecks();
        });
#endif

    ui->tabbar->setAnimatePageChange(true);
    ui->tabbar->setCustomStackWidget(ui->tabhost);
    ui->tabbar->setDetachCustomStackedWidget(true);
    ui->tabbar->addPage(tr("Bass/Clarity"));
    ui->tabbar->addPage(tr("Dynamic"));
    ui->tabbar->addPage(tr("Surround"));
    ui->tabbar->addPage(tr("Equalizer"));
    ui->tabbar->addPage(tr("Compressor"));
    ui->tabbar->addPage(tr("Volume"));
    ui->tabbar->addPage(tr("Miscellaneous"));
    ui->frame->setStyleSheet(QString("QFrame#frame{background-color: %1;}").arg(this->palette().window().color().lighter().name()));
    ui->tabhost->setStyleSheet(QString("QWidget#tabhostPage1,QWidget#tabhostPage2,QWidget#tabhostPage3,QWidget#tabhostPage4,QWidget#tabhostPage5,QWidget#tabhostPage6,QWidget#tabhostPage7{background-color: %1;}").arg(this->palette().window().color().lighter().name()));
    ui->tabbar->redrawTabBar();

    /* Make sure the QToolButton looks the same as the QPushButton */
    ui->toolButton->setMaximumSize(ui->set->width(),
                                   ui->set->height());

}

ViperWindow::~ViperWindow()
{
    delete ui;
}

void ViperWindow::loadConfigFromContainer(ConfigContainer c){
    conf->setConfigMap(c.getConfigMap());
    LoadConfig();
}

#ifdef VIPER_PLUGINMODE
QString ViperWindow::getLegacyPath(){
    return QString("%1/.config/viper4linux/").arg(QDir::homePath());
}
#endif

#ifndef VIPER_PLUGINMODE
void ViperWindow::LaunchFirstRunSetup(){
    FirstLaunchWizard* wiz = new FirstLaunchWizard(m_appwrapper,this);
    QHBoxLayout* lbLayout = new QHBoxLayout;
    QMessageOverlay* lightBox = new QMessageOverlay(this);
    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect();
    lightBox->setGraphicsEffect(eff);
    lightBox->setLayout(lbLayout);
    lightBox->layout()->addWidget(wiz);
    lightBox->show();
    QPropertyAnimation *a = new QPropertyAnimation(eff,"opacity");
    a->setDuration(500);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(wiz,&FirstLaunchWizard::wizardFinished,[=]{
        QPropertyAnimation *b = new QPropertyAnimation(eff,"opacity");
        b->setDuration(500);
        b->setStartValue(1);
        b->setEndValue(0);
        b->setEasingCurve(QEasingCurve::OutCirc);
        b->start(QPropertyAnimation::DeleteWhenStopped);
        connect(b,&QAbstractAnimation::finished, [=](){
            m_appwrapper->setIntroShown(true);
            lightBox->hide();
            settings_dlg->refreshAll();
            QTimer::singleShot(300,this,[this]{
                RunDiagnosticChecks();
            });
        });
    });
}
void ViperWindow::RunDiagnosticChecks(){
    if(system("which viper > /dev/null 2>&1") == 1){
        OverlayMsgProxy *msg = new OverlayMsgProxy(this);
        msg->openError(tr("Viper not installed"),
                       tr("Unable to find the viper executable.\n"
                          "Please make sure viper is installed and you\n"
                          "are using the lastest version of gst-plugin-viperfx"),
                       tr("Continue anyway"));
    }
    else if(!GstRegistryHelper::IsPluginInstalled()){
        OverlayMsgProxy *msg = new OverlayMsgProxy(this);
        msg->openError(tr("Missing component"),
                       tr("Unable to find the GStreamer viperfx plugin.\n"
                          "Make sure you installed Viper4Linux correctly\n"
                          "and double-check the README instructions.\n"
                          "You can also run 'gst-inspect-1.0 viperfx'\n"
                          "to check if the plugin was correctly installed."),
                       tr("Try again"));

        connect(msg,&OverlayMsgProxy::buttonPressed,[this](){
            QTimer::singleShot(300,this,[this](){
                gst_update_registry();
                RunDiagnosticChecks();
            });
        });

    }
    else if(GstRegistryHelper::HasDBusSupport() && !m_dbus->isValid()){
        qWarning() << "[W] Viper DBus IPC is inactive. Attempting to launch service using viper.sh";
        Restart();
    }
}
#endif
//Spectrum
#ifndef VIPER_PLUGINMODE
void ViperWindow::SetSpectrumVisibility(bool v){
    m_spectrograph->setVisible(v);
    if(v)
        this->findChild<QFrame*>("analysisLayout_spectrum")->setFrameShape(QFrame::StyledPanel);
    else
        this->findChild<QFrame*>("analysisLayout_spectrum")->setFrameShape(QFrame::NoFrame);
}
void ViperWindow::InitializeSpectrum(){
    m_spectrograph = new Spectrograph(this);
    m_audioengine = new AudioStreamEngine(this);

    int refresh = m_appwrapper->getSpectrumRefresh();
    if(refresh == 0) refresh = 20;
    if(refresh < 10) refresh = 10;
    else if(refresh > 500) refresh = 500;
    m_audioengine->setNotifyIntervalMs(refresh);

    analysisLayout.reset(new QFrame());
    analysisLayout->setObjectName("analysisLayout_spectrum");
    analysisLayout->setFrameShape(QFrame::Shape::StyledPanel);
    analysisLayout->setLayout(new QHBoxLayout);
    analysisLayout->layout()->setMargin(0);
    analysisLayout->layout()->addWidget(m_spectrograph);


    auto buttonbox = ui->host_layout->takeAt(ui->host_layout->count() - 1);
    layout()->addWidget(analysisLayout.data());
    layout()->addItem(buttonbox);
    analysisLayout.take();

    SetSpectrumVisibility(false);

    connect(m_appwrapper,&AppConfigWrapper::spectrumChanged,this,[this]{
        ToggleSpectrum(m_appwrapper->getSpetrumEnable(),true);
    });
    connect(m_appwrapper,&AppConfigWrapper::spectrumReloadRequired,this,&ViperWindow::RestartSpectrum);
}

void ViperWindow::RestartSpectrum(){
    ToggleSpectrum(false,false);
    ToggleSpectrum(m_appwrapper->getSpetrumEnable(),false);
}
void ViperWindow::RefreshSpectrumParameters(){
    int bands = m_appwrapper->getSpectrumBands();
    int minfreq = m_appwrapper->getSpectrumMinFreq();
    int maxfreq = m_appwrapper->getSpectrumMaxFreq();
    int refresh = m_appwrapper->getSpectrumRefresh();
    float multiplier = m_appwrapper->getSpectrumMultiplier();
    //Set default values if undefined
    if(bands == 0) bands = 100;
    if(maxfreq == 0) maxfreq = 1000;
    if(refresh == 0) refresh = 10;
    if(multiplier == 0) multiplier = 0.15;

    //Check boundaries
    if(bands < 5 ) bands = 5;
    else if(bands > 300) bands = 300;
    if(minfreq < 0) minfreq = 0;
    else if(minfreq > 10000) minfreq = 10000;
    if(maxfreq < 100) maxfreq = 100;
    else if(maxfreq > 24000) maxfreq = 24000;
    if(refresh < 10) refresh = 10;
    else if(refresh > 500) refresh = 500;
    if(multiplier < 0.01) multiplier = 0.01;
    else if(multiplier > 1) multiplier = 1;

    if(maxfreq < minfreq) maxfreq = minfreq + 100;

    QColor outline;
    if (palette().window().style() == Qt::TexturePattern)
        outline = QColor(0, 0, 0, 160);
    else
        outline = palette().window().color().lighter(140);

    if(m_appwrapper->getSpectrumTheme() == 0)
        m_spectrograph->setTheme(Qt::black,
                                 QColor(51,204,201),
                                 QColor(51,204,201).darker(),
                                 QColor(255,255,0),
                                 m_appwrapper->getSpetrumGrid(),
                                 (Spectrograph::Mode)m_appwrapper->getSpectrumShape());
    else
        m_spectrograph->setTheme(palette().window().color().lighter(),
                                 palette().highlight().color(),
                                 palette().text().color(),
                                 outline.lighter(108),
                                 m_appwrapper->getSpetrumGrid(),
                                 (Spectrograph::Mode)m_appwrapper->getSpectrumShape());

    m_spectrograph->setParams(bands, minfreq, maxfreq);
    m_audioengine->setNotifyIntervalMs(refresh);
    m_audioengine->setMultiplier(multiplier);
}
void ViperWindow::ToggleSpectrum(bool on,bool ctrl_visibility){

    RefreshSpectrumParameters();
    if(ctrl_visibility)spectrum->setVisible(on);
    if(on && (!m_spectrograph->isVisible() || !ctrl_visibility)){
        if(ctrl_visibility){
            SetSpectrumVisibility(true);
            this->setGeometry(this->x(), this->y(), this->width(),this->height()+m_spectrograph->size().height());
            //this->setFixedSize(this->width(),this->height()+m_spectrograph->size().height());
        }

        QAudioDeviceInfo in;
        for(auto item : QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
            if(item.deviceName()==m_appwrapper->getSpectrumInput())
                in = item;

        LogHelper::writeLog("Spectrum Expected Input Device: "+m_appwrapper->getSpectrumInput());
        LogHelper::writeLog("Spectrum Found Input Device: "+in.deviceName());
        LogHelper::writeLog("Spectrum Default Input Device: "+QAudioDeviceInfo::defaultInputDevice().deviceName());

        m_audioengine->setAudioInputDevice(in);
        m_audioengine->initializeRecord();
        m_audioengine->startRecording();

        connect(m_audioengine, static_cast<void (AudioStreamEngine::*)(QAudio::Mode,QAudio::State)>(&AudioStreamEngine::stateChanged),
                this, [this](QAudio::Mode mode,QAudio::State state){
            Q_UNUSED(mode);

            if (QAudio::ActiveState != state && QAudio::SuspendedState != state) {
                m_spectrograph->reset();
            }
        });

        connect(m_audioengine, static_cast<void (AudioStreamEngine::*)(qint64, qint64, const FrequencySpectrum &)>(&AudioStreamEngine::spectrumChanged),
                this, [this](qint64, qint64,const FrequencySpectrum &spectrum){
            m_spectrograph->spectrumChanged(spectrum);
        });
    }
    else if(!on && (m_spectrograph->isVisible() || !ctrl_visibility)){
        if(ctrl_visibility){
            SetSpectrumVisibility(false);
            this->setGeometry(this->x(), this->y(), this->width(),this->height()-m_spectrograph->size().height());

            //this->setFixedSize(this->width(),this->height()-m_spectrograph->size().height());
        }
        m_spectrograph->reset();
        m_audioengine->reset();
    }
}
#endif
//Overrides
void ViperWindow::setVisible(bool visible)
{
#ifndef VIPER_PLUGINMODE
    //Reconnect to dbus to make sure the connection isn't stale
    m_dbus = new DBusProxy();


    updateTrayPresetList();
    updateTrayConvolverList();
#endif

    //Hide all other windows if set to invisible
    if(!visible){
        log_dlg->hide();
        preset_dlg->hide();
    }

#ifndef VIPER_PLUGINMODE
    if(GstRegistryHelper::HasDBusSupport() && m_dbus->isValid() &&
            msg_notrunning != nullptr)
        msg_notrunning->hide();
#endif

    QWidget::setVisible(visible);
}

void ViperWindow::setPopupButtonEnabled(bool b)
{
    ui->popupButton->setVisible(b);
}
void ViperWindow::closeEvent(QCloseEvent *event)
{
#ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible()) {
        return;
    }
#endif
#ifndef VIPER_PLUGINMODE
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
#endif
}

//DBus
void ViperWindow::ShowDBusError(){
#ifndef VIPER_PLUGINMODE
    if(msg_notrunning != nullptr)
        msg_notrunning->hide();
    msg_notrunning = new OverlayMsgProxy(this);
    msg_notrunning->openError(tr("Viper inactive"),
                              tr("Unable to connect to the DBus interface.\n"
                                 "Please make sure viper is active and you are\n"
                                 "using the lastest version of gst-plugin-viperfx"),
                              tr("Launch viper"));
    connect(msg_notrunning,&OverlayMsgProxy::buttonPressed,[this](){
        int returncode = system("viper start");
        if(returncode != 0){
            if(msg_launchfail != nullptr)
                msg_launchfail->hide();
            msg_launchfail = new OverlayMsgProxy(this);
            msg_launchfail->openError(tr("Failed to launch viper"),
                                      tr("viper.sh has returned a non-null exit code.\n"
                                         "Please make sure viper is correctly installed\n"
                                         "and try to restart it manually"),
                                      tr("Continue anyway"));
        } else {
            //Reconnect DBus
            QTimer::singleShot(500,this,[this](){
                m_dbus = new DBusProxy();
                RestartSpectrum();
            });
        }
    });
#else
    if(msg_notrunning != nullptr)
        msg_notrunning->hide();
    msg_notrunning = new OverlayMsgProxy(this);
    msg_notrunning->openError(tr("Service not found"),
                              tr("Unable to connect to the DBus interface.\n"
                                 "Please make sure viper is active and you are\n"
                                 "using the lastest version of gst-plugin-viperfx"),
                              tr("Continue"));
#endif
}

//Systray
#ifndef VIPER_PLUGINMODE
void ViperWindow::raiseWindow(){    
    /*
     * NOTE: Raising the window does not always work!
     *
     * KDE users can disable 'Focus Stealing Prevention'
     * in the Window Behavior section (system settings)
     * as a workaround.
     */
    show();
    setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    raise();
    activateWindow();
}
void ViperWindow::setTrayVisible(bool visible){  
    if(visible) trayIcon->show();
    else trayIcon->hide();
}
void ViperWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        setVisible(!this->isVisible());
        if(isVisible()){
            this->showNormal();
            this->setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            this->raise();
            this->activateWindow();
        }
        //Hide tray icon if disabled and MainWin is visible (for cmdline force switch)
        if(!m_appwrapper->getTrayMode() && this->isVisible()) trayIcon->hide();
        break;
    default:
        ;
    }
}
void ViperWindow::updateTrayPresetList(){
    if(tray_presetMenu != nullptr){
        tray_presetMenu->clear();
        QString absolute = QFileInfo(m_appwrapper->getPath()).absoluteDir().absolutePath();
        QString path = pathAppend(absolute,"presets");

        QDir dir(path);
        if (!dir.exists())
            dir.mkpath(".");

        QStringList nameFilter("*.conf");
        QStringList files = dir.entryList(nameFilter);
        if(files.count()<1){
            QAction *noPresets = new QAction("No presets found");
            noPresets->setEnabled(false);
            tray_presetMenu->addAction(noPresets);
        }
        else{
            for(int i = 0; i < files.count(); i++){
                //Strip extensions
                QFileInfo fi(files[i]);
                files[i] = fi.completeBaseName();
                //Add entry
                QAction *newEntry = new QAction(files[i]);
                connect(newEntry,&QAction::triggered,this,[=](){
                    LoadPresetFile(pathAppend(path,QString("%1.conf").arg(files[i])));
                });
                tray_presetMenu->addAction(newEntry);
            }
        }
    }
}
void ViperWindow::updateTrayConvolverList(){
    if(tray_convMenu != nullptr){
        tray_convMenu->clear();
        QString absolute = QFileInfo(m_appwrapper->getPath()).absoluteDir().absolutePath();
        QString path = pathAppend(absolute,"irs_favorites");

        QDir dir(path);
        if (!dir.exists())
            dir.mkpath(".");

        QStringList nameFilter({"*.wav","*.irs"});
        QStringList files = dir.entryList(nameFilter);
        if(files.count()<1){
            QAction *noPresets = new QAction("No impulse responses found");
            noPresets->setEnabled(false);
            tray_convMenu->addAction(noPresets);
        }
        else{
            for(int i = 0; i < files.count(); i++){
                //Add entry
                QAction *newEntry = new QAction(files[i]);
                connect(newEntry,&QAction::triggered,this,[=](){
                    SetIRS(files[i],true);
                });
                tray_convMenu->addAction(newEntry);
            }
        }
    }
}

void ViperWindow::createTrayIcon()
{    
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setToolTip("Viper4Linux");
    connect(trayIcon, &QSystemTrayIcon::activated, this, &ViperWindow::iconActivated);
    trayIcon->setIcon(QIcon(":/viper.png"));
}

void ViperWindow::updateTrayMenu(QMenu* menu){

    trayIcon->hide();
    createTrayIcon();
    trayIcon->show();
    trayIconMenu = menu;
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon->contextMenu(),&QMenu::aboutToShow,[this]{
        updateTrayPresetList();
        updateTrayConvolverList();
    });
    m_appwrapper->setTrayContextMenu(MenuIO::buildString(menu));

}
QMenu* ViperWindow::getTrayContextMenu(){
    return trayIconMenu;
}
void ViperWindow::initGlobalTrayActions(){
    tray_disableAction = new QAction(tr("&Disable FX"), this);
    tray_disableAction->setProperty("tag","disablefx");
    tray_disableAction->setCheckable(true);
    tray_disableAction->setChecked(!conf->getBool("fx_enable"));
    connect(tray_disableAction, &QAction::triggered, this, [this](){
        conf->setValue("fx_enable",!tray_disableAction->isChecked());
        ui->disableFX->setChecked(tray_disableAction->isChecked());
        ApplyConfig();
    });
    tray_presetMenu = new QMenu(tr("&Presets"));
    tray_presetMenu->setProperty("tag","menu_preset");
    tray_convMenu = new QMenu(tr("&Convolver Bookmarks"));
    tray_convMenu->setProperty("tag","menu_convolver");
    auto init = MenuIO::buildMenu(buildAvailableActions(),m_appwrapper->getTrayContextMenu());
    if(init->actions().count() < 1)
        init = buildDefaultActions();
    updateTrayMenu(init);
}
QMenu* ViperWindow::buildAvailableActions()
{
    QAction *reloadAction = new QAction(tr("&Reload Viper"), this);
    reloadAction->setProperty("tag","reload");
    connect(reloadAction, &QAction::triggered, this, &ViperWindow::Restart);

    QAction* quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    quitAction->setProperty("tag","quit");

    QMenu* dynsysMenu = new QMenu(tr("Dynamic &System Presets"), this);
    for(auto preset : PresetProvider::Dynsys::DYNSYS_LOOKUP_TABLE().keys()){
        QAction *newEntry = new QAction(preset);
        connect(newEntry,&QAction::triggered,this,[=](){
            ui->dynsys_preset->setCurrentText(preset);
            DynsysPresetSelectionUpdated();
        });
        dynsysMenu->addAction(newEntry);
    }
    dynsysMenu->setProperty("tag","menu_dynsys_preset");

    QMenu* eqMenu = new QMenu(tr("&EQ Presets"), this);
    for(auto preset : PresetProvider::EQ::EQ_LOOKUP_TABLE().keys()){
        QAction *newEntry = new QAction(preset);
        connect(newEntry,&QAction::triggered,this,[=](){
            if(preset == tr("Default"))
                ResetEQ();
            else{
                ui->eqpreset->setCurrentText(preset);
                EqPresetSelectionUpdated();
            }
        });
        eqMenu->addAction(newEntry);
    }
    eqMenu->setProperty("tag","menu_eq_preset");

    QMenu* colmMenu = new QMenu(tr("C&olorful Music Presets"), this);
    for(auto preset : PresetProvider::Colm::COLM_LOOKUP_TABLE().keys()){
        if(preset == tr("Unknown")) continue;
        QAction *newEntry = new QAction(preset);
        connect(newEntry,&QAction::triggered,this,[=](){
            ui->colmpreset->setCurrentText(preset);
            ColmPresetSelectionUpdated();
        });
        colmMenu->addAction(newEntry);
    }
    colmMenu->setProperty("tag","menu_colm_preset");

    QMenu* bassModeMenu = new QMenu(tr("ViPER &Bass Mode"), this);
    QStringList bassModes({tr("Natural Bass"),tr("Pure Bass+"),tr("Subwoofer")});
    for(auto mode : bassModes){
        QAction *newEntry = new QAction(mode);
        connect(newEntry,&QAction::triggered,this,[=](){
            ui->vbmode->setCurrentIndex(bassModes.indexOf(mode));
            OnUpdate(true);
        });
        bassModeMenu->addAction(newEntry);
    }
    bassModeMenu->setProperty("tag","menu_bass_mode");

    QMenu* clarityModeMenu = new QMenu(tr("ViPER C&larity Mode"), this);
    QStringList clarityModes({tr("Natural"),tr("OZone+"),tr("XHiFi")});
    for(auto mode : clarityModes){
        QAction *newEntry = new QAction(mode);
        connect(newEntry,&QAction::triggered,this,[=](){
            ui->vcmode->setCurrentIndex(clarityModes.indexOf(mode));
            OnUpdate(true);
        });
        clarityModeMenu->addAction(newEntry);
    }
    clarityModeMenu->setProperty("tag","menu_clarity_mode");

    QMenu* menu = new QMenu(this);
    menu->addAction(tray_disableAction);
    menu->addAction(reloadAction);
    menu->addMenu(tray_presetMenu);
    menu->addSeparator();
    menu->addMenu(dynsysMenu);
    menu->addMenu(eqMenu);
    menu->addMenu(colmMenu);
    menu->addMenu(tray_convMenu);
    menu->addSeparator();
    menu->addMenu(bassModeMenu);
    menu->addMenu(clarityModeMenu);
    menu->addSeparator();
    menu->addAction(quitAction);
    return menu;
}
QMenu* ViperWindow::buildDefaultActions()
{
    QAction *reloadAction = new QAction(tr("&Reload Viper"), this);
    reloadAction->setProperty("tag","reload");
    connect(reloadAction, &QAction::triggered, this, &ViperWindow::Restart);

    QAction* quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
    quitAction->setProperty("tag","quit");

    QMenu* menu = new QMenu(this);
    menu->addAction(tray_disableAction);
    menu->addAction(reloadAction);
    menu->addMenu(tray_presetMenu);
    menu->addSeparator();
    menu->addAction(quitAction);
    return menu;
}
#endif

//---Dialogs/Buttons
void ViperWindow::DialogHandler(){
    if(sender() == ui->conv_select){
        QWidget* host = new QWidget(this);
        host->setProperty("menu", false);
        QVBoxLayout* hostLayout = new QVBoxLayout(host);
        hostLayout->addWidget(conv_dlg);
        host->hide();
        host->setAutoFillBackground(true);
        connect(conv_dlg,&ConvolverDlg::closePressed,this,[host](){
            WAF::Animation::sideSlideOut(host, WAF::BottomSide);
        });

        WAF::Animation::sideSlideIn(host, WAF::BottomSide);    }
    else if(sender() == ui->set){
        QFrame* host = new QFrame(this);
        host->setProperty("menu", false);
        QVBoxLayout* hostLayout = new QVBoxLayout(host);
        hostLayout->addWidget(settings_dlg);
        host->hide();
        host->setAutoFillBackground(true);
        settings_dlg->updateInputSinks();

        connect(settings_dlg,&SettingsDlg::closeClicked,this,[host](){
            host->update();
            host->repaint();
            WAF::Animation::sideSlideOut(host, WAF::BottomSide);
        });

        WAF::Animation::sideSlideIn(host, WAF::BottomSide);
    }
    else if(sender() == ui->cpreset){
        if(preset_dlg->isVisible()){
            //Hacky workaround to reliably raise the window on all distros
            Qt::WindowFlags eFlags = preset_dlg->windowFlags();
            eFlags |= Qt::WindowStaysOnTopHint;
            preset_dlg->setWindowFlags(eFlags);
            preset_dlg->show();
            eFlags &= ~Qt::WindowStaysOnTopHint;
            preset_dlg->setWindowFlags(eFlags);
            preset_dlg->showNormal();
            preset_dlg->setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            preset_dlg->raise();
            preset_dlg->activateWindow();
            return;
        }

        preset_dlg->move(x() + (width() - preset_dlg->width()) / 2,
                         y() + (height() - preset_dlg->height()) / 2);

        preset_dlg->setModal(true);
        preset_dlg->show();
    }
}
void ViperWindow::OpenLog(){
    log_dlg->show();
    log_dlg->updateLog();
}
void ViperWindow::Reset(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,tr("Reset Configuration"),tr("Are you sure?"),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        conf->setConfigMap(ConfigIO::readString(QString::fromStdString(default_config)));
        LoadConfig();

        ApplyConfig();
    }
}
void ViperWindow::DisableFX(){
#ifndef VIPER_PLUGINMODE
    tray_disableAction->setChecked(ui->disableFX->isChecked());
#endif
    //Apply instantly
    if(!lockapply)ApplyConfig();
}

//---Reloader
void ViperWindow::OnUpdate(bool ignoremode){
    //Will be called when slider has been moved, dynsys/eq preset set or spinbox changed
    if((m_appwrapper->getAutoFx() &&
        (ignoremode||m_appwrapper->getAutoFxMode()==0)) && !lockapply)
        ApplyConfig();
}
void ViperWindow::OnRelease(){
    if((m_appwrapper->getAutoFx() &&
        m_appwrapper->getAutoFxMode()==1) && !lockapply)
        ApplyConfig();
}
#ifndef VIPER_PLUGINMODE
void ViperWindow::Restart(){
    if(m_appwrapper->getGFix())system("killall -r glava");
    system("viper restart");
    if(m_appwrapper->getGFix())system("setsid glava -d >/dev/null 2>&1 &");
    RestartSpectrum();
}
#endif

//---User preset management
void ViperWindow::LoadPresetFile(const QString& filename){
#ifndef VIPER_PLUGINMODE
    const QString& src = filename;
    const QString dest = m_appwrapper->getPath();
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);

    conf->setConfigMap(readConfig());
#else
    conf->setConfigMap(readConfig(filename));
#endif
    LogHelper::writeLog("Loading from " + filename+ " (main/loadpreset)");
    LoadConfig();

    ApplyConfig();
}
void ViperWindow::SavePresetFile(const QString& filename){
#ifndef VIPER_PLUGINMODE
    const QString src = m_appwrapper->getPath();
    const QString& dest = filename;
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
#else
    ConfigIO::writeFile(filename, conf->getConfigMap());
#endif
    LogHelper::writeLog("Saving to " + filename+ " (main/savepreset)");
}
void ViperWindow::LoadExternalFile(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Load custom audio.conf"),"","*.conf");
    if(filename=="")return;
#ifndef VIPER_PLUGINMODE
    const QString& src = filename;
    const QString dest = m_appwrapper->getPath();
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    conf->setConfigMap(readConfig());
#else
    conf->setConfigMap(readConfig(filename));
#endif

    LogHelper::writeLog("Loading from " + filename+ " (main/loadexternal)");

    LoadConfig();

    ApplyConfig();
}
void ViperWindow::SaveExternalFile(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Save current audio.conf"),"","*.conf");
    if(filename=="")return;
    QFileInfo fi(filename);
    QString ext = fi.suffix();
    if(ext!="conf")filename.append(".conf");

#ifndef VIPER_PLUGINMODE
    const QString src = m_appwrapper->getPath();
    const QString dest = filename;
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
#else
    ConfigIO::writeFile(filename, conf->getConfigMap());
#endif
    LogHelper::writeLog("Saving to " + filename+ " (main/saveexternal)");
}

//---Config IO
void ViperWindow::LoadConfig(Context ctx){
    lockapply=true;
#ifndef VIPER_PLUGINMODE
    tray_disableAction->setChecked(!conf->getBool("fx_enable"));
#endif
    ui->disableFX->setChecked(!conf->getBool("fx_enable"));
    ui->tubesim->setChecked(conf->getBool("tube_enable"));
    ui->colm->setChecked(conf->getBool("colm_enable"));
    ui->colmwide->setValueA(conf->getInt("colm_widening"));
    ui->colmdepth->setValueA(conf->getInt("colm_depth"));
    ui->colmmidimg->setValueA(conf->getInt("colm_midimage"));
    ui->colmwide->setValueA(conf->getInt("colm_widening"));
    ui->clarity->setChecked(conf->getBool("vc_enable"));
    ui->vcmode->setCurrentIndex(conf->getInt("vc_mode"));
    ui->vclvl->setValueA(conf->getInt("vc_level"));
    ui->vb->setChecked(conf->getBool("vb_enable"));
    ui->vbmode->setCurrentIndex(conf->getInt("vb_mode"));
    ui->vbgain->setValueA(conf->getInt("vb_gain"));
    ui->vbfreq->setValueA(conf->getInt("vb_freq"));
    ui->vhp->setChecked(conf->getBool("vhe_enable"));
    ui->vhplvl->setValueA(conf->getInt("vhe_level"));
    ui->diff->setChecked(conf->getBool("ds_enable"));
    ui->difflvl->setValueA(conf->getInt("ds_level"));
    ui->reverb->setChecked(conf->getBool("reverb_enable"));
    ui->roomsize->setValueA(conf->getInt("reverb_roomsize"));
    ui->roomwidth->setValueA(conf->getInt("reverb_width"));
    ui->roomdamp->setValueA(conf->getInt("reverb_damp"));
    ui->wet->setValueA(conf->getInt("reverb_wet"));
    ui->dry->setValueA(conf->getInt("reverb_dry"));
    ui->agc->setChecked(conf->getBool("agc_enable"));
    ui->gain->setValueA(conf->getInt("agc_ratio"));
    ui->maxgain->setValueA(conf->getInt("agc_maxgain"));
    ui->maxvol->setValueA(conf->getInt("agc_volume"));
    ui->limiter->setValueA(conf->getInt("lim_threshold"));
    ui->outputpan->setValueA(conf->getInt("out_pan"));
    ui->outvolume->setValueA(conf->getInt("out_volume"));
    ui->enable_eq->setChecked(conf->getBool("eq_enable"));
    ui->enable_comp->setChecked(conf->getBool("fetcomp_enable"));
    ui->m_gain->setChecked(conf->getBool("fetcomp_autogain"));
    ui->m_width->setChecked(conf->getBool("fetcomp_autoknee"));
    ui->m_attack->setChecked(conf->getBool("fetcomp_autoattack"));
    ui->m_release->setChecked(conf->getBool("fetcomp_autorelease"));
    ui->noclip->setChecked(conf->getBool("fetcomp_noclip"));
    ui->comp_thres->setValueA(conf->getInt("fetcomp_threshold"));
    ui->compgain->setValueA(conf->getInt("fetcomp_gain"));
    ui->compwidth->setValueA(conf->getInt("fetcomp_kneewidth"));
    ui->comp_ratio->setValueA(conf->getInt("fetcomp_ratio"));
    ui->compattack->setValueA(conf->getInt("fetcomp_attack"));
    ui->comprelease->setValueA(conf->getInt("fetcomp_release"));
    ui->a_adapt->setValueA(conf->getInt("fetcomp_meta_adapt"));
    ui->a_crest->setValueA(conf->getInt("fetcomp_meta_crest"));
    ui->a_maxatk->setValueA(conf->getInt("fetcomp_meta_maxattack"));
    ui->a_maxrel->setValueA(conf->getInt("fetcomp_meta_maxrelease"));
    ui->a_kneewidth->setValueA(conf->getInt("fetcomp_meta_kneemulti"));
    ui->vcure->setChecked(conf->getBool("cure_enable"));
    ui->vcurelvl->setValueA(conf->getInt("cure_level"));
    ui->ax->setChecked(conf->getBool("ax_enable"));
    ui->axmode->setValueA(conf->getInt("ax_mode"));
    ui->vse->setChecked(conf->getBool("vse_enable"));
    ui->barkfreq->setValueA(conf->getInt("vse_ref_bark"));
    ui->barkcon->setValueA(conf->getInt("vse_bark_cons"));
    ui->conv->setChecked(conf->getBool("conv_enable"));
    ui->convcc->setValueA(conf->getInt("conv_cc_level"));
    ui->dynsys->setChecked(conf->getBool("dynsys_enable"));
    ui->dyn_xcoeff1->setValueA(conf->getInt("dynsys_xcoeff1"));
    ui->dyn_xcoeff2->setValueA(conf->getInt("dynsys_xcoeff2"));
    ui->dyn_ycoeff1->setValueA(conf->getInt("dynsys_ycoeff1"));
    ui->dyn_ycoeff2->setValueA(conf->getInt("dynsys_ycoeff2"));
    ui->dyn_sidegain1->setValueA(conf->getInt("dynsys_sidegain1"));
    ui->dyn_sidegain2->setValueA(conf->getInt("dynsys_sidegain2"));
    ui->dyn_bassgain->setValueA(conf->getInt("dynsys_bassgain"));

    QString ir = conf->getString("conv_ir_path",false);

    int eq1 = conf->getInt("eq_band1");
    int eq2 = conf->getInt("eq_band2");
    int eq3 = conf->getInt("eq_band3");
    int eq4 = conf->getInt("eq_band4");
    int eq5 = conf->getInt("eq_band5");
    int eq6 = conf->getInt("eq_band6");
    int eq7 = conf->getInt("eq_band7");
    int eq8 = conf->getInt("eq_band8");
    int eq9 = conf->getInt("eq_band9");
    int eq10 = conf->getInt("eq_band10");
    auto eq_data = QVector<float>(std::initializer_list<float>({eq1/100.0f,eq2/100.0f,
                                                                eq3/100.0f,eq4/100.0f,
                                                                eq5/100.0f,eq6/100.0f,
                                                                eq7/100.0f,eq8/100.0f,
                                                                eq9/100.0f,eq10/100.0f}));
    int it = 0;
    bool eqReloadRequired = false;
    for(auto cur_data : ui->eq_widget->getBands()){
        bool equal = isApproximatelyEqual<float>(cur_data,eq_data.at(it));
        if(eqReloadRequired == false)
            eqReloadRequired = !equal;
        it++;
    }
    if(eqReloadRequired)
        ui->eq_widget->setBands(eq_data,false);

    if(ctx == Context::Application){
        UpdateEqStringFromWidget();
        UpdateDynsysStringFromWidget();
        UpdateColmStringFromWidget();
    }

#ifdef VIPER_PLUGINMODE
    QString configdir = this->getLegacyPath();
#else
    QString configdir = QFileInfo(m_appwrapper->getPath()).absoluteDir().absolutePath();
#endif
    ir = ir.replace("$configpath\"\"", configdir);
    if(ir.size() > 2){
        if(ir.at(0)=='"')ir.remove(0,1); //remove double quotes
        if(ir.at(ir.length()-1)=='"')ir.chop(1);
        activeirs = ir;
        QFileInfo irsInfo(ir);
        ui->convpath->setText(irsInfo.baseName());
        ui->convpath->setCursorPosition(0);
    }

    UpdateAllUnitLabels();

    lockapply=false;
}
void ViperWindow::ApplyConfig(bool restart){  
    conf->setValue("fx_enable",QVariant(!ui->disableFX->isChecked()));
    conf->setValue("tube_enable",QVariant(ui->tubesim->isChecked()));
    conf->setValue("colm_enable",QVariant(ui->colm->isChecked()));
    conf->setValue("colm_widening",QVariant(ui->colmwide->valueA()));
    conf->setValue("colm_depth",QVariant(ui->colmdepth->valueA()));
    conf->setValue("colm_midimage",QVariant(ui->colmmidimg->valueA()));
    conf->setValue("vc_enable",QVariant(ui->clarity->isChecked()));
    conf->setValue("vc_mode",QVariant(ui->vcmode->currentIndex()));
    conf->setValue("vc_level",QVariant(ui->vclvl->valueA()));
    conf->setValue("cure_enable",QVariant(ui->vcure->isChecked()));
    conf->setValue("ax_enable",QVariant(ui->ax->isChecked()));
    conf->setValue("cure_level",QVariant(ui->vcurelvl->valueA()));
    conf->setValue("ax_mode",QVariant(ui->axmode->valueA()));
    conf->setValue("vse_enable",QVariant(ui->vse->isChecked()));
    conf->setValue("vse_ref_bark",QVariant(ui->barkfreq->valueA()));
    conf->setValue("vse_bark_cons",QVariant(ui->barkcon->valueA()));
    conf->setValue("conv_enable",QVariant(ui->conv->isChecked()));
    conf->setValue("conv_cc_level",QVariant(ui->convcc->valueA()));
    conf->setValue("conv_ir_path",QVariant("\""+ activeirs + "\""));
    conf->setValue("dynsys_enable",QVariant(ui->dynsys->isChecked()));
    conf->setValue("dynsys_bassgain",QVariant(ui->dyn_bassgain->valueA()));
    conf->setValue("dynsys_sidegain1",QVariant(ui->dyn_sidegain1->valueA()));
    conf->setValue("dynsys_sidegain2",QVariant(ui->dyn_sidegain2->valueA()));
    conf->setValue("dynsys_xcoeff1",QVariant(ui->dyn_xcoeff1->valueA()));
    conf->setValue("dynsys_xcoeff2",QVariant(ui->dyn_xcoeff2->valueA()));
    conf->setValue("dynsys_ycoeff1",QVariant(ui->dyn_ycoeff1->valueA()));
    conf->setValue("dynsys_ycoeff2",QVariant(ui->dyn_ycoeff2->valueA()));
    conf->setValue("agc_enable",QVariant(ui->agc->isChecked()));
    conf->setValue("agc_ratio",QVariant(ui->gain->valueA()));
    conf->setValue("agc_maxgain",QVariant(ui->maxgain->valueA()));
    conf->setValue("agc_volume",QVariant(ui->maxvol->valueA()));
    conf->setValue("lim_threshold",QVariant(ui->limiter->valueA()));
    conf->setValue("out_pan",QVariant(ui->outputpan->valueA()));
    conf->setValue("out_volume",QVariant(ui->outvolume->valueA()));
    conf->setValue("vhe_enable",QVariant(ui->vhp->isChecked()));
    conf->setValue("vhe_level",QVariant(ui->vhplvl->valueA()));
    conf->setValue("ds_enable",QVariant(ui->diff->isChecked()));
    conf->setValue("ds_level",QVariant(ui->difflvl->valueA()));
    conf->setValue("reverb_enable",QVariant(ui->reverb->isChecked()));
    conf->setValue("reverb_roomsize",QVariant(ui->roomsize->valueA()));
    conf->setValue("reverb_width",QVariant(ui->roomwidth->valueA()));
    conf->setValue("reverb_damp",QVariant(ui->roomdamp->valueA()));
    conf->setValue("reverb_wet",QVariant(ui->wet->valueA()));
    conf->setValue("reverb_dry",QVariant(ui->dry->valueA()));
    conf->setValue("vb_enable",QVariant(ui->vb->isChecked()));
    conf->setValue("vb_freq",QVariant(ui->vbfreq->valueA()));
    conf->setValue("vb_gain",QVariant(ui->vbgain->valueA()));
    conf->setValue("vb_mode",QVariant(ui->vbmode->currentIndex()));
    conf->setValue("fetcomp_enable",QVariant(ui->enable_comp->isChecked()));
    conf->setValue("fetcomp_autoattack",QVariant(ui->m_attack->isChecked()));
    conf->setValue("fetcomp_autogain",QVariant(ui->m_gain->isChecked()));
    conf->setValue("fetcomp_autoknee",QVariant(ui->m_width->isChecked()));
    conf->setValue("fetcomp_autorelease",QVariant(ui->m_release->isChecked()));
    conf->setValue("fetcomp_noclip",QVariant(ui->noclip->isChecked()));
    conf->setValue("fetcomp_threshold",QVariant(ui->comp_thres->valueA()));
    conf->setValue("fetcomp_gain",QVariant(ui->compgain->valueA()));
    conf->setValue("fetcomp_kneewidth",QVariant(ui->compwidth->valueA()));
    conf->setValue("fetcomp_ratio",QVariant(ui->comp_ratio->valueA()));
    conf->setValue("fetcomp_attack",QVariant(ui->compattack->valueA()));
    conf->setValue("fetcomp_release",QVariant(ui->comprelease->valueA()));
    conf->setValue("fetcomp_meta_adapt",QVariant(ui->a_adapt->valueA()));
    conf->setValue("fetcomp_meta_crest",QVariant(ui->a_crest->valueA()));
    conf->setValue("fetcomp_meta_maxattack",QVariant(ui->a_maxatk->valueA()));
    conf->setValue("fetcomp_meta_maxrelease",QVariant(ui->a_maxrel->valueA()));
    conf->setValue("fetcomp_meta_kneemulti",QVariant(ui->a_kneewidth->valueA()));
    conf->setValue("eq_enable",QVariant(ui->enable_eq->isChecked()));
    conf->setValue("eq_band1",QVariant(int(ui->eq_widget->getBand(0)*100)));
    conf->setValue("eq_band2",QVariant(int(ui->eq_widget->getBand(1)*100)));
    conf->setValue("eq_band3",QVariant(int(ui->eq_widget->getBand(2)*100)));
    conf->setValue("eq_band4",QVariant(int(ui->eq_widget->getBand(3)*100)));
    conf->setValue("eq_band5",QVariant(int(ui->eq_widget->getBand(4)*100)));
    conf->setValue("eq_band6",QVariant(int(ui->eq_widget->getBand(5)*100)));
    conf->setValue("eq_band7",QVariant(int(ui->eq_widget->getBand(6)*100)));
    conf->setValue("eq_band8",QVariant(int(ui->eq_widget->getBand(7)*100)));
    conf->setValue("eq_band9",QVariant(int(ui->eq_widget->getBand(8)*100)));
    conf->setValue("eq_band10",QVariant(int(ui->eq_widget->getBand(9)*100)));

    emit changesApplied();

#ifndef VIPER_PLUGINMODE
    ConfigIO::writeFile(m_appwrapper->getPath(),conf->getConfigMap());


    if(GstRegistryHelper::HasDBusSupport()){
        ConfigContainer dbus_template = *conf;
        dbus_template.setValue("conv_ir_path", activeirs);
        m_dbus->SubmitPropertyMap(dbus_template.getConfigMap());

        if(restart){
            if(m_appwrapper->getReloadMethod() == ReloadMethod::DIRECT_DBUS){
                if(!m_dbus->isValid()){
                    qWarning() << "Viper DBus IPC is inactive. Attempting to launch service using viper.sh";
                    Restart();
                }
                else
                    m_dbus->CommitProperties(DBusProxy::PARAM_GROUP_ALL);
            }
            else
                Restart();
        }
    }
    else if(restart){
        //Legacy mode
        Restart();
    }
#else
    Q_UNUSED(restart);
#endif
}

//---Predefined Presets
void ViperWindow::EqPresetSelectionUpdated(){
    if(ui->eqpreset->currentText() == tr("Custom"))
        return;
    auto preset = PresetProvider::EQ::lookupPreset(ui->eqpreset->currentText());
    if(preset.size() > 0)SetEQ(preset);
    else ResetEQ();
}
void ViperWindow::DynsysPresetSelectionUpdated(){
    if(ui->dynsys_preset->currentText() == tr("Custom"))
        return;
    const auto data = PresetProvider::Dynsys::lookupPreset(ui->dynsys_preset->currentText());
    if(data.size() <= 1)
        return;
    lockapply=true;
    ui->dyn_xcoeff1->setValueA(data.begin()[0]);
    ui->dyn_xcoeff2->setValueA(data.begin()[1]);
    ui->dyn_ycoeff1->setValueA(data.begin()[2]);
    ui->dyn_ycoeff2->setValueA(data.begin()[3]);
    ui->dyn_sidegain1->setValueA(data.begin()[4]);
    ui->dyn_sidegain2->setValueA(data.begin()[5]);
    lockapply=false;

    OnUpdate(true);
}
void ViperWindow::ColmPresetSelectionUpdated(){
    if(ui->colmpreset->currentText() == "...")
        return;
    const auto data = PresetProvider::Colm::lookupPreset(ui->colmpreset->currentText());
    if(data.size() <= 1)
        return;
    lockapply=true;
    ui->colmwide->setValueA(data.begin()[0]);
    ui->colmdepth->setValueA(data.begin()[1]);
    lockapply=false;

    OnUpdate(true);
}

//--Status
void ViperWindow::UpdateUnitLabel(int d,QObject *alt){
    if(lockapply&&alt==nullptr)return;//Skip if lockapply-flag is set (when setting presets, ...)
    QObject* obj;

    if(alt==nullptr)obj = sender();
    else obj = alt;

    QString pre = "";
    QString post = "";
    if(obj==ui->vbmode){
        //Bass
        if(d==0) UpdateTooltipLabelUnit(obj,tr("Natural"),alt==nullptr);
        else if(d==1) UpdateTooltipLabelUnit(obj,tr("Pure Bass+"),alt==nullptr);
        else if(d==2) UpdateTooltipLabelUnit(obj,tr("Subwoofer"),alt==nullptr);
        else UpdateTooltipLabelUnit(obj,tr("Mode %1").arg(QString::number( d )),alt==nullptr);
    }
    else if(obj==ui->vcmode){
        //Clarity
        if(d==0) UpdateTooltipLabelUnit(obj,tr("Natural"),alt==nullptr);
        else if(d==1) UpdateTooltipLabelUnit(obj,tr("OZone+"),alt==nullptr);
        else if(d==2) UpdateTooltipLabelUnit(obj,tr("XHiFi"),alt==nullptr);
        else UpdateTooltipLabelUnit(obj,tr("Mode %1").arg(QString::number( d )),alt==nullptr);
    }
    else if(obj==ui->gain){
        //AGC
        if(d <= 50) UpdateTooltipLabelUnit(obj,tr("Very Slight (%1)").arg(QString::number(d)),alt==nullptr);
        else if(d <= 100) UpdateTooltipLabelUnit(obj,tr("Slight (%1)").arg(QString::number(d)),alt==nullptr);
        else if(d <= 300) UpdateTooltipLabelUnit(obj,tr("Moderate (%1)").arg(QString::number(d)),alt==nullptr);
        else UpdateTooltipLabelUnit(obj,tr("Extreme (%1)").arg(QString::number( d )),alt==nullptr);
    }
    else if(obj==ui->axmode || obj==ui->vcurelvl){
        //AnalogX //Cure+
        if(d==0) UpdateTooltipLabelUnit(obj,tr("Slight"),alt==nullptr);
        else if(d==1) UpdateTooltipLabelUnit(obj,tr("Moderate"),alt==nullptr);
        else if(d==2) UpdateTooltipLabelUnit(obj,tr("Extreme"),alt==nullptr);
        else UpdateTooltipLabelUnit(obj,tr("Mode %1").arg(QString::number( d )),alt==nullptr);
    }
    //Dynsys
    else if(obj==ui->dyn_bassgain){
        float scaled = (d-100.0f)/20.0f;
        float result = (scaled/100.0f + -1.0f) * 80.0f;

        UpdateTooltipLabelUnit(obj,
                               QString::number(result, 'f', 2) + "dB "
                                                                 "(" + QString::number( (d-100)/20 ) + "%)",
                               alt==nullptr);
    }
    //Diff-Surround
    else if(obj==ui->difflvl)UpdateTooltipLabelUnit(obj,QString::number(translate(d,0,100,0,20))+"ms (" + QString::number(d) + "%)",alt==nullptr);
    //AGC
    else if(obj==ui->maxgain)UpdateTooltipLabelUnit(obj,QString::number((int)translate(d,100,800,1,8))+"x (" + QString::number(d) + ")",alt==nullptr);
    else if(obj==ui->maxvol)UpdateTooltipLabelUnit(obj,QString::number(roundf(translate(d,100,0,0,-30)*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    //Bass
    else if(obj==ui->vbgain){
        long double x = d;
        long double in =1.472577725 * pow(10L,-18L) * pow(x,7L) - 3.011526005 * pow(10L,-15L) * pow(x,6L) + 2.29923043 * pow(10L,-12L) * pow(x,5L) - 9.530124502 * pow(10L,-10L) * pow(x,4L) + 3.960377639 * pow(10L,-7L) * pow(x,3L) - 1.965034894 * pow(10L,-4L) * pow(x,2L) + 7.693150538 * pow(10L,-2L) * x + 1.965508847 * pow(10L,-2L);
        UpdateTooltipLabelUnit(obj,QString::number(roundf(in*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    }
    //Clarity
    else if(obj==ui->vclvl)UpdateTooltipLabelUnit(obj,QString::number(roundf(translate(d,0,450,0,14.8)*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    //Volume
    else if(obj==ui->outvolume){
        long double x = d;
        //Note:
        //V4A uses hardcoded arrays to display the output dB values when updating the slider in the app;
        //Although this makes it easier to implement, it is not really responsive.
        //Unlike the other sliders, this one increases exponentially rather than linearly, because of that I had to recreate a formula from the values in the v4a app, to calculate an accurate dB value (using polynomial regression; this might not be the most efficient solution).
        long double in = -7.095691001L* pow(10L,-13L) * pow(x,8L) + 3.130488467L* pow(10L,-10L) * pow(x,7L) - 5.667388779* pow(10L,-8L) * pow(x,6L) + 5.394863197L* pow(10L,-6L) * pow(x,5L) - 2.864305503L* pow(10L,-4L) * pow(x,4L)+ 8.264191247L* pow(10L,-3L) * pow(x,3L) - 1.218006784L* pow(10L,-1L) * pow(x,2L)+ 1.529341362L * x - 40.00317088L;
        UpdateTooltipLabelUnit(obj,QString::number(roundf(in*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    }
    else if(obj==ui->limiter)UpdateTooltipLabelUnit(obj,QString::number(roundf(translate(d,100,0,0,-30)*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    //Headphone Engine
    else if(obj==ui->vhplvl)UpdateTooltipLabelUnit(obj,tr("Level %1").arg(QString::number(d+1)),alt==nullptr);
    //Reverb
    else if(obj==ui->roomsize){
        long double x = d;
        UpdateTooltipLabelUnit(obj,QString::number(roundf(3.958333333* pow(10L,-7L) *pow(x,5L)- 4.106206294* pow(10L,-5L) *pow(x,4L)+ 1.189175408* pow(10L,-3L) *pow(x,3L)+ 4.16448133 * pow(10L,-3L) *pow(x,2L)+ 9.190238943 * pow(10L,-1L) * x+ 25.11013978))+"m\u00B2 (" + QString::number(d) + ")",alt==nullptr);
    }
    else if(obj==ui->roomwidth){
        long double x = d;
        long double in = -1.121794872 * pow(10L,-8L) * pow(x,5L) + 3.270687646 * pow(10L,-6L) * pow(x,4L) - 2.643502331 * pow(10L,-4L) * pow(x,3L) + 7.749854312 * pow(10L,-3L) * pow(x,2L) + 2.916958039 * pow(10L,-2L) * x+ 5.036713287;
        UpdateTooltipLabelUnit(obj,QString::number(roundf(in))+"m (" + QString::number(d) + ")",alt==nullptr);
    }
    //Compressor
    else if(obj==ui->comp_thres)  UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(0,d),alt==nullptr);
    else if(obj==ui->comp_ratio)  UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(1,d),alt==nullptr);
    else if(obj==ui->compwidth)   UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(2,d),alt==nullptr);
    else if(obj==ui->compgain)    UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(3,d),alt==nullptr);
    else if(obj==ui->compattack)  UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(4,d),alt==nullptr);
    else if(obj==ui->comprelease) UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(5,d),alt==nullptr);
    else if(obj==ui->a_kneewidth) UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(6,d),alt==nullptr);
    else if(obj==ui->a_maxatk)    UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(7,d),alt==nullptr);
    else if(obj==ui->a_maxrel)    UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(8,d),alt==nullptr);
    else if(obj==ui->a_crest)     UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(9,d),alt==nullptr);
    else if(obj==ui->a_adapt)     UpdateTooltipLabelUnit(obj,MathFunctions::buildCompressorUnitString(10,d),alt==nullptr);
    else{
        //Reverb
        if(obj==ui->roomdamp)post = "%";
        else if(obj==ui->wet)post = "%";
        else if(obj==ui->dry)post = "%";
        //Bass
        else if(obj==ui->vbfreq)post = "Hz";
        //Spectrum Expend
        else if(obj==ui->barkcon)pre = tr("Level ");
        else if(obj==ui->barkfreq)post = "Hz";
        //Convolver
        else if(obj==ui->convcc)post = "%";
        UpdateTooltipLabelUnit(obj,pre + QString::number(d) + post,alt==nullptr);
    }
    if(!lockapply||obj!=nullptr)OnUpdate(false);
}
void ViperWindow::UpdateTooltipLabelUnit(QObject* sender,const QString& text,bool viasignal){
    QWidget *w = qobject_cast<QWidget*>(sender);
    w->setToolTip(text);
    if(viasignal)ui->info->setText(text);
}
void ViperWindow::UpdateAllUnitLabels(){
    QList<QAnimatedSlider*> slidersToBeUpdated(
    {ui->colmwide,ui->colmdepth,ui->colmmidimg,ui->vclvl,ui->vbfreq,
     ui->vbgain,ui->vhplvl,ui->difflvl,ui->roomsize,ui->roomwidth,ui->roomdamp,ui->wet,ui->dry,
     ui->gain,ui->maxgain,ui->maxvol,ui->limiter,ui->outputpan,ui->outvolume,ui->comp_thres,
     ui->compgain,ui->compwidth,ui->comp_ratio,ui->compattack,ui->comprelease,ui->a_adapt,ui->a_crest,
     ui->a_maxatk,ui->a_maxrel,ui->a_kneewidth,ui->vcurelvl,ui->axmode,ui->barkcon,ui->barkfreq,ui->convcc,
     ui->dyn_xcoeff1,ui->dyn_xcoeff2,ui->dyn_ycoeff1,ui->dyn_ycoeff2,ui->dyn_sidegain1,ui->dyn_sidegain2,ui->dyn_bassgain});

    QList<QComboBox*> comboboxesToBeUpdated({ui->vcmode,ui->vbmode});

    foreach (auto w, slidersToBeUpdated)
        UpdateUnitLabel(w->valueA(),w);

    foreach (auto w, comboboxesToBeUpdated)
        UpdateUnitLabel(w->currentIndex(),w);
}

//---Helper
void ViperWindow::SetEQ(const QVector<float>& data){
    lockapply=true;
    ui->eq_widget->setBands(QVector<float>(data));
    lockapply=false;
    OnUpdate(true);
}
void ViperWindow::ResetEQ(){
    ui->reset_eq->setEnabled(false);
    QTimer::singleShot(510,this,[this](){
        ui->reset_eq->setEnabled(true);
    });
    ui->eqpreset->setCurrentIndex(0);
    SetEQ(PresetProvider::EQ::defaultPreset());
    ApplyConfig();
}
void ViperWindow::SetIRS(const QString& irs,bool apply){
    activeirs = irs;
    QFileInfo irsInfo(irs);
    ui->convpath->setText(irsInfo.baseName());
    ui->convpath->setCursorPosition(0);
    if(apply)
        ApplyConfig();
}
void ViperWindow::UpdateEqStringFromWidget(){
    QString currentEqPresetName =
            PresetProvider::EQ::reverseLookup(ui->eq_widget->getBands());
    ui->eqpreset->blockSignals(true);
    ui->eqpreset->setCurrentText(currentEqPresetName);
    ui->eqpreset->blockSignals(false);
}
void ViperWindow::UpdateDynsysStringFromWidget(){
    QString currentDynsysPresetName =
            PresetProvider::Dynsys::reverseLookup({ui->dyn_xcoeff1->valueA(),ui->dyn_xcoeff2->valueA(),
                                                   ui->dyn_ycoeff1->valueA(),ui->dyn_ycoeff2->valueA(),
                                                   ui->dyn_sidegain1->valueA(),ui->dyn_sidegain2->valueA()});
    ui->dynsys_preset->blockSignals(true);
    ui->dynsys_preset->setCurrentText(currentDynsysPresetName);
    ui->dynsys_preset->blockSignals(false);
}
void ViperWindow::UpdateColmStringFromWidget(){
    QString currentColmPresetName =
            PresetProvider::Colm::reverseLookup({ui->colmwide->valueA(),ui->colmdepth->valueA()});
    ui->colmpreset->blockSignals(true);
    ui->colmpreset->setCurrentText(currentColmPresetName);
    ui->colmpreset->blockSignals(false);
}

QVariantMap ViperWindow::readConfig(const QString& path){

#ifndef VIPER_PLUGINMODE
    QVariantMap confmap = ConfigIO::readFile(path == "" ? m_appwrapper->getPath() : path);

    if(confmap.count() < 1){
        //No audio.conf found
        std::filebuf fb;
        fb.open (m_appwrapper->getPath().toUtf8().constData(),std::ios::out);
        std::ostream os(&fb);
        os << default_config;
        fb.close();
        confmap = ConfigIO::readFile(m_appwrapper->getPath());
    }
#else
    QVariantMap confmap = ConfigIO::readFile(path);
#endif
    return confmap;
}

#ifndef VIPER_PLUGINMODE
QString ViperWindow::GetExecutablePath(){
    return m_exepath;
}
#endif

AppConfigWrapper* ViperWindow::getACWrapper(){
    return m_appwrapper;
}

//---Connect UI-Signals
void ViperWindow::ConnectActions(){
    QList<QComboBox*> registerCurrentIndexChange({ui->vbmode,ui->vcmode});

    QList<QAnimatedSlider*> registerValueAChange(
    {ui->convcc,ui->vbfreq,ui->vbgain,ui->difflvl,ui->vhplvl,ui->roomsize,ui->roomwidth,ui->roomdamp,
     ui->wet,ui->dry,ui->colmwide,ui->colmmidimg,ui->colmdepth,ui->vclvl,ui->gain,ui->maxgain,ui->maxvol,
     ui->outputpan,ui->limiter,ui->outvolume,ui->vcurelvl,ui->axmode,ui->barkfreq,ui->barkcon,ui->comprelease,ui->compgain,
     ui->compwidth,ui->comp_ratio,ui->comp_thres,ui->compattack,ui->comprelease,ui->a_adapt,ui->a_crest,ui->a_maxatk,ui->a_maxrel,
     ui->a_kneewidth,ui->dyn_xcoeff1,ui->dyn_xcoeff2,ui->dyn_ycoeff1,ui->dyn_ycoeff2,ui->dyn_bassgain,ui->dyn_sidegain1,ui->dyn_sidegain2});

    QList<QWidget*> registerSliderRelease(
    {ui->convcc,ui->vbfreq,ui->vbgain,ui->difflvl,ui->vhplvl,ui->roomsize,ui->roomwidth,ui->roomdamp,ui->wet,ui->dry,
     ui->colmwide,ui->colmmidimg,ui->colmdepth,ui->vclvl,ui->gain,ui->maxgain,ui->maxvol,ui->outputpan,ui->limiter,ui->outvolume,
     ui->vcurelvl,ui->axmode,ui->barkfreq,ui->barkcon,ui->comprelease,ui->compgain,ui->compwidth,ui->comp_ratio,ui->comp_thres,
     ui->compattack,ui->comprelease,ui->a_adapt,ui->a_crest,ui->a_maxatk,ui->a_maxrel,ui->a_kneewidth,ui->dyn_xcoeff1,ui->dyn_xcoeff2,
     ui->dyn_ycoeff1,ui->dyn_ycoeff2,ui->dyn_bassgain,ui->dyn_sidegain1,ui->dyn_sidegain2});

    QList<QWidget*> registerClick(
    {ui->vb,ui->clarity,ui->vcure,ui->tubesim,ui->vhp,ui->diff,ui->reverb,ui->enable_eq,ui->enable_comp,ui->noclip,ui->m_gain,
     ui->m_width,ui->m_attack,ui->m_release,ui->vb,ui->clarity,ui->vcure,ui->tubesim,ui->agc,ui->colm,ui->vse,ui->conv,ui->ax,ui->dynsys});

    QList<QWidget*> registerDynsysUpdate(
    {ui->dyn_xcoeff1,ui->dyn_xcoeff2,ui->dyn_ycoeff1,ui->dyn_ycoeff2,ui->dyn_sidegain1,ui->dyn_sidegain2});

    QList<QWidget*> registerColmUpdate(
    {ui->colmwide,ui->colmdepth});

    foreach (QWidget* w, registerCurrentIndexChange)
        connect(w,              SIGNAL(currentIndexChanged(int)),          this,   SLOT(UpdateUnitLabel(int)));

    foreach (QWidget* w, registerValueAChange)
        connect(w,              SIGNAL(valueChangedA(int)),          this,   SLOT(UpdateUnitLabel(int)));

    foreach (QWidget* w, registerSliderRelease)
        connect(w,              SIGNAL(sliderReleased()),           this,   SLOT(OnRelease()));

    foreach (QWidget* w, registerClick)
        connect(w,              SIGNAL(clicked()),                  this,   SLOT(OnUpdate()));

    foreach (QWidget* w, registerDynsysUpdate)
        connect(w,              SIGNAL(sliderReleased()),           this,   SLOT(UpdateDynsysStringFromWidget()));

    foreach (QWidget* w, registerColmUpdate)
        connect(w,              SIGNAL(sliderReleased()),           this,   SLOT(UpdateColmStringFromWidget()));

    connect(ui->apply,          SIGNAL(clicked()),                  this,   SLOT(ApplyConfig()));
    connect(ui->disableFX,      SIGNAL(clicked()),                  this,   SLOT(DisableFX()));
    connect(ui->reset_eq,       SIGNAL(clicked()),                  this,   SLOT(ResetEQ()));
    connect(ui->reset,          SIGNAL(clicked()),                  this,   SLOT(Reset()));
    connect(ui->conv_select,    SIGNAL(clicked()),                  this,   SLOT(DialogHandler()));
    connect(ui->cpreset,        SIGNAL(clicked()),                  this,   SLOT(DialogHandler()));
    connect(ui->set,            SIGNAL(clicked()),                  this,   SLOT(DialogHandler()));
    connect(ui->eq_widget,      SIGNAL(bandsUpdated()),             this,   SLOT(ApplyConfig()));
    connect(ui->eq_widget,      SIGNAL(mouseReleased()),            this,   SLOT(UpdateEqStringFromWidget()));
    connect(ui->eqpreset,       SIGNAL(currentIndexChanged(int)),   this,   SLOT(EqPresetSelectionUpdated()));
    connect(ui->dynsys_preset,  SIGNAL(currentIndexChanged(int)),   this,   SLOT(DynsysPresetSelectionUpdated()));
    connect(ui->vbmode,         SIGNAL(currentIndexChanged(int)),   this,   SLOT(OnRelease()));
    connect(ui->vcmode,         SIGNAL(currentIndexChanged(int)),   this,   SLOT(OnRelease()));
    connect(ui->colmpreset,     SIGNAL(currentIndexChanged(int)),   this,   SLOT(ColmPresetSelectionUpdated()));
}
