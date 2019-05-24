#include "settings.h"
#include "ui_settings.h"
#include "main.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QDialog>
#include <unistd.h>
#include <pwd.h>
#include <QCloseEvent>
#include <cstdio>
#include <cstdlib>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>

using namespace std;
static settings* obj;
settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings){
    ui->setupUi(this);
    obj = this;

    connect(ui->styleSelect,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changeStyle(const QString&)));

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char result[100];
    strcpy(result,homedir);
    strcat(result,"/.config/viper4linux/audio.conf");
    string path = mainwin->getPath();
    string style_sheet = mainwin->getStylesheet();
    if(path.empty()) ui->path->setText(QString::fromUtf8(result));
    else ui->path->setText(QString::fromStdString(path));
    ui->autofx->setChecked(mainwin->getAutoFx());
    ui->muteonrestart->setChecked(mainwin->getMuteOnRestart());
    ui->glavafix->setChecked(mainwin->getGFix());
    connect(ui->save, SIGNAL(clicked()), this, SLOT(submit()));
    connect(ui->github, SIGNAL(clicked()), this, SLOT(github()));
    connect(ui->glavafix_help, SIGNAL(clicked()), this, SLOT(glava_help()));


    ui->styleSelect->addItem("Default","default");
    ui->styleSelect->addItem("Dark Orange","dark_orange");
    ui->styleSelect->addItem("Dark Blue","dark_blue");
    QVariant qvS(QString::fromStdString(style_sheet));
    int index = ui->styleSelect->findData(qvS);
    if ( index != -1 ) {
       ui->styleSelect->setCurrentIndex(index);
    }
}
settings::~settings(){
    delete ui;
}
void settings::submit(){
    mainwin->setGFix(ui->glavafix->isChecked());
    mainwin->setPath(ui->path->text().toUtf8().constData());
    mainwin->setAutoFx(ui->autofx->isChecked());
    mainwin->setMuteOnRestart(ui->muteonrestart->isChecked());

    this->close();
}
void settings::changeStyle(const QString& style){
    mainwin->setStylesheet(ui->styleSelect->itemData(ui->styleSelect->currentIndex()).toString().toUtf8().constData());
}
void settings::github(){
    QDesktopServices::openUrl(QUrl("https://github.com/ThePBone/Viper4Linux-GUI"));
}
void settings::reject()
{
    mainwin->enableSetBtn(true);
    QDialog::reject();
}
void settings::glava_help(){
    QMessageBox *msgBox = new QMessageBox(this);
     msgBox->setText("This fix kills GLava (desktop visualizer) and restarts it after a new config has been applied.\nThis prevents GLava to switch to another audio sink, while V4L is restarting.");
     msgBox->setStandardButtons(QMessageBox::Ok);
     msgBox->setDefaultButton(QMessageBox::Ok);
     msgBox->exec();
}
