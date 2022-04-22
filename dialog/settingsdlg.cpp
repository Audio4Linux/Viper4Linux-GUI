#include "settingsdlg.h"
#include "ui_settings.h"
#include "viper_window.h"
#include "ui_viper_window.h"
#include "palettedlg.h"
#include "misc/autostartmanager.h"
#ifndef VIPER_PLUGINMODE
#include "qmessageoverlay.h"
#include "crashhandler/killer.h"
#endif
#include "misc/GstRegistryHelper.h"
#include "config/appconfigwrapper.h"

#include <QGraphicsOpacityEffect>
#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include <QStyleFactory>
#include <QSystemTrayIcon>
#include <qtimer.h>

#include <config/container.h>
#include <config/io.h>

using namespace std;
static bool lockslot = false;

SettingsDlg::SettingsDlg(ViperWindow* mainwin,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings){
    ui->setupUi(this);
    m_mainwin = mainwin;

    appconf = mainwin->getACWrapper();

    /* Hide troubleshooting section for now */
    ui->dt_box->hide();

#ifdef VIPER_PLUGINMODE
    for(int i = 0; i < ui->selector->topLevelItemCount(); i++){
        auto item = ui->selector->topLevelItem(i);
        if(item->text(0) == "Devices" ||
           item->text(0) == "General" ||
           item->text(0) == "Systray" ||
           item->text(0) == "Spectrum Analyser")
            item->setHidden(true);
    }

#else
    ui->themeSelect->addItem("Phantom");
    QString autostart_path = AutostartManager::getAutostartPath("viper-gui.desktop");
#endif

    /*
     * Prepare TreeView
     */
#ifdef VIPER_PLUGINMODE
    ui->selector->setCurrentItem(ui->selector->findItems("Interface",Qt::MatchFlag::MatchExactly).first());
    ui->stackedWidget->setCurrentIndex(1);
#else
    ui->selector->setCurrentItem(ui->selector->findItems(tr("General"),Qt::MatchFlag::MatchExactly).first());
    ui->stackedWidget->setCurrentIndex(0);
#endif

    connect(ui->selector,static_cast<void (QTreeWidget::*)(QTreeWidgetItem*,QTreeWidgetItem*)>(&QTreeWidget::currentItemChanged),this,[this](QTreeWidgetItem* cur, QTreeWidgetItem*){
        int toplevel_index = ui->selector->indexOfTopLevelItem(cur);
        switch(toplevel_index){
        case -1:
            if(cur->text(0) == tr("Context Menu"))
                ui->stackedWidget->setCurrentIndex(5);
            if(cur->text(0) == tr("Design"))
                ui->stackedWidget->setCurrentIndex(7);
            if(cur->text(0) == tr("Advanced"))
                ui->stackedWidget->setCurrentIndex(8);
            break;
        case 5:
            // -- SA-ROOT
            ui->stackedWidget->setCurrentIndex(6);
            break;
        case 6:
            // -- GST-INFO
            ui->stackedWidget->setCurrentIndex(9);
            break;
        default:
            ui->stackedWidget->setCurrentIndex(toplevel_index);
        }
    });
#ifndef VIPER_PLUGINMODE
    ui->selector->expandItem(ui->selector->findItems(tr("Spectrum Analyser"),Qt::MatchFlag::MatchExactly).first());
    ui->selector->expandItem(ui->selector->findItems(tr("Systray"),Qt::MatchFlag::MatchExactly).first());

    /*
     * Prepare all combooxes
     */
    ui->paletteSelect->addItem(tr("Default"),"default");
    ui->paletteSelect->addItem(tr("Black"),"black");
    ui->paletteSelect->addItem(tr("Blue"),"blue");
    ui->paletteSelect->addItem(tr("Dark Blue"),"darkblue");
    ui->paletteSelect->addItem(tr("Dark Green"),"darkgreen");
    ui->paletteSelect->addItem(tr("Gray"),"gray");
    ui->paletteSelect->addItem(tr("Green"),"green");
    ui->paletteSelect->addItem(tr("Honeycomb"),"honeycomb");
    ui->paletteSelect->addItem(tr("Pitch Black"),"pitchblack");
    ui->paletteSelect->addItem(tr("Stone"),"stone");
    ui->paletteSelect->addItem(tr("White"),"white");
    ui->paletteSelect->addItem(tr("Custom"),"custom");

    ui->languageSelect->addItem("English", "en");
    ui->languageSelect->addItem("German", "de");
    ui->languageSelect->addItem("Simplified Chinese", "zh_CN");
    ui->languageSelect->addItem("Traditional Chinese (Hong Kong)", "zh_HK");

#else
    /* Remove palette selection */
    ui->design_lay->removeRow(1);
#endif

    for ( const auto& i : QStyleFactory::keys() )
        ui->themeSelect->addItem(i);

    /*
     * Refresh all input fields
     */
    refreshAll();

    /*
     * Connect all signals for page General
     */
    connect(ui->languageSelect,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),this,[this]{
        appconf->setLanguage(ui->languageSelect->itemData(ui->languageSelect->currentIndex()).toString());
    });

#ifndef VIPER_PLUGINMODE
    connect(ui->glavafix, &QPushButton::clicked, this, [this]{
        appconf->setGFix(ui->glavafix->isChecked());
    });
    connect(ui->dt_disable_sync, &QPushButton::clicked, this, [this]{
        appconf->setSyncDisabled(ui->dt_disable_sync->isChecked());
    });
    connect(ui->autofx, &QGroupBox::clicked, this, [this]{
        appconf->setAutoFx(ui->autofx->isChecked());
    });
    connect(ui->run_first_launch, &QPushButton::clicked, this, [this,mainwin]{
        emit closeClicked();
        QTimer::singleShot(500,this,[mainwin]{
            mainwin->LaunchFirstRunSetup();
        });
    });
    auto autofx_mode = [this]{
        if(lockslot)return;
        int mode = 0;
        if(ui->aa_instant->isChecked())mode=0;
        else if(ui->aa_release->isChecked())mode=1;
        appconf->setAutoFxMode(mode);
    };
    connect(ui->aa_instant, &QRadioButton::clicked, this, autofx_mode);
    connect(ui->aa_release, &QRadioButton::clicked, this, autofx_mode);
    connect(ui->glavafix_help, &QPushButton::clicked, this, [this]{
        QMessageBox::information(this,tr("Help"),
                                 tr("This fix kills GLava (desktop visualizer) and restarts it after a new config has been applied.\nThis prevents GLava to switch to another audio sink, while V4L is restarting."));
    });
#endif
#ifndef VIPER_PLUGINMODE
    /*
     * Connect all signals for Session
     */
    auto systray_sel = [this,mainwin]{
        if(lockslot)return;
        int mode = 0;
        if(ui->systray_r_none->isChecked())mode=0;
        else if(ui->systray_r_showtray->isChecked())mode=1;
        appconf->setTrayMode(mode);
        mainwin->setTrayVisible(mode);
        ui->systray_icon_box->setEnabled(mode);
        ui->menu_edit->setEnabled(mode);
    };
    connect(ui->systray_r_none,&QRadioButton::clicked,this,systray_sel);
    connect(ui->systray_r_showtray,&QRadioButton::clicked,this,systray_sel);
    auto autostart_update = [this,mainwin,autostart_path]{
        if(ui->systray_minOnBoot->isChecked()){
            AutostartManager::saveDesktopFile(autostart_path,mainwin->GetExecutablePath(),
                                              ui->systray_autostartViper->isChecked(),
                                              ui->systray_delay->isChecked());
        }
        else QFile(autostart_path).remove();
        ui->systray_autostartViper->setEnabled(ui->systray_minOnBoot->isChecked());
        ui->systray_delay->setEnabled(ui->systray_minOnBoot->isChecked());
    };
    connect(ui->systray_minOnBoot,&QPushButton::clicked,this,autostart_update);
    connect(ui->systray_autostartViper,&QPushButton::clicked,this,autostart_update);
    connect(ui->systray_delay,&QPushButton::clicked,this,autostart_update);
#endif

    /*
     * Connect all signals for Interface
     */

    connect(ui->themeSelect, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this, [this](const QString&){
        if(lockslot)return;
        appconf->setTheme(ui->themeSelect->itemText(ui->themeSelect->currentIndex()).toUtf8().constData());
    });
#ifndef VIPER_PLUGINMODE
    connect(ui->paletteSelect,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),this,[this]{
        if(lockslot)return;
        appconf->setColorpalette(ui->paletteSelect->itemData(ui->paletteSelect->currentIndex()).toString());
        ui->paletteConfig->setEnabled(appconf->getColorpalette()=="custom");
    });
    connect(ui->paletteConfig,&QPushButton::clicked,this,[this]{
        auto c = new class PaletteEditor(appconf,this);
        c->setFixedSize(c->geometry().width(),c->geometry().height());
        c->show();
    });
#endif
    connect(ui->eq_alwaysdrawhandles,&QCheckBox::clicked,[this](){
        appconf->setEqualizerPermanentHandles(ui->eq_alwaysdrawhandles->isChecked());
    });

    /*
     * Connect all signals for Convolver
     */
    auto deftab_mode = [this]{
        if(lockslot)return;
        int mode = 0;
        if(ui->deftab_favorite->isChecked())mode=0;
        else if(ui->deftab_filesys->isChecked())mode=1;
        appconf->setConv_DefTab(mode);
    };
    connect(ui->deftab_filesys, &QRadioButton::clicked, this, deftab_mode);
    connect(ui->deftab_favorite, &QRadioButton::clicked, this, deftab_mode);
    connect(ui->saveirspath, &QPushButton::clicked, this, [this]{
        appconf->setIrsPath(ui->irspath->text());
    });

    /*
     * Connect all signals for Devices
     */
#ifndef VIPER_PLUGINMODE
    auto deviceUpdated = [this](){
        if(lockslot) return;
        QString absolute =
                QFileInfo(appconf->getPath()).absoluteDir().absolutePath();
        QString devices(pathAppend(absolute,"devices.conf"));
        if(ui->dev_mode_auto->isChecked()){
            QFile(devices).remove();
        }else{
            if(ui->dev_select->currentData() == "---")
                return;

            ConfigContainer* devconf = new ConfigContainer();
            devconf->setConfigMap(ConfigIO::readFile(devices));
            devconf->setValue("location",ui->dev_select->currentData());
            ConfigIO::writeFile(devices,devconf->getConfigMap());
        }
    };
    connect(ui->dev_reload_viper,&QPushButton::clicked,mainwin,&ViperWindow::Restart);
    connect(ui->dev_mode_auto,&QRadioButton::clicked,this,deviceUpdated);
    connect(ui->dev_mode_manual,&QRadioButton::clicked,this,deviceUpdated);
    connect(ui->dev_select,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, deviceUpdated);
#endif
#ifndef VIPER_PLUGINMODE
    /*
     * Connect all signals for SA/ROOT
     */
    connect(ui->sa_enable,&QGroupBox::clicked,this,[this,mainwin](){
        appconf->setSpectrumEnable(ui->sa_enable->isChecked());
        ui->spectrum_theme->setEnabled(ui->sa_enable->isChecked());
        ui->spectrum_advanced->setEnabled(ui->sa_enable->isChecked());
        emit closeClicked();
        mainwin->ui->set->click();
    });
    connect(ui->sa_bands,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumBands(number);
    });
    connect(ui->sa_minfreq,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumMinFreq(number);
    });
    connect(ui->sa_maxfreq,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumMaxFreq(number);
    });
    connect(ui->sa_input,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, [this](const QString& str){
        if(lockslot)return;
        appconf->setSpectrumInput(str);
    });
    connect(ui->sa_type,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, [this](const QString&){
        if(lockslot)return;
        appconf->setSpectrumShape(ui->sa_type->currentIndex());
    });
    /*
     * Connect all signals for SA/Design
     */
    connect(ui->sa_grid,&QCheckBox::clicked,this,[this](){
        appconf->setSpectrumGrid(ui->sa_grid->isChecked());
    });
    auto sa_theme_sel = [this]{
        int mode = 0;
        if(ui->sa_theme_default->isChecked())mode=0;
        else if(ui->sa_theme_inherit->isChecked())mode=1;
        appconf->setSpectrumTheme(mode);
    };
    connect(ui->sa_theme_default,&QRadioButton::clicked,this,sa_theme_sel);
    connect(ui->sa_theme_inherit,&QRadioButton::clicked,this,sa_theme_sel);

    /*
     * Connect all signals for SA/Advanced
     */
    connect(ui->sa_refresh,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumRefresh(number);
    });
    connect(ui->sa_multi,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[this](double number){
        appconf->setSpectrumMultiplier(number);
    });
#endif

    /*
     * Connect all signals for Global
     */
    connect(ui->close, &QPushButton::clicked, this, &SettingsDlg::closeClicked);
    connect(ui->github, &QPushButton::clicked, this, []{
        QDesktopServices::openUrl(QUrl("https://github.com/Audio4Linux/Viper4Linux-GUI"));
    });

#ifndef VIPER_PLUGINMODE
    connect(ui->menu_edit,&QMenuEditor::targetChanged,[mainwin,this]{
        auto menu = ui->menu_edit->exportMenu();
        mainwin->updateTrayMenu(menu);
    });
    connect(ui->menu_edit,&QMenuEditor::resetPressed,[mainwin,this]{
        QMessageBox::StandardButton reply = QMessageBox::question(this,tr("Warning"), tr("Do you really want to restore the default layout?"),
                                                                  QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            ui->menu_edit->setTargetMenu(mainwin->buildDefaultActions());
            auto menu = ui->menu_edit->exportMenu();
            mainwin->updateTrayMenu(menu);
        }
    });
    ui->menu_edit->setSourceMenu(mainwin->buildAvailableActions());

    /*
     * Check for systray availability
     */
#ifndef QT_NO_SYSTEMTRAYICON
    ui->systray_unsupported->hide();
#else
    ui->session->setEnabled(false);
#endif
    if(!QSystemTrayIcon::isSystemTrayAvailable()){
        ui->systray_unsupported->show();
        ui->session->setEnabled(false);
    }
#endif
}
SettingsDlg::~SettingsDlg(){
    delete ui;
}
void SettingsDlg::refreshDevices()
{
#ifndef VIPER_PLUGINMODE
    lockslot = true;
    ui->dev_select->clear();
    QString absolute =
            QFileInfo(appconf->getPath()).absoluteDir().absolutePath();
    QFile devices(pathAppend(absolute,"devices.conf"));
    bool devmode_auto = !devices.exists();
    ui->dev_mode_auto->setChecked(devmode_auto);
    ui->dev_mode_manual->setChecked(!devmode_auto);

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LC_ALL", "C");
    process.setProcessEnvironment(env);
    process.start("sh", QStringList()<<"-c"<<"pactl list sinks | grep \'Name: \' -A1");
    process.waitForFinished(500);

    ConfigContainer* devconf = new ConfigContainer();
    devconf->setConfigMap(ConfigIO::readFile(pathAppend(absolute,"devices.conf")));
    QString out = process.readAllStandardOutput();
    ui->dev_select->addItem("...","---");
    for(auto item : out.split("Name:")){
        item.prepend("Name:");
        QRegularExpression re(R"((?<=(Name:)\s)(?<name>.+)[\s\S]+(?<=(Description:)\s)(?<desc>.+))");
        QRegularExpressionMatch match = re.match(item, 0, QRegularExpression::PartialPreferCompleteMatch);
        if(match.hasMatch()){
            ui->dev_select->addItem(QString("%1 (%2)").arg(match.captured("desc")).arg(match.captured("name")),
                                    match.captured("name"));
        }
    }
    QString dev_location = devconf->getString("location");
    if(dev_location.isEmpty())
        ui->dev_select->setCurrentIndex(0);
    else{
        bool notFound = true;
        for(int i = 0; i < ui->dev_select->count(); i++){
            if(ui->dev_select->itemData(i) ==
                    dev_location){
                notFound = false;
                ui->dev_select->setCurrentIndex(i);
                break;
            }
        }
        if(notFound){
            QString name = QString(tr("Unknown (%1)")).arg(dev_location);
            ui->dev_select->addItem(name,dev_location);
            ui->dev_select->setCurrentText(name);
        }
    }
    lockslot = false;
#endif
}

void SettingsDlg::refreshAll(){
    lockslot = true;

    ui->gst_plg_installed->setText(GstRegistryHelper::IsPluginInstalled() ? tr("Yes") : tr("No"));
    ui->gst_plg_mode->setText(GstRegistryHelper::HasDBusSupport() ? "Audio4Linux" : "Legacy");
    ui->gst_plg_dbus->setText(GstRegistryHelper::HasDBusSupport() ? tr("Yes") : tr("No"));
    ui->gst_plg_version->setText(GstRegistryHelper::GetPluginVersion());
    ui->gst_plg_path->setText(GstRegistryHelper::GetPluginPath());
    ui->gst_plg_path->setToolTip(GstRegistryHelper::GetPluginPath());

#ifndef VIPER_PLUGINMODE
    QString autostart_path = AutostartManager::getAutostartPath("viper-gui.desktop");

    ui->menu_edit->setTargetMenu(m_mainwin->getTrayContextMenu());
    ui->menu_edit->setIconStyle(appconf->getWhiteIcons());

    ui->autofx->setChecked(appconf->getAutoFx());
    ui->glavafix->setChecked(appconf->getGFix());
    ui->dt_disable_sync->setChecked(appconf->getSyncDisabled());

    QVariant qvLang(appconf->getLanguage());
    int indexLang = ui->languageSelect->findData(qvLang);
    if ( indexLang != -1 )
        ui->languageSelect->setCurrentIndex(indexLang);

    updateInputSinks();
#endif
    ui->irspath->setText(appconf->getIrsPath());

    QString qvT(appconf->getTheme());
    int indexT = ui->themeSelect->findText(qvT);
    if ( indexT != -1 ) {
        ui->themeSelect->setCurrentIndex(indexT);
    }else{
        int index_fallback = ui->themeSelect->findText("Fusion");
        if ( index_fallback != -1 )
            ui->themeSelect->setCurrentIndex(index_fallback);
    }

#ifndef VIPER_PLUGINMODE
    QVariant qvS2(appconf->getColorpalette());
    int index2 = ui->paletteSelect->findData(qvS2);
    if ( index2 != -1 )
        ui->paletteSelect->setCurrentIndex(index2);

    ui->paletteConfig->setEnabled(appconf->getColorpalette()=="custom");

    ui->aa_instant->setChecked(!appconf->getAutoFxMode());//same here..
    ui->aa_release->setChecked(appconf->getAutoFxMode());

    ui->systray_r_none->setChecked(!appconf->getTrayMode());
    ui->systray_r_showtray->setChecked(appconf->getTrayMode());
    ui->systray_icon_box->setEnabled(appconf->getTrayMode());
    ui->menu_edit->setEnabled(appconf->getTrayMode());

    bool autostart_enabled = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::Exists);
    bool autostartviper_enabled = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::UsesViperAutostart);
    bool autostart_delayed = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::Delayed);

    ui->systray_minOnBoot->setChecked(autostart_enabled);
    ui->systray_autostartViper->setEnabled(autostart_enabled);
    ui->systray_autostartViper->setChecked(autostartviper_enabled);
    ui->systray_delay->setEnabled(autostart_enabled);
    ui->systray_delay->setChecked(autostart_delayed);
#endif

    ui->deftab_favorite->setChecked(!appconf->getConv_DefTab());
    ui->deftab_filesys->setChecked(appconf->getConv_DefTab());

    ui->eq_alwaysdrawhandles->setChecked(appconf->getEqualizerPermanentHandles());

#ifndef VIPER_PLUGINMODE
    refreshDevices();

    int bands = appconf->getSpectrumBands();
    int minfreq = appconf->getSpectrumMinFreq();
    int maxfreq = appconf->getSpectrumMaxFreq();
    int refresh = appconf->getSpectrumRefresh();
    float multiplier = appconf->getSpectrumMultiplier();
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
    ui->sa_enable->setChecked(appconf->getSpetrumEnable());
    ui->spectrum_theme->setEnabled(appconf->getSpetrumEnable());
    ui->spectrum_advanced->setEnabled(appconf->getSpetrumEnable());

    ui->sa_type->setCurrentIndex(appconf->getSpectrumShape());
    ui->sa_bands->setValue(bands);
    ui->sa_minfreq->setValue(minfreq);
    ui->sa_maxfreq->setValue(maxfreq);
    ui->sa_grid->setChecked(appconf->getSpetrumGrid());
    ui->sa_refresh->setValue(refresh);
    ui->sa_multi->setValue(multiplier);

    ui->sa_theme_default->setChecked(!appconf->getSpectrumTheme());
    ui->sa_theme_inherit->setChecked(appconf->getSpectrumTheme());
#endif
    lockslot = false;
}

void SettingsDlg::updateButtonStyle(bool white)
{
#ifndef VIPER_PLUGINMODE
    ui->menu_edit->setIconStyle(white);
#else
    Q_UNUSED(white);
#endif
}

void SettingsDlg::setVisible(bool visible)
{
#ifndef VIPER_PLUGINMODE
    refreshDevices();
#endif
    QDialog::setVisible(visible);
}

void SettingsDlg::updateInputSinks(){
#ifndef VIPER_PLUGINMODE
    lockslot = true;
    ui->sa_input->clear();
    for ( const auto& dev: QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
        ui->sa_input->addItem(dev.deviceName());

    QString qvSA(appconf->getSpectrumInput());
    int indexSA = ui->sa_input->findText(qvSA);
    if ( indexSA != -1 ) {
        ui->sa_input->setCurrentIndex(indexSA);
    }else{
        int index_fallback = ui->themeSelect->findText("default");
        if ( index_fallback != -1 )
            ui->sa_input->setCurrentIndex(index_fallback);
    }
    lockslot = false;
#endif
}

