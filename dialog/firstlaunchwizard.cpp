#include "firstlaunchwizard.h"
#include "ui_firstlaunchwizard.h"
#include "misc/common.h"
#include "misc/autostartmanager.h"
#include "misc/GstRegistryHelper.h"
#include "viper_window.h"

#include <QTimer>
#include <QEasingCurve>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QMessageBox>

#include <config/container.h>
#include <config/io.h>

FirstLaunchWizard::FirstLaunchWizard(AppConfigWrapper* _appconf, ViperWindow* mainwin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FirstLaunchWizard)
{
    ui->setupUi(this);

    appconf = _appconf;

    ui->stackedWidget->setCurrentIndex(0);

    QTimer::singleShot(1000, [&]{
        ui->p1_icon->startAnimation();
    });
    ui->p2_icon->startAnimation();
    ui->pm_icon->startAnimation();
    ui->pl_icon->startAnimation();
    ui->p3_icon->startAnimation();
    ui->p4_icon->startAnimation();

    ui->stackedWidget->setAnimation(QEasingCurve::Type::OutCirc);

    connect(ui->p1_next,&QPushButton::clicked,[&]{
        if(!GstRegistryHelper::IsPluginInstalled()){
            ui->stackedWidget->slideInIdx(1);
        }
        else if(!GstRegistryHelper::HasDBusSupport()){
            ui->stackedWidget->slideInIdx(2);
        }
        else{
            ui->stackedWidget->slideInIdx(3);
        }
    });
    connect(ui->p2_next,&QPushButton::clicked,[&]{
        ui->stackedWidget->slideInIdx(4);
    });
    connect(ui->p3_next,&QPushButton::clicked,[&]{
        ui->stackedWidget->slideInIdx(5);
    });
    connect(ui->p4_next,&QPushButton::clicked,[&]{
        emit wizardFinished();
    });

    auto openTelegram = []{
        QDesktopServices::openUrl(QUrl("https://t.me/joinchat/FTKC2A2bolHkFAyO-fuPjw"));
    };
    connect(ui->p4_telegram,&QPushButton::clicked, openTelegram);

    connect(ui->pm_telegram,&QPushButton::clicked, openTelegram);
    connect(ui->pm_readme,&QPushButton::clicked, []{
        QDesktopServices::openUrl(QUrl("https://github.com/Audio4Linux/Viper4Linux"));
    });
    connect(ui->pm_next,&QPushButton::clicked, [this]{
        gst_update_registry();
        if(GstRegistryHelper::IsPluginInstalled()){
            if(!GstRegistryHelper::HasDBusSupport())
                ui->stackedWidget->slideInIdx(2);
            else
                ui->stackedWidget->slideInIdx(3);
        }
        else{
            QMessageBox::warning(this, tr("Error"), tr("Still no luck. No GStreamer plugin named 'viperfx' has been found!"));
        }
    });
    connect(ui->pl_next,&QPushButton::clicked, [this]{
        ui->stackedWidget->slideInIdx(3);
    });
    connect(ui->pl_help,&QPushButton::clicked, []{
        QDesktopServices::openUrl(QUrl("https://gist.github.com/ThePBone/93da516635cdbc01759c10d2e5abb7ea"));
    });

    auto deviceUpdated = [this](){
        if(lockslot) return;
        QString absolute =
                QFileInfo(appconf->getPath()).absoluteDir().absolutePath();
        QString devices(pathAppend(absolute,"devices.conf"));
        if(ui->p2_dev_mode_auto->isChecked()){
            QFile(devices).remove();
        }else{
            if(ui->p2_dev_select->currentData() == "---")
                return;

            ConfigContainer* devconf = new ConfigContainer();
            devconf->setConfigMap(ConfigIO::readFile(devices));
            devconf->setValue("location",ui->p2_dev_select->currentData());
            ConfigIO::writeFile(devices,devconf->getConfigMap());
        }
    };
    refreshDevices();

    ui->p3_systray_disable->setChecked(!appconf->getTrayMode());
    ui->p3_systray_enable->setChecked(appconf->getTrayMode());
    ui->p3_systray_icon_box->setEnabled(appconf->getTrayMode());

    QString autostart_path = AutostartManager::getAutostartPath("viper-gui.desktop");
    bool autostart_enabled = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::Exists);
    bool autostartviper_enabled = AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::UsesViperAutostart);

    ui->p3_systray_minOnBoot->setChecked(autostart_enabled);
    ui->p3_systray_autostartViper->setEnabled(autostart_enabled);
    ui->p3_systray_autostartViper->setChecked(autostartviper_enabled);

    auto systray_radio = [this,mainwin]{
        if(lockslot)return;
        int mode = 0;
        if(ui->p3_systray_disable->isChecked())mode=0;
        else if(ui->p3_systray_enable->isChecked())mode=1;
        appconf->setTrayMode(mode);
        mainwin->setTrayVisible(mode);
        ui->p3_systray_icon_box->setEnabled(mode);
    };

    connect(ui->p3_systray_disable,&QRadioButton::clicked,this,systray_radio);
    connect(ui->p3_systray_enable,&QRadioButton::clicked,this,systray_radio);

    auto systray_autostart_radio = [this,autostart_path,mainwin]{
        if(ui->p3_systray_minOnBoot->isChecked()){
            AutostartManager::saveDesktopFile(autostart_path,mainwin->GetExecutablePath(),
                                              ui->p3_systray_autostartViper->isChecked(),
                                              AutostartManager::inspectDesktopFile(autostart_path,AutostartManager::Delayed));
        }
        else QFile(autostart_path).remove();
        ui->p3_systray_autostartViper->setEnabled(ui->p3_systray_minOnBoot->isChecked());
    };

    connect(ui->p3_systray_minOnBoot,&QPushButton::clicked,this,systray_autostart_radio);
    connect(ui->p3_systray_autostartViper,&QPushButton::clicked,this,systray_autostart_radio);

    connect(ui->p2_dev_mode_auto,&QRadioButton::clicked,this,deviceUpdated);
    connect(ui->p2_dev_mode_manual,&QRadioButton::clicked,this,deviceUpdated);
    connect(ui->p2_dev_select,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, deviceUpdated);

}

FirstLaunchWizard::~FirstLaunchWizard()
{
    delete ui;
}

void FirstLaunchWizard::refreshDevices()
{
    lockslot = true;
    ui->p2_dev_select->clear();
    QString absolute =
            QFileInfo(appconf->getPath()).absoluteDir().absolutePath();
    QFile devices(pathAppend(absolute,"devices.conf"));
    bool devmode_auto = !devices.exists();
    ui->p2_dev_mode_auto->setChecked(devmode_auto);
    ui->p2_dev_mode_manual->setChecked(!devmode_auto);

    QProcess process;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LC_ALL", "C");
    process.setProcessEnvironment(env);
    process.start("sh", QStringList()<<"-c"<<"pactl list sinks | grep \'Name: \' -A1");
    process.waitForFinished(500);

    ConfigContainer* devconf = new ConfigContainer();
    devconf->setConfigMap(ConfigIO::readFile(pathAppend(absolute,"devices.conf")));
    QString out = process.readAllStandardOutput();
    ui->p2_dev_select->addItem("...","---");
    for(auto item : out.split("Name:")){
        item.prepend("Name:");
        QRegularExpression re(R"((?<=(Name:)\s)(?<name>.+)[\s\S]+(?<=(Description:)\s)(?<desc>.+))");
        QRegularExpressionMatch match = re.match(item, 0, QRegularExpression::PartialPreferCompleteMatch);
        if(match.hasMatch()){
            ui->p2_dev_select->addItem(QString("%1 (%2)").arg(match.captured("desc")).arg(match.captured("name")),
                                       match.captured("name"));
        }
    }
    QString dev_location = devconf->getString("location");
    if(dev_location.isEmpty())
        ui->p2_dev_select->setCurrentIndex(0);
    else{
        bool notFound = true;
        for(int i = 0; i < ui->p2_dev_select->count(); i++){
            if(ui->p2_dev_select->itemData(i) ==
                    dev_location){
                notFound = false;
                ui->p2_dev_select->setCurrentIndex(i);
                break;
            }
        }
        if(notFound){
            QString name = QString(tr("Unknown (%1)")).arg(dev_location);
            ui->p2_dev_select->addItem(name,dev_location);
            ui->p2_dev_select->setCurrentText(name);
        }
    }
    lockslot = false;
}
