#include "logdlg.h"
#include "ui_log.h"

#include <QTextStream>
#include <QFile>

LogDlg::LogDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log)
{
    ui->setupUi(this);
    connect(ui->reload, SIGNAL(clicked()), this, SLOT(updateLog()));
    connect(ui->select, SIGNAL(currentIndexChanged(int)),this,SLOT(updateLog()));

#ifdef VIPER_PLUGINMODE
    ui->select->hide();
#endif

    updateLog();
}

LogDlg::~LogDlg()
{
    delete ui;
}
void LogDlg::reject()
{
    QDialog::reject();
}
void LogDlg::updateLog(){
    ui->viperlog->clear();
    QString path;

#ifndef VIPER_PLUGINMODE
    if(ui->select->currentIndex() == 0) path = "/tmp/viper4linux/viper.log";
    else path = "/tmp/viper4linux/ui.log";
#else
    path = "/tmp/gsteffectmanager/viper_ui.log";
#endif

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
