#include "convolver.h"
#include "ui_convolver.h"
#include "main.h"
#include <QDir>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <QString>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
static bool lockupdate = false;
using namespace std;
Convolver::Convolver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Convolver)
{
    ui->setupUi(this);

    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char result[100];
    strcpy(result,homedir);
    strcat(result,"/IRS");

    ui->path->setText(result);
    QDir path(result);
    QStringList nameFilter("*.ir");
    nameFilter.append("*.irs");
    QStringList files = path.entryList(nameFilter);
    ui->files->addItems(files);

    connect(ui->reload, SIGNAL(clicked()), this, SLOT(reload()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(ui->files, SIGNAL(itemSelectionChanged()), this, SLOT(updateIR()));
    connect(ui->fileSelect, SIGNAL(clicked()), this, SLOT(selectFolder()));
}

Convolver::~Convolver()
{
    delete ui;
}
void Convolver::closeWindow(){
    mainwin->enableConvBtn(true);
    this->close();
}
void Convolver::reload(){
    lockupdate=true;
    QDir path(ui->path->text());
    QStringList nameFilter("*.ir");
    nameFilter.append("*.irs");
    QStringList files = path.entryList(nameFilter);
    ui->files->clear();
    ui->files->addItems(files);
    lockupdate=false;
}
void Convolver::updateIR(){
    if(lockupdate)return; //Clearing Seletion by code != User Interaction
    QString path = QDir(ui->path->text()).filePath(ui->files->selectedItems().first()->text());
    if(QFileInfo::exists(path) && QFileInfo(path).isFile())mainwin->setIRS(path.toUtf8().constData());
}
void Convolver::reject()
{
    mainwin->enableConvBtn(true);
    QDialog::reject();
}
void Convolver::selectFolder(){

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
