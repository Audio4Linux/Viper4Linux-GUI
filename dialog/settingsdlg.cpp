#include "settingsdlg.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "palettedlg.h"
#include "misc/autostartmanager.h"

#include <QProcess>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include <QStyleFactory>
#include <QSystemTrayIcon>

using namespace std;
static bool lockslot = false;

SettingsDlg::SettingsDlg(MainWindow* mainwin,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings){
    ui->setupUi(this);
    m_mainwin = mainwin;

    lockslot = true;
    connect(ui->styleSelect,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changeStyle(const QString&)));
    connect(ui->paletteSelect,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changePalette(const QString&)));
    connect(ui->paletteConfig,SIGNAL(clicked()),this,SLOT(openPalConfig()));

    appconf = mainwin->getACWrapper();
    QString autostart_path = AutostartManager::getAutostartPath("viper-gui.desktop");

    ui->path->setText(appconf->getPath());
    ui->irspath->setText(appconf->getIrsPath());
    ui->autofx->setChecked(appconf->getAutoFx());
    ui->muteonrestart->setChecked(appconf->getMuteOnRestart());
    ui->glavafix->setChecked(appconf->getGFix());
    ui->reloadmethod->setCurrentIndex((int)appconf->getReloadMethod());

    connect(ui->close, &QPushButton::clicked, this, &SettingsDlg::closeClicked);
    connect(ui->github, SIGNAL(clicked()), this, SLOT(github()));
    connect(ui->glavafix_help, SIGNAL(clicked()), this, SLOT(glava_help()));
    connect(ui->uimode_css, SIGNAL(clicked()), this, SLOT(changeThemeMode()));
    connect(ui->uimode_pal, SIGNAL(clicked()), this, SLOT(changeThemeMode()));
    connect(ui->themeSelect, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateTheme()));

    connect(ui->aa_instant, SIGNAL(clicked()), this, SLOT(updateAutoFxMode()));
    connect(ui->aa_release, SIGNAL(clicked()), this, SLOT(updateAutoFxMode()));

    connect(ui->deftab_filesys, SIGNAL(clicked()), this, SLOT(updateCDefTab()));
    connect(ui->deftab_favorite, SIGNAL(clicked()), this, SLOT(updateCDefTab()));

    connect(ui->glavafix, SIGNAL(clicked()), this, SLOT(updateGLava()));
    connect(ui->autofx, SIGNAL(clicked()), this, SLOT(updateAutoFX()));
    connect(ui->muteonrestart, SIGNAL(clicked()), this, SLOT(updateMuteRestart()));
    connect(ui->savepath, SIGNAL(clicked()), this, SLOT(updatePath()));
    connect(ui->saveirspath, SIGNAL(clicked()), this, SLOT(updateIrsPath()));

    ui->selector->setCurrentItem(ui->selector->findItems("General",Qt::MatchFlag::MatchExactly).first());
    ui->stackedWidget->setCurrentIndex(0);
    connect(ui->selector,static_cast<void (QTreeWidget::*)(QTreeWidgetItem*,QTreeWidgetItem*)>(&QTreeWidget::currentItemChanged),this,[this](QTreeWidgetItem* cur, QTreeWidgetItem* prev){
        int toplevel_index = ui->selector->indexOfTopLevelItem(cur);
        switch(toplevel_index){
        case -1:
            if(cur->text(0) == "Design")
                ui->stackedWidget->setCurrentIndex(6);
            if(cur->text(0) == "Advanced")
                ui->stackedWidget->setCurrentIndex(7);
            break;
        default:
            ui->stackedWidget->setCurrentIndex(toplevel_index);
        }
    });
    ui->selector->expandItem(ui->selector->findItems("Spectrum Analyser",Qt::MatchFlag::MatchExactly).first());

    connect(ui->reloadmethod,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,[this](){
        if(lockslot)return;
        appconf->setReloadMethod((ReloadMethod)ui->reloadmethod->currentIndex());
    });
    connect(ui->systray_r_none,&QRadioButton::clicked,this,[this,mainwin](){
        if(lockslot)return;
        int mode = 0;
        if(ui->systray_r_none->isChecked())mode=0;
        else if(ui->systray_r_showtray->isChecked())mode=1;
        appconf->setTrayMode(mode);
        mainwin->setTrayVisible(mode);
        ui->systray_icon_box->setEnabled(mode);
    });
    connect(ui->systray_r_showtray,&QRadioButton::clicked,this,[this,mainwin](){
        if(lockslot)return;
        int mode = 0;
        if(ui->systray_r_none->isChecked())mode=0;
        else if(ui->systray_r_showtray->isChecked())mode=1;
        appconf->setTrayMode(mode);
        mainwin->setTrayVisible(mode);
        ui->systray_icon_box->setEnabled(mode);
    });
    connect(ui->systray_minOnBoot,&QPushButton::clicked,this,[this,mainwin,autostart_path](){
        if(ui->systray_minOnBoot->isChecked()){
            AutostartManager::saveDesktopFile(autostart_path,mainwin->GetExecutablePath(),
                                              ui->systray_autostartViper->isChecked());
        }
        else QFile(autostart_path).remove();
        ui->systray_autostartViper->setEnabled(ui->systray_minOnBoot->isChecked());
    });
    connect(ui->systray_autostartViper,&QPushButton::clicked,this,[this,mainwin,autostart_path](){
        if(ui->systray_minOnBoot->isChecked())
            AutostartManager::saveDesktopFile(autostart_path,mainwin->GetExecutablePath(),
                                              ui->systray_autostartViper->isChecked());
        else QFile(autostart_path).remove();
    });

    ui->styleSelect->addItem("Default","default");
    ui->styleSelect->addItem("Black","amoled");
    ui->styleSelect->addItem("Blue","blue");
    ui->styleSelect->addItem("MacOS","aqua");
    ui->styleSelect->addItem("Ubuntu","ubuntu");
    ui->styleSelect->addItem("Visual Studio Dark","vsdark");
    ui->styleSelect->addItem("Visual Studio Light","vslight");

    ui->paletteSelect->addItem("Default","default");
    ui->paletteSelect->addItem("Black","black");
    ui->paletteSelect->addItem("Blue","blue");
    ui->paletteSelect->addItem("Dark","dark");
    ui->paletteSelect->addItem("Dark Blue","darkblue");
    ui->paletteSelect->addItem("Dark Green","darkgreen");

    ui->paletteSelect->addItem("Honeycomb","honeycomb");
    ui->paletteSelect->addItem("Gray","gray");
    ui->paletteSelect->addItem("Green","green");
    ui->paletteSelect->addItem("Stone","stone");
    ui->paletteSelect->addItem("White","white");
    ui->paletteSelect->addItem("Custom","custom");

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

    for ( const auto& i : QStyleFactory::keys() )
        ui->themeSelect->addItem(i);

    QString qvT(appconf->getTheme());
    int indexT = ui->themeSelect->findText(qvT);
    if ( indexT != -1 ) {
        ui->themeSelect->setCurrentIndex(indexT);
    }else{
        int index_fallback = ui->themeSelect->findText("Fusion");
        if ( index_fallback != -1 )
            ui->themeSelect->setCurrentIndex(index_fallback);
    }

    QVariant qvS(appconf->getStylesheet());
    int index = ui->styleSelect->findData(qvS);
    if ( index != -1 )
        ui->styleSelect->setCurrentIndex(index);

    QVariant qvS2(appconf->getColorpalette());
    int index2 = ui->paletteSelect->findData(qvS2);
    if ( index2 != -1 )
        ui->paletteSelect->setCurrentIndex(index2);

    ui->styleSelect->setEnabled(!appconf->getThememode());
    ui->paletteConfig->setEnabled(appconf->getThememode() && appconf->getColorpalette()=="custom");
    ui->paletteSelect->setEnabled(appconf->getThememode());

    ui->uimode_css->setChecked(!appconf->getThememode());//If 0 set true, else false
    ui->uimode_pal->setChecked(appconf->getThememode());//If 0 set false, else true

    ui->aa_instant->setChecked(!appconf->getAutoFxMode());//same here..
    ui->aa_release->setChecked(appconf->getAutoFxMode());

    ui->systray_r_none->setChecked(!appconf->getTrayMode());
    ui->systray_r_showtray->setChecked(appconf->getTrayMode());
    ui->systray_icon_box->setEnabled(appconf->getTrayMode());

    bool autostart_enabled = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::Exists);
    bool autostartviper_enabled = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::UsesViperAutostart);

    ui->systray_minOnBoot->setChecked(autostart_enabled);
    ui->systray_autostartViper->setEnabled(autostart_enabled);
    ui->systray_autostartViper->setChecked(autostartviper_enabled);

    ui->deftab_favorite->setChecked(!appconf->getConv_DefTab());
    ui->deftab_filesys->setChecked(appconf->getConv_DefTab());

    ui->eq_alwaysdrawhandles->setChecked(appconf->getEqualizerPermanentHandles());
    connect(ui->eq_alwaysdrawhandles,&QCheckBox::clicked,[this](){
        appconf->setEqualizerPermanentHandles(ui->eq_alwaysdrawhandles->isChecked());
    });

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
    ui->sa_bands->setValue(bands);
    ui->sa_minfreq->setValue(minfreq);
    ui->sa_maxfreq->setValue(maxfreq);
    ui->sa_grid->setChecked(appconf->getSpetrumGrid());
    ui->sa_refresh->setValue(refresh);
    ui->sa_multi->setValue(multiplier);

    ui->sa_theme_default->setChecked(!appconf->getSpectrumTheme());
    ui->sa_theme_inherit->setChecked(appconf->getSpectrumTheme());

    connect(ui->sa_bands,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumBands(number);
    });
    connect(ui->sa_minfreq,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumMinFreq(number);
    });
    connect(ui->sa_maxfreq,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumMaxFreq(number);
    });
    connect(ui->sa_refresh,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),this,[this](int number){
        appconf->setSpectrumRefresh(number);
    });
    connect(ui->sa_multi,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),this,[this](double number){
        appconf->setSpectrumMultiplier(number);
    });
    connect(ui->sa_enable,&QGroupBox::clicked,this,[this,mainwin](){
        appconf->setSpectrumEnable(ui->sa_enable->isChecked());
        emit closeClicked();
        mainwin->ui->set->click();
    });
    connect(ui->sa_grid,&QCheckBox::clicked,this,[this](){
        appconf->setSpectrumGrid(ui->sa_grid->isChecked());
    });
    connect(ui->sa_theme_default,&QRadioButton::clicked,this,[this](){
        int mode = 0;
        if(ui->sa_theme_default->isChecked())mode=0;
        else if(ui->sa_theme_inherit->isChecked())mode=1;
        appconf->setSpectrumTheme(mode);
    });
    connect(ui->sa_theme_inherit,&QRadioButton::clicked,this,[this](){
        int mode = 0;
        if(ui->sa_theme_default->isChecked())mode=0;
        else if(ui->sa_theme_inherit->isChecked())mode=1;
        appconf->setSpectrumTheme(mode);
    });
    connect(ui->sa_input,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, [this](const QString& str){
        if(lockslot)return;
        appconf->setSpectrumInput(str);
    });

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

    connect(ui->dev_reload_viper,&QPushButton::clicked,mainwin,&MainWindow::Restart);
    connect(ui->dev_mode_auto,&QRadioButton::clicked,this,deviceUpdated);
    connect(ui->dev_mode_manual,&QRadioButton::clicked,this,deviceUpdated);
    connect(ui->dev_select,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, deviceUpdated);

#ifndef QT_NO_SYSTEMTRAYICON
    ui->systray_unsupported->hide();
#else
    ui->session->setEnabled(false);
#endif

    if(!QSystemTrayIcon::isSystemTrayAvailable()){
        ui->systray_unsupported->show();
        ui->session->setEnabled(false);
    }

    lockslot = false;
}
SettingsDlg::~SettingsDlg(){
    delete ui;
}
void SettingsDlg::refreshDevices()
{
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
        QRegularExpression re("(?<=(Name:)\\s)(?<name>.+)[\\s\\S]+(?<=(Description:)\\s)(?<desc>.+)");
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
            QString name = QString("Unknown (%1)").arg(dev_location);
            ui->dev_select->addItem(name,dev_location);
            ui->dev_select->setCurrentText(name);
        }
    }
    lockslot = false;
}

void SettingsDlg::setVisible(bool visible)
{
    refreshDevices();
    QWidget::setVisible(visible);
}

void SettingsDlg::updateInputSinks(){
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
}
void SettingsDlg::updateTheme(){
    if(lockslot)return;
    appconf->setTheme(ui->themeSelect->itemText(ui->themeSelect->currentIndex()).toUtf8().constData());
}
void SettingsDlg::updateAutoFX(){
    appconf->setAutoFx(ui->autofx->isChecked());
}
void SettingsDlg::updateMuteRestart(){
    appconf->setMuteOnRestart(ui->muteonrestart->isChecked());
};
void SettingsDlg::updatePath(){
    appconf->setPath(ui->path->text());
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Restart required"), tr("Please restart this application to make sure all changes are applied correctly.\n"
                                                                   "Press 'OK' to quit or 'Cancel' if you want to continue without a restart."),
                                  QMessageBox::Ok|QMessageBox::Cancel);
    if (reply == QMessageBox::Ok)
        QApplication::quit();
}
void SettingsDlg::updateIrsPath(){
    appconf->setIrsPath(ui->irspath->text());
}
void SettingsDlg::updateGLava(){
    appconf->setGFix(ui->glavafix->isChecked());
}
void SettingsDlg::updateAutoFxMode(){
    if(lockslot)return;
    int mode = 0;
    if(ui->aa_instant->isChecked())mode=0;
    else if(ui->aa_release->isChecked())mode=1;
    appconf->setAutoFxMode(mode);
}
void SettingsDlg::updateCDefTab(){
    if(lockslot)return;
    int mode = 0;
    if(ui->deftab_favorite->isChecked())mode=0;
    else if(ui->deftab_filesys->isChecked())mode=1;
    appconf->setConv_DefTab(mode);
}
void SettingsDlg::changeThemeMode(){
    if(lockslot)return;

    int mode = 0;
    if(ui->uimode_css->isChecked())mode=0;
    else if(ui->uimode_pal->isChecked())mode=1;

    ui->styleSelect->setEnabled(!mode);
    ui->paletteSelect->setEnabled(mode);
    ui->paletteConfig->setEnabled(mode && appconf->getColorpalette()=="custom");
    appconf->setThememode(mode);
}
void SettingsDlg::changePalette(const QString&){
    if(lockslot)return;
    appconf->setColorpalette(ui->paletteSelect->itemData(ui->paletteSelect->currentIndex()).toString());
    ui->paletteConfig->setEnabled(appconf->getColorpalette()=="custom");
}
void SettingsDlg::openPalConfig(){
    auto c = new class PaletteEditor(appconf,this);
    c->setFixedSize(c->geometry().width(),c->geometry().height());
    c->show();
}
void SettingsDlg::changeStyle(const QString&){
    if(lockslot)return;
    appconf->setStylesheet(ui->styleSelect->itemData(ui->styleSelect->currentIndex()).toString());
}
void SettingsDlg::github(){
    QDesktopServices::openUrl(QUrl("https://github.com/ThePBone/Viper4Linux-GUI-Legacy"));
}
void SettingsDlg::glava_help(){
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setText(tr("This fix kills GLava (desktop visualizer) and restarts it after a new config has been applied.\nThis prevents GLava to switch to another audio sink, while V4L is restarting."));
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    msgBox->exec();
}
