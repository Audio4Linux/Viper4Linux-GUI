#include "autostartmanager.h"
#include "config/container.h"
#include "config/io.h"

#include <QDir>
#include <QFile>

AutostartManager::AutostartManager()
{
}

void AutostartManager::saveDesktopFile(QString path, const QString& exepath,
                                       bool enableAutostartViper, bool delayed){
    ConfigContainer* conf = new ConfigContainer();
    conf->setValue("Exec",QString("%0%1 --tray%2%3")
                   .arg(delayed ? "sleep 5s && " : "")
                   .arg(exepath)
                   .arg(enableAutostartViper ? " --startviper" : "")
                   .arg(delayed ? " &" : ""));
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

bool AutostartManager::inspectDesktopFile(const QString& path, InspectionMode mode){
    ConfigContainer conf;
    conf.setConfigMap(ConfigIO::readFile(path));
    switch(mode){
    case UsesViperAutostart:
        return conf.getString("Exec").contains("--startviper");
    case Delayed:
        return conf.getString("Exec").contains("sleep 5s");
    case Exists:
        return QFile::exists(path);
    }
    return false;
}

QString AutostartManager::getAutostartPath(const QString& filename){
    return QDir::homePath() + "/.config/autostart/" + filename;
}
