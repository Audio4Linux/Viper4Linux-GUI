#include "settings.h"
#include "ui_settings.h"
#include "main.h"
#include "palette.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>
#include <QStyleFactory>

using namespace std;
static bool lockslot = false;
settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings){
    ui->setupUi(this);

    lockslot = true;
    connect(ui->styleSelect,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changeStyle(const QString&)));
    connect(ui->paletteSelect,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changePalette(const QString&)));
    connect(ui->paletteConfig,SIGNAL(clicked()),this,SLOT(openPalConfig()));

    appconf = mainwin->getACWrapper();

    ui->path->setText(appconf->getPath());
    ui->irspath->setText(appconf->getIrsPath());
    ui->autofx->setChecked(appconf->getAutoFx());
    ui->muteonrestart->setChecked(appconf->getMuteOnRestart());
    ui->glavafix->setChecked(appconf->getGFix());

    connect(ui->close, SIGNAL(clicked()), this, SLOT(reject()));
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
    ui->paletteSelect->addItem("Purple","purple");
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

    ui->deftab_favorite->setChecked(!appconf->getConv_DefTab());
    ui->deftab_filesys->setChecked(appconf->getConv_DefTab());

    lockslot = false;
}
settings::~settings(){
    delete ui;
}
void settings::updateTheme(){
    if(lockslot)return;
    appconf->setTheme(ui->themeSelect->itemText(ui->themeSelect->currentIndex()).toUtf8().constData());
}
void settings::updateAutoFX(){
    appconf->setAutoFx(ui->autofx->isChecked());
}
void settings::updateMuteRestart(){
    appconf->setMuteOnRestart(ui->muteonrestart->isChecked());
};
void settings::updatePath(){
    appconf->setPath(ui->path->text());
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Restart required", "Please restart this application to make sure all changes are applied correctly.\n"
                                                            "Press 'OK' to quit or 'Cancel' if you want to continue without a restart.",
                                  QMessageBox::Ok|QMessageBox::Cancel);
    if (reply == QMessageBox::Ok)
      QApplication::quit();
}
void settings::updateIrsPath(){
    appconf->setIrsPath(ui->irspath->text());
}
void settings::updateGLava(){
    appconf->setGFix(ui->glavafix->isChecked());
}
void settings::updateAutoFxMode(){
    if(lockslot)return;
    int mode = 0;
    if(ui->aa_instant->isChecked())mode=0;
    else if(ui->aa_release->isChecked())mode=1;
    appconf->setAutoFxMode(mode);
}
void settings::updateCDefTab(){
    if(lockslot)return;
    int mode = 0;
    if(ui->deftab_favorite->isChecked())mode=0;
    else if(ui->deftab_filesys->isChecked())mode=1;
    appconf->setConv_DefTab(mode);
}
void settings::changeThemeMode(){
    if(lockslot)return;

    int mode = 0;
    if(ui->uimode_css->isChecked())mode=0;
    else if(ui->uimode_pal->isChecked())mode=1;

    ui->styleSelect->setEnabled(!mode);
    ui->paletteSelect->setEnabled(mode);
    ui->paletteConfig->setEnabled(mode && appconf->getColorpalette()=="custom");
    appconf->setThememode(mode);
}
void settings::changePalette(const QString&){
    if(lockslot)return;
    appconf->setColorpalette(ui->paletteSelect->itemData(ui->paletteSelect->currentIndex()).toString());
    ui->paletteConfig->setEnabled(appconf->getColorpalette()=="custom");
}
void settings::openPalConfig(){
    auto c = new class palette(this);
    c->setFixedSize(c->geometry().width(),c->geometry().height());
    c->show();
}
void settings::changeStyle(const QString&){
    if(lockslot)return;
    appconf->setStylesheet(ui->styleSelect->itemData(ui->styleSelect->currentIndex()).toString());
}
void settings::github(){
    QDesktopServices::openUrl(QUrl("https://github.com/ThePBone/Viper4Linux-GUI-Legacy"));
}
void settings::reject()
{
    mainwin->EnableSettingButton(true);
    QDialog::reject();
}
void settings::glava_help(){
    QMessageBox *msgBox = new QMessageBox(this);
     msgBox->setText("This fix kills GLava (desktop visualizer) and restarts it after a new config has been applied.\nThis prevents GLava to switch to another audio sink, while V4L is restarting.");
     msgBox->setStandardButtons(QMessageBox::Ok);
     msgBox->setDefaultButton(QMessageBox::Ok);
     msgBox->exec();
}
