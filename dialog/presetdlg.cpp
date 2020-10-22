#include "presetdlg.h"
#include "ui_preset.h"
#include "viper_window.h"
#include "misc/converter.h"
#include "androidimporterdlg.h"
#include "misc/loghelper.h"
#include "misc/common.h"
#include "config/appconfigwrapper.h"

#include "3rdparty/WAF/Animation/Animation.h"

#include <QDir>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDesktopServices>


PresetDlg::PresetDlg(ViperWindow* mainwin,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preset)
{
    ui->setupUi(this);
    m_mainwin = mainwin;
    appconf = mainwin->getACWrapper();
    manager = new QNetworkAccessManager(this);
    UpdateList();
    connect(ui->add,SIGNAL(clicked()),SLOT(add()));
    connect(ui->load,SIGNAL(clicked()),SLOT(load()));
    connect(ui->githubRepo,SIGNAL(clicked()),SLOT(visitGithub()));
    connect(ui->remove,SIGNAL(clicked()),SLOT(remove()));
    connect(ui->download,SIGNAL(clicked()),SLOT(download()));
    connect(ui->presetName,SIGNAL(textChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(ui->files, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(ui->files, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(repoIndexChanged()));
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(indexDownloaded(QNetworkReply*)));

    connect(ui->tabWidget, &QTabWidget::currentChanged, [=](int i){
        if(i == 1 && ui->repoindex->count() <= 1){
            QUrl url("https://api.github.com/repos/noahbliss/Viper4Linux-Configs/contents/");
            request.setUrl(url);
            manager->get(request);
        }
    });

    //Placeholder Repoindex
    QFont font;
    font.setItalic(true);
    font.setPointSize(10);
    QListWidgetItem* placeholder = new QListWidgetItem;
    placeholder->setFont(font);
    placeholder->setText(tr("No data received"));
    placeholder->setFlags(placeholder->flags() & ~Qt::ItemIsEnabled);
    ui->repoindex->addItem(placeholder);

    QMenu *menu = new QMenu();
    menu->addAction(tr("Android Profile"), this,SLOT(importAndroid()));
    menu->addAction(tr("Linux Configuration"), this,SLOT(importLinux()));
    ui->importBtn->setMenu(menu);

    QMenu *menuEx = new QMenu();

    QMenu *exportSubA = menuEx->addMenu(tr("Android Profile"));
    exportSubA->addAction(tr("V4A <2.6"),this,[this]{exportAndroid(converter::officialV4A);});
    exportSubA->addAction(tr("V4A >2.7"),this,[this]{exportAndroid(converter::teamDeWittV4A);});

    menuEx->addAction(tr("Linux Configuration"), this,SLOT(exportLinux()));
    ui->exportBtn->setMenu(menuEx);
}
PresetDlg::~PresetDlg()
{
    delete ui;
}
void PresetDlg::repoIndexChanged(){
    if(ui->files->currentItem() == nullptr)return;
    ui->presetName->setText(ui->files->currentItem()->text());
}
void PresetDlg::reloadRepo(){
    ui->repoindex->clear();
    QUrl url("https://api.github.com/repos/noahbliss/Viper4Linux-Configs/contents/");
    request.setUrl(url);
    manager->get(request);
}

void PresetDlg::indexDownloaded(QNetworkReply* reply){
    if (reply->error()) {
        LogHelper::writeLog("Cannot download repo index: " + reply->errorString() + " (presets/repoindex)");
        return;
    }
    QByteArray answer = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(answer, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        LogHelper::writeLog("Cannot parse GitHub API response (presets/repoindex): " + parseError.errorString());
        return;
    }
    if (!document.isArray())
    {
        LogHelper::writeLog("Malformed GitHub API response (presets/repoindex): Document does not contain array");
        return;
    }
    QJsonArray array = document.array();
    irs.clear();
    ui->repoindex->clear();
    foreach (const QJsonValue & v, array)
    {
        QJsonObject obj = v.toObject();
        QJsonValue name = obj.value("name");
        QJsonValue url = obj.value("download_url");
        QFileInfo namefi(name.toString());
        if(name.isUndefined()||url.isUndefined())continue;
        if(name.toString().at(0)==QChar('.'))continue; //Skip hidden files
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
QString PresetDlg::optimizeName(const QString& s){
    QRegExp rx("(\\-)"); //Split Confname by -
    QStringList query = s.split(rx);
    if(query.count()!=3) return ""; //Filter configs which ignore the naming convention
    QString author = query[0];
    if(author=="000")author = "noahbliss";
    QString description = query[1].replace("_"," ");
    return description + " by " + author;
}
void PresetDlg::visitGithub(){
    QDesktopServices::openUrl(QUrl("https://github.com/noahbliss/Viper4Linux-Configs"));
}
void PresetDlg::reject()
{
    QDialog::reject();
}
void PresetDlg::UpdateList(){
    ui->files->clear();
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");

    QDir dir(path);
    if (!dir.exists())
        dir.mkpath(".");

    QStringList nameFilter("*.conf");
    QStringList files = dir.entryList(nameFilter);
    if(files.count()<1){
        QFont font;
        font.setItalic(true);
        font.setPointSize(11);

        QListWidgetItem* placeholder = new QListWidgetItem;
        placeholder->setFont(font);
        placeholder->setText(tr("No presets saved"));
        placeholder->setFlags(placeholder->flags() & ~Qt::ItemIsEnabled);
        ui->files->addItem(placeholder);
        return;
    }

    for(int i = 0; i < files.count(); i++){ //Strip extensions
        QFileInfo fi(files[i]);
        files[i] = fi.completeBaseName();
    }

    ui->files->addItems(files);
}
QString PresetDlg::toCamelCase(const QString& s)
{
    QStringList parts = s.split(' ', QString::SkipEmptyParts);
    for (int i = 0; i < parts.size(); ++i)
        parts[i].replace(0, 1, parts[i][0].toUpper());

    return parts.join(" ");
}

void PresetDlg::add(){
    if(ui->presetName->text()==""){
        QMessageBox::warning(this, tr("Error"), tr("Preset Name is empty"),QMessageBox::Ok);
        return;
    }
    m_mainwin->ApplyConfig(false);
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");
    m_mainwin->SavePresetFile(path + "/" + ui->presetName->text() + ".conf");
    ui->presetName->text() = "";
    UpdateList();
    emit presetChanged();
}
void PresetDlg::importAndroid(){
#ifdef VIPER_PLUGINMODE
    AndroidImporterDlg* ia = new AndroidImporterDlg(m_mainwin->getLegacyPath(),this);
#else
    AndroidImporterDlg* ia = new AndroidImporterDlg(m_mainwin->getACWrapper()->getPath(),this);
#endif
    QWidget* host = new QWidget(this);
    host->setProperty("menu", false);
    QVBoxLayout* hostLayout = new QVBoxLayout(host);
    hostLayout->addWidget(ia);
    host->hide();
    WAF::Animation::sideSlideIn(host, WAF::BottomSide);

    connect(ia,&AndroidImporterDlg::importFinished,this,[host,this](){
        WAF::Animation::sideSlideOut(host, WAF::BottomSide);
        UpdateList();
        emit presetChanged();
    });
}
void PresetDlg::exportAndroid(converter::configtype cmode){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::warning(this, tr("Error"), tr("Nothing selected"),QMessageBox::Ok);
        return;
    }
    QString newname;
    if(cmode==converter::officialV4A)newname = "com.vipercn.viper4android_v2.headset.xml";
    else newname = "headset.xml";
    QString filename = QFileDialog::getSaveFileName(this,tr("Save XML"),newname,"*.xml");
    if(filename=="")return;
    QFileInfo fi(filename);
    QString ext = fi.suffix();
    if(ext!="xml")filename.append(".xml");

#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text() + ".conf");
    QFile file (fullpath);
    if(!QFile::exists(fullpath)){
        QMessageBox::warning(this, tr("Error"), tr("Selected File doesn't exist"),QMessageBox::Ok);
        UpdateList();
        emit presetChanged();
        return;
    }

    const QString& src = fullpath;
    const QString dest = filename;

    if (QFile::exists(dest))QFile::remove(dest);
    QFile qFile(dest);
    if (qFile.open(QIODevice::WriteOnly)) {
        conversion_result_t re = converter::toAndroid(src,cmode);
        QTextStream out(&qFile); out << re.configuration;
        qFile.close();
    }
    LogHelper::writeLog("Exporting to "+filename + " (presets/androidexport)");
}
void PresetDlg::importLinux(){
    QString filename = QFileDialog::getOpenFileName(this,tr("Load custom audio.conf"),"","*.conf");
    if(filename=="")return;

    QFileInfo fileInfo(filename);
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");

    const QString& src = filename;
    const QString dest = path + "/" + fileInfo.fileName();
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    LogHelper::writeLog("Importing from "+filename+ " (presets/linuximport)");
}
void PresetDlg::exportLinux(){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::warning(this, tr("Error"), tr("Nothing selected"),QMessageBox::Ok);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this,tr("Save audio.conf"),"","*.conf");
    if(filename=="")return;
    QFileInfo fi(filename);
    QString ext = fi.suffix();
    if(ext!="conf")filename.append(".conf");

    QFileInfo fileInfo(filename);
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text() + ".conf");
    QFile file (fullpath);
    if(!QFile::exists(fullpath)){
        QMessageBox::warning(this, tr("Error"), tr("Selected File doesn't exist"),QMessageBox::Ok);
        UpdateList();
        emit presetChanged();
        return;
    }

    const QString& src = fullpath;
    const QString dest = filename;
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    LogHelper::writeLog("Exporting to "+filename+ " (presets/linuxexport)");
}
void PresetDlg::remove(){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::warning(this, tr("Error"), tr("Nothing selected"),QMessageBox::Ok);
        return;
    }
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text() + ".conf");
    QFile file (fullpath);
    if(!QFile::exists(fullpath)){
        QMessageBox::warning(this, tr("Error"), tr("Selected File doesn't exist"),QMessageBox::Ok);
        UpdateList();
        emit presetChanged();
        return;
    }
    file.remove();
    LogHelper::writeLog("Removed "+fullpath+ " (presets/remove)");
    UpdateList();
    emit presetChanged();
}
void PresetDlg::load(){
    if(ui->files->selectedItems().length() == 0){
        QMessageBox::warning(this, tr("Error"), tr("Nothing selected"),QMessageBox::Ok);
        return;
    }
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(ui->files->selectedItems().first()->text() + ".conf");
    if(!QFile::exists(fullpath)){
        QMessageBox::warning(this, tr("Error"), tr("Selected File doesn't exist"),QMessageBox::Ok);
        UpdateList();
        emit presetChanged();
        return;
    }
    m_mainwin->LoadPresetFile(fullpath);
}
void PresetDlg::nameChanged(const QString& name){
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    if(QFile::exists(pathAppend(absolute,"presets") + "/" + name + ".conf"))
        ui->add->setText(tr("Overwrite"));
    else ui->add->setText(tr("Save"));
}
void PresetDlg::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->files->mapToGlobal(pos);
    QMenu menu;
    QAction* action_rename = menu.addAction(tr("Rename"));
    QAction* action_del = menu.addAction(tr("Delete"));
    QListWidgetItem* pointedItem = ui->files->itemAt(pos);
    if(!pointedItem)return;
#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
#else
    QString absolute = m_mainwin->getLegacyPath();
#endif
    QString path = pathAppend(absolute,"presets");
    QString fullpath = QDir(path).filePath(pointedItem->text() + ".conf");

    QAction* selectedAction;
    if(pointedItem){
        selectedAction = menu.exec(globalPos);
        if(selectedAction) {
            if(selectedAction == action_rename) {
                bool ok;
                QString text = QInputDialog::getText(this, tr("Rename"),
                                                     tr("New Name"), QLineEdit::Normal,
                                                     pointedItem->text(), &ok);
                if (ok && !text.isEmpty())QFile::rename(fullpath,QDir(path).filePath(text + ".conf"));
                UpdateList();
                emit presetChanged();
            }
            if(selectedAction == action_del) {
                if(!QFile::exists(fullpath)){
                    QMessageBox::warning(this, tr("Error"), tr("Selected File doesn't exist"),QMessageBox::Ok);
                    UpdateList();
                    emit presetChanged();
                    return;
                }
                QFile file (fullpath);
                file.remove();
                LogHelper::writeLog("Removed "+fullpath);
                UpdateList();
                emit presetChanged();

            }
        }
    }
    menu.exec(globalPos);
}
void PresetDlg::performIRSDownload(QNetworkReply* reply){
    if(reply->error())
    {
        ui->status->setText(tr("Error: %1").arg(reply->errorString()));
    }
    else
    {
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!=200){
            ui->status->setText(tr("Error: %1").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()) + " - " + reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
            return;
        }else{
            ui->status->setText(tr("HTTP-Response %1").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()));
        }
        QFileInfo fileInfo(ui->repoindex->currentItem()->data(Qt::UserRole).toString());
        QString fileName=fileInfo.baseName();
        QStringList irsMatches = irs.filter(fileName); //Query for IRS Name
        QFileInfo fileInfo2(irsMatches.first());
        QString name=fileInfo2.baseName().replace("%20","");

#ifndef VIPER_PLUGINMODE
        QDir d = QFileInfo(appconf->getPath()).absoluteDir();
        QString absolute=d.absolutePath();
#else
        QString absolute = m_mainwin->getLegacyPath();
#endif
        QFile *file = new QFile(absolute+"/"+name+".irs");
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;
        ui->status->setText(tr("Download finished"));
    }
    reply->deleteLater();
    UpdateList();
    emit presetChanged();
}
void PresetDlg::performDownload(QNetworkReply* reply){
    if(reply->error())
    {
        ui->status->setText(tr("Error: %1").arg(reply->errorString()));
    }
    else
    {
        //--CONF Download
        if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!=200){
            ui->status->setText(tr("Error: %1").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()) + " - " + reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());
            return;
        }else{
            ui->status->setText(tr("HTTP-Response %1").arg(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()));
        }

        QString name = ui->repoindex->currentItem()->text();
        QString url = ui->repoindex->currentItem()->data(Qt::UserRole).toString();

#ifndef VIPER_PLUGINMODE
        QDir d = QFileInfo(appconf->getPath()).absoluteDir();
        QString absolute=d.absolutePath();
#else
        QString absolute = m_mainwin->getLegacyPath();
#endif
        QFile *file = new QFile(absolute+"/presets/"+name+".conf");
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;

        //--IRS Download
        ui->status->setText(tr("Downloading IRS..."));
        QFileInfo fileInfo(url);
        QString fileName=fileInfo.baseName();
        QStringList irsMatches = irs.filter(fileName); //Query for IRS
        if(irsMatches.count()==0){
            ui->status->setText(tr("Download finished"));
        }else{
            QString match = irsMatches.first();
            QNetworkAccessManager* dlmanagerIRS = new QNetworkAccessManager(this);
            connect(dlmanagerIRS,SIGNAL(finished(QNetworkReply*)),this,SLOT(performIRSDownload(QNetworkReply*)));
            dlmanagerIRS->get(QNetworkRequest(QUrl(match)));
        }
    }
    reply->deleteLater();
    UpdateList();
    emit presetChanged();
}
void PresetDlg::download(){
    if(ui->repoindex->currentItem()==nullptr)return;
    ui->status->setText(tr("Downloading Config..."));
    QNetworkAccessManager* dlmanager = new QNetworkAccessManager(this);
    connect(dlmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(performDownload(QNetworkReply*)));
    QString url = ui->repoindex->currentItem()->data(Qt::UserRole).toString();
    dlmanager->get(QNetworkRequest(QUrl(url)));
}
