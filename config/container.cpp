#include "container.h"
#include <QDebug>

ConfigContainer::ConfigContainer()
{
}
void ConfigContainer::setValue(QString key,QVariant value){
    map[key] = value;
}
QVariant ConfigContainer::getVariant(QString key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        return QVariant();
    }
    return map.find(key).value();
}
QString ConfigContainer::getString(QString key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        return "";
    }
    return getVariant(key).toString();
}
int ConfigContainer::getInt(QString key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        return 0;
    }
    return getVariant(key).toInt();
}
float ConfigContainer::getFloat(QString key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        return 0.0f;
    }
    return getVariant(key).toFloat();
}
bool ConfigContainer::getBool(QString key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        return false;
    }
    return getVariant(key).toBool();
}
QVariantMap ConfigContainer::getConfigMap(){
    return map;
}
void ConfigContainer::setConfigMap(QVariantMap newmap){
    map.clear();
    for(auto e : newmap.keys())
      map[e] = newmap.value(e);
}
