#include "importandroid.h"
#include "ui_importandroid.h"
#include "converter.h"
#include "main.h"
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
    string response = converter::read(filename.toUtf8().constData(),mode);
    string::size_type loc = response.find( "Syntax error", 0 );
    if( loc == 0 ) {
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Syntax Error", QString::fromStdString(response),QMessageBox::Ok);
        return;
    }

    char* resp = strtok(&response[0u], "|");
    string newconfig = "";
    string notices = "";
    int count_resp = 0;
    while (resp != nullptr) {
        if (count_resp == 0)newconfig=resp;
        else if (count_resp == 1)notices=resp;
        else break;
        resp = strtok (nullptr, ";");
        count_resp++;
    }

    QString msginfotext = "Successfully converted!\n";
    if(notices!=""){
        msginfotext += "\nNotices:\n";
        msginfotext += QString::fromStdString(notices);
    }

    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
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
        else cerr << "Unable to open file";
    }

    preset->UpdateList();
    QMessageBox::information(this,"Import",msginfotext);
    this->close();
}
QString importandroid::pathAppend(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
