#include "convolver.h"
#include "ui_convolver.h"
#include "main.h"
#include <QDir>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <QString>

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
}

Convolver::~Convolver()
{
    delete ui;
}
void Convolver::closeWindow(){
    this->close();
}
void Convolver::reload(){
    QDir path(ui->path->text());
    QStringList nameFilter("*.ir");
    nameFilter.append("*.irs");
    QStringList files = path.entryList(nameFilter);
    ui->files->clear();
    ui->files->addItems(files);
}
void Convolver::updateIR(){
    QString path = QDir(ui->path->text()).filePath(ui->files->selectedItems().first()->text());
    mainwin->setIRS(path.toUtf8().constData());
}
