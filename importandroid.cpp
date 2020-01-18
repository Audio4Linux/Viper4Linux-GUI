#include "importandroid.h"
#include "ui_importandroid.h"
#include "converter.h"
#include "mainwindow.h"
#include "misc/loghelper.h"

#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
importandroid::importandroid(QString confpath,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importandroid)
{
    m_confpath = confpath;
    ui->setupUi(this);
    ui->comboBox->addItem(tr("Official Viper4Android"),0);
    ui->comboBox->addItem(tr("Viper4Android >2.7 by Team DeWitt"),1);
    connect(ui->pushButton,SIGNAL(clicked()),SLOT(import()));
}

importandroid::~importandroid()
{
    delete ui;
}

void importandroid::import(){
    if(ui->lineEdit->text()==""){
        QMessageBox::warning(this, tr("Missing Input"), tr("Name is not set"),QMessageBox::Ok);
        return;
    }
    QString filename = QFileDialog::getOpenFileName(this,tr("Import Viper4Android config file (xml)"),"","*.xml");
    if(filename=="")return;
    auto mode = converter::officialV4A;
    if(ui->comboBox->currentIndex()==1)mode = converter::teamDeWittV4A;
    conversion_result_t response = converter::toLinux(filename,mode);
    if( response.error ) {
        QMessageBox::warning(this, tr("Syntax Error"), response.notices,QMessageBox::Ok);
        LogHelper::writeLog("Converter (a-to-l;mode=" + QString::number(mode) + "): " + response.notices + " (importandroid/syntaxcheck)");
        return;
    }

    string newconfig = response.configuration.toUtf8().constData();
    QString msginfotext = tr("Successfully converted!\n");
    if(response.notices.length() > 0){
        msginfotext += tr("\nNotices:\n");
        msginfotext += response.notices;
    }

    QDir d = QFileInfo(m_confpath).absoluteDir();
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
    QMessageBox::information(this,tr("Import"),msginfotext);
    this->close();
}
QString importandroid::pathAppend(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
