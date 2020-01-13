#include "importandroid.h"
#include "ui_importandroid.h"
#include "converter.h"
#include "main.h"
#include "misc/loghelper.h"

#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
importandroid::importandroid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importandroid)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Official Viper4Android",0);
    ui->comboBox->addItem("Viper4Android >2.7 by Team DeWitt",1);
    connect(ui->pushButton,SIGNAL(clicked()),SLOT(import()));
}

importandroid::~importandroid()
{
    delete ui;
}

void importandroid::import(){
    if(ui->lineEdit->text()==""){
        QMessageBox::warning(this, "Missing Input", "Name is not set",QMessageBox::Ok);
        return;
    }
    QString filename = QFileDialog::getOpenFileName(this,"Import Viper4Android config file (xml)","","*.xml");
    if(filename=="")return;
    auto mode = converter::officialV4A;
    if(ui->comboBox->currentIndex()==1)mode = converter::teamDeWittV4A;
    conversion_result_t response = converter::toLinux(filename,mode);
    if( response.error ) {
        QMessageBox::warning(this, "Syntax Error", response.notices,QMessageBox::Ok);
        LogHelper::writeLog("Converter (a-to-l;mode=" + QString::number(mode) + "): " + response.notices + " (importandroid/syntaxcheck)");
        return;
    }

    string newconfig = response.configuration.toUtf8().constData();
    QString msginfotext = "Successfully converted!\n";
    if(response.notices.length() > 0){
        msginfotext += "\nNotices:\n";
        msginfotext += response.notices;
    }

    QDir d = QFileInfo(mainwin->getACWrapper()->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
    QString path = pathAppend(absolute,"presets");

    QString text = ui->lineEdit->text();
    if (!text.isEmpty()) {

        ofstream cfile(QDir::cleanPath(path + QDir::separator() + text + ".conf").toUtf8().constData());
        if (cfile.is_open())
        {
            cfile << newconfig;
            cfile.close();
        }
        else
            LogHelper::writeLog("Unable to create new file at '" + QDir::cleanPath(path + QDir::separator() + text + ".conf") + "'; cannot import converted android config (importandroid/importer)");
    }

    emit importFinished();
    QMessageBox::information(this,"Import",msginfotext);
    this->close();
}
QString importandroid::pathAppend(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
