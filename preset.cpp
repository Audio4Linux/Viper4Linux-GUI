#include "preset.h"
#include "ui_preset.h"
#include "main.h"
#include <QDir>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <QString>
#include <QCloseEvent>
#include <QString>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <iostream>
#include <sstream>
#include <fstream>
#include "converter.h"
Preset::Preset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preset)
{
    ui->setupUi(this);
    UpdateList();
    connect(ui->add,SIGNAL(clicked()),SLOT(add()));
    connect(ui->load,SIGNAL(clicked()),SLOT(load()));
    connect(ui->remove,SIGNAL(clicked()),SLOT(remove()));
    connect(ui->importBtn,SIGNAL(clicked()),SLOT(import()));
}

Preset::~Preset()
{
    delete ui;
}
void Preset::reject()
{
    mainwin->enablePresetBtn(true);
    QDialog::reject();
}
void Preset::UpdateList(){
    ui->files->clear();
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString absolute=d.absolutePath();
    QString path = pathAppend(absolute,"presets");

    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(".");

    QStringList nameFilter("*.conf");
    QStringList files = dir.entryList(nameFilter);
    ui->files->addItems(files);
}
QString Preset::pathAppend(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}
void Preset::add(){
    if(ui->presetName->text()==""){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Preset Name is empty",QMessageBox::Ok);
        return;
    }
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString absolute=d.absolutePath();
    QString path = pathAppend(absolute,"presets");
    mainwin->SavePresetFile(path + "/" + ui->presetName->text() + ".conf");

    UpdateList();
}

void Preset::import(){
    QString filename = QFileDialog::getOpenFileName(this,"Import Viper4Android config file (xml)","","*.xml");
    if(filename=="")return;

    string response = converter::read(filename.toUtf8().constData());
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

    bool ok;
    QString text = QInputDialog::getText(0, "Import",
                                         msginfotext + "\nPreset Name:", QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {

        ofstream cfile(QDir::cleanPath(path + QDir::separator() + text + ".conf").toUtf8().constData());
        if (cfile.is_open())
        {
            cfile << newconfig;
            cfile.close();
        }
        else cerr << "Unable to open file";
    }

    UpdateList();
}

void Preset::remove(){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Nothing selected",QMessageBox::Ok);
        return;
    }
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString absolute=d.absolutePath();
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text());
    QFile file (fullpath);
    if(!QFile::exists(fullpath)){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Selected File doesn't exist",QMessageBox::Ok);
        UpdateList();
        return;
    }
    file.remove();
    cout << "Removed " << fullpath.toUtf8().constData() << endl;
    UpdateList();

}
void Preset::load(){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Nothing selected",QMessageBox::Ok);
        return;
    }
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString absolute=d.absolutePath();
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text());
    if(!QFile::exists(fullpath)){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Selected File doesn't exist",QMessageBox::Ok);
        UpdateList();
        return;
    }
    mainwin->LoadPresetFile(fullpath);
}
