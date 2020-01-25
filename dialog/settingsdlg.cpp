#include "settingsdlg.h"
#include "ui_settings.h"
#include "mainwindow.h"
#include "palettedlg.h"
#include "misc/autostartmanager.h"

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
    connect(ui->selector,static_cast<void (QTreeWidget::*)(QTreeWidgetItem*,QTreeWidgetItem*)>(&QTreeWidget::currentItemChanged),this,[this](QTreeWidgetItem* cur, QTreeWidgetItem* prev){
        ui->stackedWidget->setCurrentIndex(ui->selector->indexOfTopLevelItem(cur));
    });

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
    ui->styleSelect->addItem("Breeze Light","breeze_light");
    ui->styleSelect->addItem("Breeze Dark","breeze_dark");
    ui->styleSelect->addItem("Gray","dark_orange");
    ui->styleSelect->addItem("MacOS","aqua");
    ui->styleSelect->addItem("Material Dark","materialdark");
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
    ui->paletteSelect->addItem("Silver","silver");
    ui->paletteSelect->addItem("Solarized","solarized");
    ui->paletteSelect->addItem("White","white");
    ui->paletteSelect->addItem("Custom","custom");

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
