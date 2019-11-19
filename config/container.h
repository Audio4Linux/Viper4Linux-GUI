#ifndef CONFIGCONTAINER_H
#define CONFIGCONTAINER_H

#include <QObject>
#include <QVariant>

class ConfigContainer
{
public:
    ConfigContainer();
    void setValue(QString key,QVariant value);
    QVariant getVariant(QString key);
    QString getString(QString key);
    int getInt(QString key);
    float getFloat(QString key);
    bool getBool(QString key);

    QVariantMap getConfigMap();
    void setConfigMap(QVariantMap newmap);

private:
    QVariantMap map = QVariantMap();

};

#endif // CONFIGCONTAINER_H
