#ifndef CONFIGIO_H
#define CONFIGIO_H

#include <QObject>
#include <QVariantMap>

class ConfigIO
{
public:
    static QString writeString(QVariantMap map);
    static void writeFile(QString path,QVariantMap map,QString prefix = "");
    static QVariantMap readFile(QString path,bool allowWarning = true);
};

#endif // CONFIGIO_H
