#include "settings.h"
#include "ui_settings.h"
#include "main.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QDialog>
#include <unistd.h>
#include <pwd.h>

using namespace std;
settings::settings(QWidget *parent) :
QDialog(parent),
ui(new Ui::settings){
    ui->setupUi(this);
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
