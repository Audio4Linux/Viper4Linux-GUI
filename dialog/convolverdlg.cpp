#include "convolverdlg.h"
#include "ui_convolver.h"
#include "misc/loghelper.h"
#include "viper_window.h"

#include <QDir>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

static bool lockupdate = false;
using namespace std;
ConvolverDlg::ConvolverDlg(ViperWindow* mainwin,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Convolver)
{
    ui->setupUi(this);
    m_mainwin = mainwin;
    AppConfigWrapper* appconf = mainwin->getACWrapper();

    ui->path->setText(appconf->getIrsPath());

#ifndef VIPER_PLUGINMODE
    QDir d = QFileInfo(appconf->getPath()).absoluteDir();
    QString absolute=d.absolutePath();
    configpath = QDir::cleanPath(absolute + QDir::separator());

    QDir irs_fav(QDir::cleanPath(configpath + QDir::separator() + "irs_favorites"));
#else
    QDir irs_fav(QDir::cleanPath(mainwin->getLegacyPath() + QDir::separator() + "irs_favorites"));
#endif
    if (!irs_fav.exists())
        irs_fav.mkpath(".");

    reload();
    reloadFav();

    connect(ui->reload, SIGNAL(clicked()), this, SLOT(reload()));
    connect(ui->fav_add, SIGNAL(clicked()), this, SLOT(addFav()));
    connect(ui->fav_remove, SIGNAL(clicked()), this, SLOT(removeFav()));
    connect(ui->fav_rename, SIGNAL(clicked()), this, SLOT(renameFav()));
    connect(ui->files, SIGNAL(itemSelectionChanged()), this, SLOT(updateIR()));
    connect(ui->favorites, SIGNAL(itemSelectionChanged()), this, SLOT(updateIR_Fav()));
    connect(ui->fileSelect, SIGNAL(clicked()), this, SLOT(selectFolder()));

    connect(ui->closeA,&QPushButton::clicked,this,&ConvolverDlg::closePressed);
    connect(ui->closeB,&QPushButton::clicked,this,&ConvolverDlg::closePressed);

    ui->tabWidget->setCurrentIndex(appconf->getConv_DefTab());
}

ConvolverDlg::~ConvolverDlg()
{
    delete ui;
}
void ConvolverDlg::closeWindow(){
    this->close();
}
void ConvolverDlg::reload(){
    lockupdate=true;
    QDir path(ui->path->text());
    QStringList nameFilter("*.wav");
    nameFilter.append("*.irs");
    QStringList files = path.entryList(nameFilter);
    ui->files->clear();
    if(files.empty()){
        QFont font;
        font.setItalic(true);
        font.setPointSize(11);

        QListWidgetItem* placeholder = new QListWidgetItem;
        placeholder->setFont(font);
        placeholder->setText(tr("No IRS files found"));
        placeholder->setFlags(placeholder->flags() & ~Qt::ItemIsEnabled);
        ui->files->addItem(placeholder);
    }
    else ui->files->addItems(files);
    lockupdate=false;
}
void ConvolverDlg::reloadFav(){
    lockupdate=true;
    QDir path(QDir::cleanPath(configpath + QDir::separator() + "irs_favorites"));
    QStringList nameFilter("*.wav");
    nameFilter.append("*.irs");
    QStringList files = path.entryList(nameFilter);
    ui->favorites->clear();
    if(files.empty()){
        QFont font;
        font.setItalic(true);
        font.setPointSize(11);

        QListWidgetItem* placeholder = new QListWidgetItem;
        placeholder->setFont(font);
        placeholder->setText(tr("Nothing here yet..."));
        placeholder->setFlags(placeholder->flags() & ~Qt::ItemIsEnabled);
        ui->favorites->addItem(placeholder);
        QListWidgetItem* placeholder2 = new QListWidgetItem;
        //placeholder2->setFont(font);
        placeholder2->setText(tr("Add some IRS files in the 'filesystem' tab"));
        placeholder2->setFlags(placeholder2->flags() & ~Qt::ItemIsEnabled);
        ui->favorites->addItem(placeholder2);
    }
    else ui->favorites->addItems(files);
    lockupdate=false;
}
void ConvolverDlg::renameFav(){
    if(ui->favorites->selectedItems().count()<1)return;
    bool ok;
    QString text = QInputDialog::getText(this, tr("Rename"),
                                         tr("New Name"), QLineEdit::Normal,
                                         ui->favorites->selectedItems().first()->text(), &ok);
    QString fullpath = QDir(QDir::cleanPath(configpath + QDir::separator() + "irs_favorites")).filePath(ui->favorites->selectedItems().first()->text());;
    QString dest = QDir::cleanPath(configpath + QDir::separator() + "irs_favorites");
    if (ok && !text.isEmpty())QFile::rename(fullpath,QDir(dest).filePath(text));
    reloadFav();
}
void ConvolverDlg::removeFav(){
    if(ui->favorites->selectedItems().count()<1)return;
    QString fullpath = QDir(QDir::cleanPath(configpath + QDir::separator() + "irs_favorites")).filePath(ui->favorites->selectedItems().first()->text());;
    if(!QFile::exists(fullpath)){
        QMessageBox::warning(this, tr("Error"), tr("Selected File doesn't exist"),QMessageBox::Ok);
        reloadFav();
        return;
    }
    QFile file (fullpath);
    file.remove();
    LogHelper::writeLog("Removed "+fullpath+" from favorites (convolver/remove)");
    reloadFav();
}
void ConvolverDlg::addFav(){
    if(ui->files->selectedItems().count()<1)return; //Clearing Selection by code != User Interaction

    const QString src = QDir(ui->path->text()).filePath(ui->files->selectedItems().first()->text());
    const QString& dest = QDir(QDir::cleanPath(configpath + QDir::separator() + "irs_favorites")).filePath(ui->files->selectedItems().first()->text());

    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    LogHelper::writeLog("Adding " + src + " to favorites (convolver/add)");
    reloadFav();
}

void ConvolverDlg::updateIR(){
    if(lockupdate || ui->files->selectedItems().count()<1)return; //Clearing Selection by code != User Interaction
    QString path = QDir(ui->path->text()).filePath(ui->files->selectedItems().first()->text());
    if(QFileInfo::exists(path) && QFileInfo(path).isFile())m_mainwin->SetIRS(path,true);
}
void ConvolverDlg::updateIR_Fav(){
    if(lockupdate || ui->favorites->selectedItems().count()<1)return; //Clearing Selection by code != User Interaction
    QString path = QDir(QDir::cleanPath(configpath + QDir::separator() + "irs_favorites")).filePath(ui->favorites->selectedItems().first()->text());
    if(QFileInfo::exists(path) && QFileInfo(path).isFile())m_mainwin->SetIRS(path,true);
}
void ConvolverDlg::reject()
{
    QDialog::reject();
}
void ConvolverDlg::selectFolder(){

    QFileDialog *fd = new QFileDialog;
    fd->setFileMode(QFileDialog::Directory);
    fd->setOption(QFileDialog::ShowDirsOnly);
    fd->setViewMode(QFileDialog::Detail);
    QString result = fd->getExistingDirectory();
    if (result!="")
    {
        ui->path->setText(result);
        reload();
    }
}
