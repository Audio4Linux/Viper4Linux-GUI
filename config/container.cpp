#include "container.h"

#include <QDebug>
#include <utility> 
ConfigContainer::ConfigContainer()
= default;
void ConfigContainer::setValue(const QString& key,QVariant value){
    map[key] = std::move(value);
}
QVariant ConfigContainer::getVariant(const QString& key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        return QVariant();
    }
    return map.find(key).value();
}
QString ConfigContainer::getString(const QString& key,bool setToDefaultIfMissing, QString alternative){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        if(setToDefaultIfMissing)map[key] = alternative;
        return alternative;
    }
    return getVariant(key).toString();
}
int ConfigContainer::getInt(const QString& key, int alternative){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        map[key] = alternative;
        return alternative;
    }
    return getVariant(key).toInt();
}
float ConfigContainer::getFloat(const QString& key){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        map[key] = 0.0F;
        return 0.0F;
    }
    return getVariant(key).toFloat();
}
bool ConfigContainer::getBool(const QString& key, bool setToDefaultIfMissing, bool alternative){
    if(!map.contains(key)){
        qWarning().noquote().nospace() << "[W] Requested key '" << key << "' not found";
        if(setToDefaultIfMissing)map[key] = alternative;
        return alternative;
    }
    return getVariant(key).toBool();
}
QVariantMap ConfigContainer::getConfigMap(){
    return map;
}
void ConfigContainer::setConfigMap(const QVariantMap& newmap){
    map.clear();
    for(const auto& e : newmap.keys())
      map[e] = newmap.value(e);
}
