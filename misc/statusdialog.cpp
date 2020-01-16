#include "statusdialog.h"
#include "ui_statusdialog.h"

#include <QDebug>
#include <QMessageBox>

StatusDialog::StatusDialog(DBusProxy* dbus, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatusDialog)
{
    ui->setupUi(this);
    if(!dbus->isValid()){
        QMessageBox::critical(this,"DBus connection error","Unable to connect to DBus interface.\n"
                                                           "Please make sure viper is running and you are using the lastest version of gst-plugin-viperfx");
        this->close();
        return;
    }
    ui->gst_plugin_ver->setText(dbus->GetVersion());

    uint versiondata_core = dbus->GetDriverStatus(DBusProxy::PARAM_GET_DRIVER_VERSION);
    QString versionhex_core = QString("%1").arg(versiondata_core, 0, 16);
    QString versionfinal_core("");
    int count = 0;
    for(auto c:versionhex_core){
        if((count%2)!=0)versionfinal_core += '.';
        else versionfinal_core += c;
        count++;
    }
    ui->core_ver->setText(versionfinal_core);

    int proc = dbus->GetDriverStatus(DBusProxy::PARAM_GET_DRVCANWORK);
    ui->proc->setText(proc ? "Processing" : "Not processing");
    int enab = dbus->GetDriverStatus(DBusProxy::PARAM_GET_ENABLED);
    ui->enabled->setText(enab ? "Enabled" : "Disabled");

    int samplerate = dbus->GetDriverStatus(DBusProxy::PARAM_GET_SAMPLINGRATE);
    ui->samplerate->setText(QString::number(samplerate));

}

StatusDialog::~StatusDialog()
{
    delete ui;
}
