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
    auto file = QString("%1/.config/viper4linux/ui.2.conf").arg(QDir::homePath());
    ConfigIO::writeFile(file,appconf->getConfigMap());
}
void AppConfigWrapper::loadAppConfig(){
    auto map = ConfigIO::readFile(QString("%1/.config/viper4linux/ui.2.conf").arg(QDir::homePath()),false);
    appconf->setConfigMap(map);
}

bool AppConfigWrapper::getAutoFx(){
    return appconf->getBool("apply.auto.enable");
}
void AppConfigWrapper::setAutoFx(bool afx){
    appconf->setValue("apply.auto.enable",QVariant(afx));
    saveAppConfig();
}
bool AppConfigWrapper::getMuteOnRestart(){
    return appconf->getBool("apply.mutedreload");
}
void AppConfigWrapper::setMuteOnRestart(bool on){
    appconf->setValue("apply.mutedreload",QVariant(on));
    saveAppConfig();
}
void AppConfigWrapper::setGFix(bool f){
    appconf->setValue("apply.fixglava",QVariant(f));
    saveAppConfig();
}
bool AppConfigWrapper::getGFix(){
    return appconf->getBool("apply.fixglava");
}
void AppConfigWrapper::setPath(QString npath){
    appconf->setValue("io.configpath",QVariant(QString("\"%1\"").arg(npath)));
    saveAppConfig();
}
QString AppConfigWrapper::getPath(){
    QString path = chopFirstLastChar(appconf->getString("io.configpath",true));
    if(path.length() < 2)
        return QString("%1/.config/viper4linux/audio.conf").arg(QDir::homePath());
    return path;
}
void AppConfigWrapper::setStylesheet(QString s){
    appconf->setValue("theme.stylesheet",QVariant(s));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getStylesheet(){
    return appconf->getString("theme.stylesheet");
}
int AppConfigWrapper::getThememode(){
    return appconf->getInt("theme.mode");
}
void AppConfigWrapper::setThememode(int mode){
    appconf->setValue("theme.mode",QVariant(mode));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
void AppConfigWrapper::setColorpalette(QString s){
    appconf->setValue("theme.palette",QVariant(s));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getColorpalette(){
    return appconf->getString("theme.palette");
}
void AppConfigWrapper::setCustompalette(QString s){
    appconf->setValue("theme.palette.custom",QVariant(s));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getCustompalette(){
    return appconf->getString("theme.palette.custom");
}
void AppConfigWrapper::setWhiteIcons(bool b){
    appconf->setValue("theme.icons.white",QVariant(b));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
bool AppConfigWrapper::getWhiteIcons(){
    return appconf->getBool("theme.icons.white");
}
int AppConfigWrapper::getAutoFxMode(){
    return appconf->getInt("apply.auto.mode");
}
void AppConfigWrapper::setAutoFxMode(int mode){
    appconf->setValue("apply.auto.mode",QVariant(mode));
    saveAppConfig();
}
ReloadMethod AppConfigWrapper::getReloadMethod(){
    return (ReloadMethod)appconf->getInt("apply.method");
}
void AppConfigWrapper::setReloadMethod(ReloadMethod mode){
    appconf->setValue("apply.method",QVariant((uint)mode));
    saveAppConfig();
}
void AppConfigWrapper::setIrsPath(QString npath){
    appconf->setValue("convolver.default.irspath",QVariant(QString("\"%1\"").arg(npath)));
    saveAppConfig();
}
int AppConfigWrapper::getConv_DefTab(){
    return appconf->getInt("convolver.default.tab");
}
void AppConfigWrapper::setConv_DefTab(int mode){
    appconf->setValue("convolver.default.tab",QVariant(mode));
    saveAppConfig();
}
void AppConfigWrapper::setTheme(QString thm){
    appconf->setValue("theme.name",QVariant(thm));
    m_stylehelper->SetStyle();
    saveAppConfig();
}
QString AppConfigWrapper::getTheme(){
    return appconf->getString("theme.name");
}
QString AppConfigWrapper::getIrsPath(){
    QString irs_path = chopFirstLastChar(appconf->getString("convolver.default.irspath",false));
    if(irs_path.length() < 2)
        return QString("%1/IRS").arg(QDir::homePath());
    return irs_path;
}
int AppConfigWrapper::getTrayMode(){
    return appconf->getInt("session.tray.mode");
}
void AppConfigWrapper::setTrayMode(int mode){
    appconf->setValue("session.tray.mode",QVariant(mode));
    saveAppConfig();
}

//--------
QString AppConfigWrapper::getAppConfigFilePath(){
    return QString("%1/.config/viper4linux/ui.2.conf").arg(QDir::homePath());
}
