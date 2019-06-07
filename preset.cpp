#include "preset.h"
#include "ui_preset.h"
#include "main.h"
#include <QDir>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <QString>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>
#include <iostream>
#include <sstream>
#include <fstream>
#include "converter.h"
#include "importandroid.h"
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
    connect(ui->presetName,SIGNAL(textChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(ui->files, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
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
    if(files.count()<1)return;

    for(int i = 0; i < files.count(); i++){ //Strip extensions
        QFileInfo fi(files[i]);
        files[i] = fi.completeBaseName();
    }
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
    ui->presetName->text() = "";
    UpdateList();
}

void Preset::import(){
    auto ia = new importandroid(this);
    ia->setFixedSize(ia->geometry().width(),ia->geometry().height());
    ia->show();
}

void Preset::remove(){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Nothing selected",QMessageBox::Ok);
        return;
    }
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString path = pathAppend(d.absolutePath(),"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text() + ".conf");
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
    QString path = pathAppend(d.absolutePath(),"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text() + ".conf");
    if(!QFile::exists(fullpath)){
        QMessageBox::StandardButton msg;
        msg = QMessageBox::warning(this, "Error", "Selected File doesn't exist",QMessageBox::Ok);
        UpdateList();
        return;
    }
    mainwin->LoadPresetFile(fullpath);
}
void Preset::nameChanged(QString name){
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString path = pathAppend(d.absolutePath(),"presets");
    if(QFile::exists(path + "/" + name + ".conf"))ui->add->setText("Overwrite");
    else ui->add->setText("Save");
}

void Preset::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->files->mapToGlobal(pos);
    QMenu menu;
    QAction* action_rename = menu.addAction("Rename");
    QAction* action_del = menu.addAction("Delete");
    QListWidgetItem* pointedItem = ui->files->itemAt(pos);
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString path = pathAppend(d.absolutePath(),"presets");
    QString fullpath = QDir(path).filePath(pointedItem->text() + ".conf");


    QAction* selectedAction;
    if(pointedItem){
        selectedAction = menu.exec(globalPos);
        if(selectedAction) {
            if(selectedAction == action_rename) {
                bool ok;
                QString text = QInputDialog::getText(this, "Rename",
                                                     "New Name", QLineEdit::Normal,
                                                     pointedItem->text(), &ok);
                if (ok && !text.isEmpty())QFile::rename(fullpath,QDir(path).filePath(text + ".conf"));
                UpdateList();
            }
            if(selectedAction == action_del) {
                if(!QFile::exists(fullpath)){
                    QMessageBox::StandardButton msg;
                    msg = QMessageBox::warning(this, "Error", "Selected File doesn't exist",QMessageBox::Ok);
                    UpdateList();
                    return;
                }
                QFile file (fullpath);
                file.remove();
                cout << "Removed " << fullpath.toUtf8().constData() << endl;
                UpdateList();
            }
        }
    }
    menu.exec(globalPos);
}
