#include "uploadwizard.h"
#include "ui_uploadwizard.h"
#include <main.h>
#include <fstream>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QProcess>
UploadWizard::UploadWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::UploadWizard)
{
    ui->setupUi(this);
    QDir tempdir("/tmp/vipergui");
    if(tempdir.exists())tempdir.removeRecursively();
    UpdateList();
    connect(this,SIGNAL(currentIdChanged(int)),this,SLOT(PageChanged(int)));
    connect(ui->getkey,SIGNAL(clicked()),this,SLOT(openOAuth()));
    connect(ui->manageOAuth,SIGNAL(clicked()),this,SLOT(openOAuthSettings()));
    connect(ui->viewPR,SIGNAL(clicked()),this,SLOT(openPRs()));
    connect(ui->viewrepo,SIGNAL(clicked()),this,SLOT(openRepo()));
    connect(ui->viewYRepo,SIGNAL(clicked()),this,SLOT(openRepo()));
    //connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
}

UploadWizard::~UploadWizard()
{
    delete ui;
}
void UploadWizard::openOAuth(){
    QDesktopServices::openUrl(QUrl("https://github.com/login/oauth/authorize?client_id=56ea3102eb3b299f115d&scope=repo"));
}
void UploadWizard::openOAuthSettings(){
    QDesktopServices::openUrl(QUrl("https://github.com/settings/connections/applications/56ea3102eb3b299f115d"));
}
void UploadWizard::openPRs(){
    QDesktopServices::openUrl(QUrl("https://github.com/L3vi47h4N/Viper4Linux-Configs/pulls"));
}
void UploadWizard::openRepo(){
    QDesktopServices::openUrl(QUrl("https://github.com/" + forkurl));
}
void UploadWizard::UpdateList(){
    ui->configSelect->clear();
    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString absolute=d.absolutePath();
    QString path = QDir::cleanPath(absolute + QDir::separator() + "presets");
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
    ui->configSelect->addItems(files);
}
void UploadWizard::PageChanged(int id){
    switch(id){
    case 0:
        break;
    case 1:
        break;
    case 2:{
        if(ui->configSelect->currentItem()==nullptr){
            back();
            QMessageBox msgBox;
            msgBox.information(nullptr, "Error", "No config selected");
            return;
        }
        if(ui->author->text()==""){
            back();
            QMessageBox msgBox;
            msgBox.information(nullptr, "Error", "Author not set");
            return;
        }
        if(ui->name->text()==""){
            back();
            QMessageBox msgBox;
            msgBox.information(nullptr, "Error", "Name not set");
            return;
        }
        QString changes = "Name: " + ui->name->text().remove(QRegExp("[^a-zA-Z\\d\\s]")) +
                "\nAuthor: " + ui->author->text().remove(QRegExp("[^a-zA-Z\\d\\s]")) +
                "\nIRS: ";

        QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
        QString path = QDir::cleanPath(d.absolutePath() + QDir::separator() + "presets");
        QString fullpath = QDir(path).filePath(ui->configSelect->selectedItems().first()->text() + ".conf");
        QString contents;
        if(!QFile::exists(fullpath))changes =+ "File does not exist";
        else{

            bool skipIrs = false;
            std::ifstream cFile(fullpath.toUtf8().constData());
            if (cFile.is_open())
            {
                std::string line;
                while(getline(cFile, line)){
                    if(line[0] == '#' || line.empty() || line.empty()) continue;
                    auto delimiterPos = line.find('=');
                    auto name = line.substr(0, delimiterPos);
                    auto value = line.substr(delimiterPos + 1);

                    contents += QString::fromStdString(line) + "\n";
                    if(value=="")continue;

                    if(ui->includeIRS->checkState()!=Qt::Checked && !skipIrs){
                        changes += "No";
                        skipIrs=true;
                    }
                    else if(!skipIrs){
                        if(name == "conv_enable"){
                            if(value == "false"){
                                changes += "No, disabled in config";
                                skipIrs=true;
                            }
                        }
                        if(name == "conv_ir_path"){
                            QString irs_path = "";
                            if(!skipIrs){
                                irs_path= QString::fromStdString(value);
                                if(QFile::exists(irs_path.split("\"")[1])){
                                    changes += "Yes";
                                    irsPath=irs_path.split("\"")[1];
                                }
                                else{
                                    QString var = irs_path.replace("\"$configpath\"",d.absolutePath()).replace("\"","");
                                    if(QFile::exists(var)){
                                        changes += "Yes, using variable";
                                        irsPathVar=var;
                                        irsPath=var;
                                    }
                                    else{
                                        changes += "No, does not exist";
                                    }
                                }
                                skipIrs=true;
                            }
                        }
                    }
                }
                cFile.close();
            }
            else {
                mainwin->writeLog("Unable to read file at '" + fullpath + "' (uploadwizard/configselection)");                ;
                changes += "Unable to read";
            }
            changes += "\n\nSelected Config:\n";
            changes += contents;

        }
        ui->changes->setText(changes);
        break;
    }
    case 3:{
        button(WizardButton::BackButton)->setEnabled(false);
        button(WizardButton::NextButton)->setEnabled(false);
        DoFork();
        button(WizardButton::BackButton)->setEnabled(true);
        button(WizardButton::NextButton)->setEnabled(true);
        break;
    }
    case 4:{
        button(WizardButton::BackButton)->setEnabled(false);
        button(WizardButton::NextButton)->setEnabled(false);
        if(CheckFork(forkurl)==false){
            back();
            QMessageBox::warning(this,"Error","The fork has not been found on your profile, you might have to wait a moment until it is accessible. Please try again in a few moments.");
            mainwin->writeLog("Forked repo not yet found on GitHub, please try again later... (uploadwizard/forkcheck)");
            button(WizardButton::BackButton)->setEnabled(true);
            button(WizardButton::NextButton)->setEnabled(true);
            break;
        }
        if(DoClone(forkurl)==false){
            back();
            QMessageBox::warning(this,"Error","Unable to clone repo, make sure /tmp/vipergui is writable");
            mainwin->writeLog("Unable to clone repo, make sure /tmp/vipergui is writable (uploadwizard/clone)");
            button(WizardButton::BackButton)->setEnabled(true);
            button(WizardButton::NextButton)->setEnabled(true);
            break;
        }
        if(DoChanges(forkurl)==false){
            back();
            button(WizardButton::BackButton)->setEnabled(true);
            button(WizardButton::NextButton)->setEnabled(true);
            break;
        }
        button(WizardButton::BackButton)->setEnabled(false);
        button(WizardButton::NextButton)->setEnabled(true);
        break;
    }
    case 5:{
        button(WizardButton::BackButton)->setEnabled(false);
        button(WizardButton::NextButton)->setEnabled(false);
        if(DoPush(forkurl)==false){
            back();
            button(WizardButton::BackButton)->setEnabled(false);
            button(WizardButton::NextButton)->setEnabled(false);
            break;
        }
        button(WizardButton::BackButton)->setEnabled(false);
        button(WizardButton::NextButton)->setEnabled(true);
        break;
    }
    case 7:{
        button(WizardButton::BackButton)->setEnabled(false);
        button(WizardButton::NextButton)->setEnabled(false);
        button(WizardButton::FinishButton)->setEnabled(false);
        if(ui->pull_title->text()==""){
            back();
            QMessageBox::warning(this,"Error","Title is empty");
            button(WizardButton::BackButton)->setEnabled(false);
            button(WizardButton::NextButton)->setEnabled(true);
            break;
        }
        if(DoPR(forkurl)==false){
            back();
            mainwin->writeLog("Invalid GitHub API response while sending a pull request (uploadwizard/pullrequest)");
            QMessageBox::warning(this,"Error","Invalid GitHub API response, please send a Pull Request manually");
            button(WizardButton::BackButton)->setEnabled(true);
            button(WizardButton::NextButton)->setEnabled(true);
        }
        else{
            button(WizardButton::BackButton)->setEnabled(false);
            button(WizardButton::NextButton)->setEnabled(true);
            button(WizardButton::FinishButton)->setEnabled(true);
        }
        break;
    }
    }
}
bool UploadWizard::DoPR(QString repo){
    QString user = repo.split("/")[0];

    QString out = "[" + getRequest("https://thebone.cf/viperuploader/router.php?token=" + ui->oauth_key->text() +
                                   "&method=post&title=" + QUrl::toPercentEncoding(ui->pull_title->text()) +
                                   "&body=" + QUrl::toPercentEncoding(ui->pull_desc->toPlainText()) +
                                   "&base=master&head=" + user + ":master" +
                                   "&url=repos/L3vi47h4N/Viper4Linux-Configs/pulls") + "]";
    if(out.contains("Error")){
        mainwin->writeLog("GitHub API returned an error (pull request): " + out);
        QMessageBox::information(this,"Error",out);
    }
    else{
        QJsonParseError parseError;
        QJsonDocument document = QJsonDocument::fromJson(out.toLocal8Bit(), &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            mainwin->writeLog("Cannot parse GitHub API response (uploadwizard/pullrequest): " + parseError.errorString());
            return false;
        }
        if (!document.isArray())
        {
            mainwin->writeLog("Malformed GitHub API response (uploadwizard/pullrequest): Document does not contain array");
            return false;
        }
        QJsonArray array = document.array();
        foreach (const QJsonValue & v, array)
        {
            QJsonObject obj = v.toObject();
            QJsonValue url = obj.value("html_url");
        }
        if(out.contains("Bad credentials")){
            QMessageBox::warning(this,"Error","Bad credentials\nInvalid/expired OAuth key");
            mainwin->writeLog("GitHub API (uploadwizard/pullrequest): Bad credentials");
        }
        if(out.contains("\"errors\":[")){
            try {
                QMessageBox::warning(this,"Error",array[0].toObject().value("errors").toArray()[0].toObject().value("message").toString());
                mainwin->writeLog("GitHub API returned an error (uploadwizard/pullrequest): " + array[0].toObject().value("errors").toArray()[0].toObject().value("message").toString());
                button(WizardButton::BackButton)->setEnabled(true);
                button(WizardButton::NextButton)->setEnabled(true);
                back();
            } catch (exception ex) {
                mainwin->writeLog("Unable to parse error information from GitHub API (uploadwizard/pullrequest): " + QString::fromStdString(ex.what()));
               return false;
            }
        }
    }
    return true;
}
bool UploadWizard::DoPush(QString repo){
    QProcess *process = new QProcess(this);
    QString tempdir = "/tmp/vipergui";
    ui->push_state->setText("Committing changes...");
    ui->push_log->append("Committing changes...");

    QString name = ui->name->text().remove(QRegExp("[^a-zA-Z\\d\\s]"));
    QString author = ui->author->text().remove(QRegExp("[^a-zA-Z\\d\\s]"));
    QString base = author.replace(" ","_") + "-" + name.replace(" ","_") + "-audio";

    process->setWorkingDirectory(tempdir + "/Viper4Linux-Configs");
    process->start("git add " + base + ".*");
    process->waitForFinished();
    process->start("git commit -m \"Added " + base + "\"");
    process->waitForFinished();
    QString user = repo.split("/")[0];

    QProcess *push = new QProcess(this);
    ui->push_state->setText("Pushing changes to GitHub...");
    ui->push_log->append("Pushing changes to GitHub...");
    push->setWorkingDirectory(tempdir + "/Viper4Linux-Configs");
    push->start("git push https://" + user + ":" + ui->oauth_key->text() + "@github.com/" + repo + ".git");
    push->waitForFinished();

    ui->push_state->setText("Finished");
    return true;
}
bool UploadWizard::DoClone(QString repo){
    QProcess *process = new QProcess(this);
    QString tempdir = "/tmp/vipergui";
    QDir dir(tempdir);
    dir.mkpath(tempdir);
    ui->change_state->setText("Cloning forked repo...");
    ui->change_log->append("Cloning forked repo... to " + tempdir);
    process->setWorkingDirectory(tempdir);
    process->start("git clone http://github.com/" + repo);
    process->waitForFinished();
    if(QFile::exists(tempdir+"/Viper4Linux-Configs/README.md"))ui->change_log->append("Cloned successfully");
    else{
        mainwin->writeLog("Not successfully cloned, local repo at '" + tempdir+"/Viper4Linux-Configs/" + "' not found (uploadwizard/clone): ");
        ui->change_log->append("Not successfully cloned, local repo not found");
        return false;
    }
    return true;
}
bool UploadWizard::DoChanges(QString repo){
    QProcess *process = new QProcess(this);
    QString tempdir = "/tmp/vipergui";

    ui->change_log->append("Saving Conf...");

    QString name = ui->name->text().remove(QRegExp("[^a-zA-Z\\d\\s]"));
    QString author = ui->author->text().remove(QRegExp("[^a-zA-Z\\d\\s]"));
    QString base = author.replace(" ","_") + "-" + name.replace(" ","_") + "-audio";

    QDir d = QFileInfo(QString::fromStdString(mainwin->getPath())).absoluteDir();
    QString path = QDir::cleanPath(d.absolutePath() + QDir::separator() + "presets");

    //Save Conf
    const QString src = path + "/" + ui->configSelect->currentItem()->text() + ".conf";
    const QString dest = tempdir + "/Viper4Linux-Configs/" + base + ".conf";
    if (QFile::exists(dest))QFile::remove(dest);
    QFile::copy(src,dest);
    mainwin->writeLog("Saving config to " + dest + " (uploadwizard/dochanges)");

    //Save IRS
    if(irsPath!=""){
        ui->change_state->setText("Saving IRS...");
        ui->change_log->append("Saving IRS...");
        const QString srcIrs = irsPath;
        const QString firstdestIrs = tempdir + "/" + base + ".irs";
        const QString destIrs = tempdir + "/Viper4Linux-Configs/" + base + ".irs";
        if (QFile::exists(destIrs))QFile::remove(destIrs);
        QFile::copy(srcIrs,destIrs);

        ui->change_state->setText("Updating IRS-Path...");
        ui->change_log->append("Updating IRS-Path...");
        QFile inputFile(tempdir + "/Viper4Linux-Configs/" + base + ".conf");
        QString config = "";
        if (inputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&inputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if(line.indexOf("conv_ir_path")==0) config += "conv_ir_path=\"$configpath\"\"/" + base + ".irs\"" + "\n";
                else config += line + "\n";
            }
            inputFile.close();
        }
        if (QFile::exists(dest))QFile::remove(dest);
        if (inputFile.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&inputFile);
            stream << config;
            inputFile.close();
        }
    }
    ui->change_state->setText("Finished");
    ui->change_log->append("Finished");
    return true;
}
bool UploadWizard::CheckFork(QString repo){
    QString out = getRequest("https://api.github.com/repos/" + repo);
    if(out.contains("Error: ")&&!out.contains("Not Found")){
        mainwin->writeLog("Error while checking the fork: " + out + " (uploadwizard/forkcheck)");
    }
    else if(out.contains("Not Found")){
        return false;
    }
    return true;
}
void UploadWizard::DoFork(){
    ui->forkstatus->setText("Forking Repo...");
    ui->forklog->setText("Forking Viper4Linux-Config Repo... ");
    QString out = "[" + getRequest("https://thebone.cf/viperuploader/router.php?token=" + ui->oauth_key->text() + "&method=post&url=repos/L3vi47h4N/Viper4Linux-Configs/forks") + "]";
    if(out.contains("Error: ")){
        QMessageBox::information(this,"Error",out);
        mainwin->writeLog("Error while forking: " + out + " (uploadwizard/fork)");
    }
    else{
        QJsonParseError parseError;
        QJsonDocument document = QJsonDocument::fromJson(out.toLocal8Bit(), &parseError);
        if (parseError.error != QJsonParseError::NoError)
        {
            mainwin->writeLog("Cannot parse GitHub API response (uploadwizard/fork): " + parseError.errorString());
        }
        if (!document.isArray())
        {
            mainwin->writeLog("Malformed GitHub API response (uploadwizard/fork): Document does not contain array");
            return;
        }
        QJsonArray array = document.array();
        foreach (const QJsonValue & v, array)
        {
            QJsonObject obj = v.toObject();
            QJsonValue url = obj.value("full_name");
            if(url!="")forkurl = url.toString();
            ui->forklog->append(">" + url.toString());
        }
        if(out.contains("Bad credentials")){
            mainwin->writeLog("GitHub API (uploadwizard/fork): Bad credentials");
            QMessageBox::warning(this,"Error","Bad credentials\nInvalid/expired OAuth key");
            back();
        }
        else ui->forkstatus->setText("Finished");
    }
}
QString UploadWizard::getRequest(QString url_s){
    QUrl url(url_s);
    QNetworkReply* reply;
    QNetworkRequest networkRequest(url);
    QNetworkAccessManager manager;
    QEventLoop connection_loop;

    connect(&manager, SIGNAL( finished( QNetworkReply* ) ), &connection_loop, SLOT( quit() ) );
    reply = manager.get(networkRequest);
    connection_loop.exec();

    if (reply->error()) return "Error: " + reply->errorString();
    reply->deleteLater();
    return reply->readAll();
}
