#include "autostartmanager.h"
#include "config/container.h"
#include "config/io.h"

#include <QDir>
#include <QFile>

AutostartManager::AutostartManager()
{
}

void AutostartManager::saveDesktopFile(QString path, QString exepath,
                                       bool enableAutostartViper){
    ConfigContainer* conf = new ConfigContainer();
    conf->setValue("Exec",QString("%1 --tray%2").arg(exepath).arg(enableAutostartViper ? " --startviper" : ""));
    conf->setValue("Name","Viper4Linux2-GUI Systray");
    conf->setValue("StartupNotify",false);
    conf->setValue("Terminal",false);
    conf->setValue("Type","Application");
    conf->setValue("Version","1.0");
    conf->setValue("X-GNOME-Autostart-Delay",10);
    conf->setValue("X-GNOME-Autostart-enabled",true);
    conf->setValue("X-KDE-autostart-after","panel");
    conf->setValue("X-KDE-autostart-phase",2);
    conf->setValue("X-MATE-Autostart-Delay",10);
    ConfigIO::writeFile(path,conf->getConfigMap(),"[Desktop Entry]");
}

bool AutostartManager::inspectDesktopFile(QString path, InspectionMode mode){
    ConfigContainer conf;
    switch(mode){
    case UsesViperAutostart:
        conf.setConfigMap(ConfigIO::readFile(path,false));
        return conf.getString("Exec").contains("--startviper");
        break;
    case Exists:
        return QFile::exists(path);
        break;
    }
}

QString AutostartManager::getAutostartPath(QString filename){
    return QDir::homePath() + "/.config/autostart/" + filename;
}
