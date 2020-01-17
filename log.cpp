#include "log.h"
#include "ui_log.h"
#include "main.h"

#include <QFile>

log::log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log)
{
    ui->setupUi(this);
    connect(ui->reload, SIGNAL(clicked()), this, SLOT(updateLog()));
    connect(ui->select, SIGNAL(currentIndexChanged(int)),this,SLOT(updateLog()));
    updateLog();
}

log::~log()
{
    delete ui;
}
void log::reject()
{
    mainwin->EnableLogButton(true);
    QDialog::reject();
}
void log::updateLog(){
    ui->viperlog->clear();
    QString path;
    if(ui->select->currentText()=="GST Plugin") path = "/tmp/viper4linux/viper.log";
    else path = "/tmp/viper4linux/ui.log";

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        ui->viperlog->append(tr("<Failed to open '%1'>").arg(path));
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        ui->viperlog->append(line);
    }

    file.close();
}
