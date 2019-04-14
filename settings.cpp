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

using namespace std;
static settings* obj;
settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings){
    ui->setupUi(this);
    obj = this;

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char result[100];
    strcpy(result,homedir);
    strcat(result,"/.config/viper4linux/audio.conf");
    string path = mainwin->getPath();
    if(path.empty()) ui->path->setText(QString::fromUtf8(result));
    else ui->path->setText(QString::fromStdString(path));
    ui->autofx->setChecked(mainwin->getAutoFx());
    ui->muteonrestart->setChecked(mainwin->getMuteOnRestart());
    connect(ui->save, SIGNAL(clicked()), this, SLOT(submit()));
    connect(ui->github, SIGNAL(clicked()), this, SLOT(github()));
}
settings::~settings(){
    delete ui;
}
void settings::submit(){
    mainwin->setPath(ui->path->text().toUtf8().constData());
    mainwin->setAutoFx(ui->autofx->isChecked());
    mainwin->setMuteOnRestart(ui->muteonrestart->isChecked());
    this->close();
}
void settings::github(){
    QDesktopServices::openUrl(QUrl("https://github.com/ThePBone/Viper4Linux-GUI"));
}
void settings::reject()
{
    mainwin->enableSetBtn(true);
    QDialog::reject();
}
