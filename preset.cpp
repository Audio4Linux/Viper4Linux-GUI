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
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRegExp>
#include <QJsonObject>
#include <QVector>
#include <QTimer>
#include <QDesktopServices>
#include <uploadwizard.h>
Preset::Preset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preset)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager(this);
    UpdateList();
    connect(ui->add,SIGNAL(clicked()),SLOT(add()));
    connect(ui->load,SIGNAL(clicked()),SLOT(load()));
    connect(ui->upload,SIGNAL(clicked()),SLOT(upload()));
    connect(ui->githubRepo,SIGNAL(clicked()),SLOT(visitGithub()));
    connect(ui->remove,SIGNAL(clicked()),SLOT(remove()));
    connect(ui->download,SIGNAL(clicked()),SLOT(download()));
    connect(ui->importBtn,SIGNAL(clicked()),SLOT(import()));
    connect(ui->presetName,SIGNAL(textChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(ui->files, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(indexDownloaded(QNetworkReply*)));

    QUrl url("https://api.github.com/repos/L3vi47h4N/Viper4Linux-Configs/contents/");
    request.setUrl(url);
    manager->get(request);

}
Preset::~Preset()
{
    delete ui;
}
void Preset::upload(){
    UploadWizard* upl = new UploadWizard();
    upl->exec();
}
void Preset::indexDownloaded(QNetworkReply* reply){
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    QByteArray answer = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(answer, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Parse error: " << parseError.errorString();
        return;
    }
    if (!document.isArray())
    {
        qDebug() << "Document does not contain array";
        return;
    }
    QJsonArray array = document.array();
    irs.clear();
    foreach (const QJsonValue & v, array)
    {
        QJsonObject obj = v.toObject();
        QJsonValue name = obj.value("name");
        QJsonValue url = obj.value("download_url");
        QFileInfo namefi(name.toString());
        if(name.isUndefined()||url.isUndefined())continue;
        if(name.toString().at(0)==".")continue; //Skip hidden files
        if(namefi.completeSuffix()!="conf"&&namefi.completeSuffix()!="irs")continue; //Skip unrelated files
        if(namefi.completeSuffix()=="conf"){
            ui->repoindex->addItem(optimizeName(namefi.baseName()));
            QVariant url_data(url);
            ui->repoindex->item(ui->repoindex->count() - 1)->setData(Qt::UserRole,url_data);
        }
        else if(namefi.completeSuffix()=="irs"){
            irs.append(url.toString());
        }
    }

}
QString Preset::optimizeName(QString s){
    QRegExp rx("(\\-)"); //Split Confname by -
    QStringList query = s.split(rx);
    if(query.count()!=3) return ""; //Filter configs which ignore the naming convention
    QString author = query[0];
    if(author=="000")author = "L3vi47h4N";
    QString description = query[1].replace("_"," ");
    return toCamelCase(description) + " by " + author;
}
void Preset::visitGithub(){
    QDesktopServices::openUrl(QUrl("https://github.com/L3vi47h4N/Viper4Linux-Configs"));
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
QString Preset::toCamelCase(const QString& s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
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
    if(!pointedItem)return;
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
void Preset::performIRSDownload(QNetworkReply* reply){
    if(reply->error())
    {
        ui->status->setText("Error: " + reply->errorString());
    }
    else
    {
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!=200){
            ui->status->setText("Error " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() + " - " + reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
            return;
        }else{
            ui->status->setText("HTTP-Response " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
        }
        QFileInfo fileInfo(ui->repoindex->currentItem()->data(Qt::UserRole).toString());
        QString fileName=fileInfo.baseName();
        QStringList irsMatches = irs.filter(fileName); //Query for IRS Name
        QFileInfo fileInfo2(irsMatches.first());
        QString name=fileInfo2.baseName().replace("%20","");
        QFileInfo filepath(QString::fromStdString(mainwin->getPath()));
        QFile *file = new QFile(filepath.absolutePath()+"/"+name+".irs");
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;
        ui->status->setText("Download finished");
     }
    reply->deleteLater();
    UpdateList();
}
void Preset::performDownload(QNetworkReply* reply){
    if(reply->error())
    {
        ui->status->setText("Error: " + reply->errorString());
    }
    else
    {
        //--CONF Download
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!=200){
            ui->status->setText("Error " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() + " - " + reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
            return;
        }else{
            ui->status->setText("HTTP-Response " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString());
        }

        QString name = ui->repoindex->currentItem()->text();
        QString url = ui->repoindex->currentItem()->data(Qt::UserRole).toString();
        QFileInfo filepath(QString::fromStdString(mainwin->getPath()));
        QFile *file = new QFile(filepath.absolutePath()+"/presets/"+name+".conf");
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;

        //--IRS Download
        ui->status->setText("Downloading IRS...");
        QFileInfo fileInfo(url);
        QString fileName=fileInfo.baseName();
        QStringList irsMatches = irs.filter(fileName); //Query for IRS
        if(irsMatches.count()==0){
            ui->status->setText("Download finished");
        }else{
            QString match = irsMatches.first();
            QNetworkAccessManager* dlmanagerIRS = new QNetworkAccessManager(this);
            connect(dlmanagerIRS,SIGNAL(finished(QNetworkReply*)),this,SLOT(performIRSDownload(QNetworkReply*)));
            dlmanagerIRS->get(QNetworkRequest(QUrl(match)));
        }
    }
    reply->deleteLater();
    UpdateList();
}
void Preset::download(){
    if(ui->repoindex->currentItem()==nullptr)return;
    ui->status->setText("Downloading Config...");
    QNetworkAccessManager* dlmanager = new QNetworkAccessManager(this);
    connect(dlmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(performDownload(QNetworkReply*)));
    QString url = ui->repoindex->currentItem()->data(Qt::UserRole).toString();
    dlmanager->get(QNetworkRequest(QUrl(url)));
}
