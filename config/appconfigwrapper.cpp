#include "appconfigwrapper.h"

#include <QDir>
#include <QDebug>

using namespace std;

AppConfigWrapper::AppConfigWrapper(StyleHelper* stylehelper)
{
    appconf = new ConfigContainer();
    m_stylehelper = stylehelper;
}
void AppConfigWrapper::saveAppConfig(){
    auto file = QString("%1/.config/viper4linux/ui.conf").arg(QDir::homePath());
    ConfigIO::writeFile(file,appconf->getConfigMap());
}
void AppConfigWrapper::loadAppConfig(){
    auto map = ConfigIO::readFile(QString("%1/.config/viper4linux/ui.conf").arg(QDir::homePath()),false);
    appconf->setConfigMap(map);
}

bool AppConfigWrapper::getAutoFx(){
    return appconf->getBool("autoapply");
}
void AppConfigWrapper::setAutoFx(bool afx){
    appconf->setValue("autoapply",QVariant(afx));
    saveAppConfig();
}
bool AppConfigWrapper::getMuteOnRestart(){
    return appconf->getBool("muteOnRestart");
}
void AppConfigWrapper::setMuteOnRestart(bool on){
    appconf->setValue("muteOnRestart",QVariant(on));
    saveAppConfig();
}
void AppConfigWrapper::setGFix(bool f){
    appconf->setValue("glavafix",QVariant(f));
    saveAppConfig();
}
bool AppConfigWrapper::getGFix(){
    return appconf->getBool("glavafix");
}
void AppConfigWrapper::setPath(QString npath){
    appconf->setValue("configpath",QVariant(QString("\"%1\"").arg(npath)));
    saveAppConfig();
}
QString AppConfigWrapper::getPath(){
    QString path = chopFirstLastChar(appconf->getString("configpath"));
    if(path.length() < 1)
        return QString("%1/.config/viper4linux/audio.conf").arg(QDir::homePath());
    return path;
}
void AppConfigWrapper::setStylesheet(QString s){
    appconf->setValue("stylesheet",QVariant(s));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getStylesheet(){
    return appconf->getString("stylesheet");
}
int AppConfigWrapper::getThememode(){
    return appconf->getInt("thememode");
}
void AppConfigWrapper::setThememode(int mode){
    appconf->setValue("thememode",QVariant(mode));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
void AppConfigWrapper::setColorpalette(QString s){
    appconf->setValue("custompalette",QVariant(s));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getColorpalette(){
    return appconf->getString("colorpalette");
}
void AppConfigWrapper::setCustompalette(QString s){
    appconf->setValue("colorpalette",QVariant(s));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getCustompalette(){
    return appconf->getString("custompalette");
}
void AppConfigWrapper::setWhiteIcons(bool b){
    appconf->setValue("customwhiteicons",QVariant(b));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
bool AppConfigWrapper::getWhiteIcons(){
    return appconf->getBool("customwhiteicons");
}
int AppConfigWrapper::getAutoFxMode(){
    return appconf->getInt("autoapplymode");
}
void AppConfigWrapper::setAutoFxMode(int mode){
    appconf->setValue("autoapplymode",QVariant(mode));
    saveAppConfig();
}
void AppConfigWrapper::setIrsPath(QString npath){
    appconf->setValue("irspath",QVariant(QString("\"%1\"").arg(npath)));
    saveAppConfig();
}
int AppConfigWrapper::getConv_DefTab(){
    return appconf->getInt("convolver_defaulttab");
}
void AppConfigWrapper::setConv_DefTab(int mode){
    appconf->setValue("convolver_defaulttab",QVariant(mode));
    saveAppConfig();
}
void AppConfigWrapper::setTheme(QString thm){
    appconf->setValue("theme",QVariant(thm));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getTheme(){
    return appconf->getString("theme");
}
QString AppConfigWrapper::getIrsPath(){
    QString irs_path = chopFirstLastChar(appconf->getString("irspath"));
    if(irs_path.length() < 1)
        return QString("%1/IRS").arg(QDir::homePath());
    return irs_path;
}
//--------
QString AppConfigWrapper::getAppConfigFilePath(){
    return QString("%1/.config/viper4linux/ui.conf").arg(QDir::homePath());
}
