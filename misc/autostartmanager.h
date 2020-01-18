#ifndef AUTOSTARTMANAGER_H
#define AUTOSTARTMANAGER_H

#include <QObject>

class AutostartManager
{
public:
    typedef enum InspectionMode{
        UsesViperAutostart,
        Exists
    }InspectionMode;
    AutostartManager();
    static void saveDesktopFile(QString path, QString exepath, bool enableAutostartViper);
    static bool inspectDesktopFile(QString path, InspectionMode mode);
    static QString getAutostartPath(QString filename);
};

#endif // AUTOSTARTMANAGER_H
