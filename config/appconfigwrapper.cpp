#include "appconfigwrapper.h"

#include <QDir>
#include <QTranslator>
#include <QDebug>

#include "misc/stylehelper.h"
#include "container.h"
#include "io.h"

using namespace std;

AppConfigWrapper::AppConfigWrapper(StyleHelper* stylehelper, QString _working_dir_plgmode)
    : m_stylehelper(stylehelper), working_dir_plgmode(_working_dir_plgmode)
{
    appconf = new ConfigContainer();
}
void AppConfigWrapper::saveAppConfig(){
    ConfigIO::writeFile(getAppConfigFilePath(), appconf->getConfigMap());
}
void AppConfigWrapper::loadAppConfig(){
    auto map = ConfigIO::readFile(getAppConfigFilePath());
    appconf->setConfigMap(map);
}

bool AppConfigWrapper::getAutoFx(){
#ifdef VIPER_PLUGINMODE
    return true;
#else
    return appconf->getBool("apply.auto.enable", true, true);
#endif
}

#ifndef VIPER_PLUGINMODE
void AppConfigWrapper::setAutoFx(bool afx){
    appconf->setValue("apply.auto.enable",QVariant(afx));
    saveAppConfig();
}
void AppConfigWrapper::setGFix(bool f){
    appconf->setValue("apply.fixglava",QVariant(f));
    saveAppConfig();
}
bool AppConfigWrapper::getGFix(){
    return appconf->getBool("apply.fixglava");
}
void AppConfigWrapper::setPath(const QString& npath){
    appconf->setValue("io.configpath",QVariant(QString("\"%1\"").arg(npath)));
    saveAppConfig();
}

QString AppConfigWrapper::getPath(){
    QString path = "";
    path = appconf->getString("io.configpath",true);
    path = chopFirstLastChar(path);
    if(path.length() < 2)
        return QString("%1/.config/viper4linux/audio.conf").arg(QDir::homePath());
    return path;
}
#endif
void AppConfigWrapper::setColorpalette(const QString& s){
    appconf->setValue("theme.palette",QVariant(s));
    if(m_stylehelper != nullptr)
        m_stylehelper->SetStyle();
    emit styleChanged();
    saveAppConfig();
}
QString AppConfigWrapper::getColorpalette(){
    return appconf->getString("theme.palette");
}
void AppConfigWrapper::setCustompalette(const QString& s){
    appconf->setValue("theme.palette.custom",QVariant(s));
    if(m_stylehelper != nullptr)
        m_stylehelper->SetStyle();
    emit styleChanged();
    saveAppConfig();
}
QString AppConfigWrapper::getCustompalette(){
    return appconf->getString("theme.palette.custom");
}

void AppConfigWrapper::setWhiteIcons(bool b){
    appconf->setValue("theme.icons.white",QVariant(b));
    if(m_stylehelper != nullptr)
        m_stylehelper->SetStyle();
    emit styleChanged();
    saveAppConfig();
}
bool AppConfigWrapper::getWhiteIcons(){
    return appconf->getBool("theme.icons.white");
}
int AppConfigWrapper::getAutoFxMode(){
#ifdef VIPER_PLUGINMODE
    return 0;
#else
    return appconf->getInt("apply.auto.mode", 1);
#endif
}
#ifndef VIPER_PLUGINMODE
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
bool AppConfigWrapper::getSyncDisabled(){
    return appconf->getBool("driver.propsync", true, false);
}
void AppConfigWrapper::setSyncDisabled(bool b){
    appconf->setValue("driver.propsync",QVariant(b));
    saveAppConfig();
}
#endif
void AppConfigWrapper::setIrsPath(const QString& npath){
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
void AppConfigWrapper::setTheme(const QString& thm){
    appconf->setValue("theme.name",QVariant(thm));
    if(m_stylehelper != nullptr)
        m_stylehelper->SetStyle();
    emit styleChanged();
    saveAppConfig();
}
QString AppConfigWrapper::getTheme(){
    QString name = appconf->getString("theme.name");
    if(name.isEmpty()){
        name = "Fusion";
        appconf->setValue("theme.name",name);
        emit styleChanged();
    }
    return name;
}
void AppConfigWrapper::setLanguage(const QString& thm){
    appconf->setValue("language",QVariant(thm));
    saveAppConfig();
    emit languageChanged();
}
QString AppConfigWrapper::getLanguage(){
    QString name = appconf->getString("language");
    if(name.isEmpty()){
        name = "en";
        appconf->setValue("language", name);
        saveAppConfig();
        emit languageChanged();
    }
    return name;
}
QString AppConfigWrapper::getIrsPath(){
    QString irs_path = chopFirstLastChar(appconf->getString("convolver.default.irspath",false));
    if(irs_path.length() < 2)
        return QString("%1/IRS").arg(QDir::homePath());
    return irs_path;
}
#ifndef VIPER_PLUGINMODE
int AppConfigWrapper::getTrayMode(){
    return appconf->getInt("session.tray.mode");
}
void AppConfigWrapper::setTrayMode(int mode){
    appconf->setValue("session.tray.mode",QVariant(mode));
    saveAppConfig();
}
void AppConfigWrapper::setTrayContextMenu(const QString& ctx){
    appconf->setValue("session.tray.contextmenu",QVariant(ctx));
    saveAppConfig();
}
QString AppConfigWrapper::getTrayContextMenu(){
    QString name = appconf->getString("session.tray.contextmenu");
    return name;
}
void AppConfigWrapper::setSpectrumEnable(bool b){
    appconf->setValue("visualizer.spectrum.enable",QVariant(b));
    emit spectrumChanged();
    saveAppConfig();
}
bool AppConfigWrapper::getSpetrumEnable(){
    return appconf->getBool("visualizer.spectrum.enable");
}
int AppConfigWrapper::getSpectrumBands(){
    return appconf->getInt("visualizer.spectrum.bands");
}
void AppConfigWrapper::setSpectrumBands(int number){
    appconf->setValue("visualizer.spectrum.bands",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
int AppConfigWrapper::getSpectrumMinFreq(){
    return appconf->getInt("visualizer.spectrum.frequency.min");
}
void AppConfigWrapper::setSpectrumMinFreq(int number){
    appconf->setValue("visualizer.spectrum.frequency.min",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
int AppConfigWrapper::getSpectrumMaxFreq(){
    return appconf->getInt("visualizer.spectrum.frequency.max");
}
void AppConfigWrapper::setSpectrumMaxFreq(int number){
    appconf->setValue("visualizer.spectrum.frequency.max",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
void AppConfigWrapper::setSpectrumGrid(bool b){
    appconf->setValue("visualizer.spectrum.grid",QVariant(b));
    emit spectrumChanged();
    saveAppConfig();
}
bool AppConfigWrapper::getSpetrumGrid(){
    return appconf->getBool("visualizer.spectrum.grid");
}
int AppConfigWrapper::getSpectrumTheme(){
    return appconf->getInt("visualizer.spectrum.theme", 1);
}
void AppConfigWrapper::setSpectrumTheme(int number){
    appconf->setValue("visualizer.spectrum.theme",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
int AppConfigWrapper::getSpectrumShape(){
    return appconf->getInt("visualizer.spectrum.shape");
}
void AppConfigWrapper::setSpectrumShape(int number){
    appconf->setValue("visualizer.spectrum.shape",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
void AppConfigWrapper::setSpectrumInput(const QString& npath){
    appconf->setValue("visualizer.spectrum.device",QVariant(npath));
    emit spectrumReloadRequired();
    saveAppConfig();
}
QString AppConfigWrapper::getSpectrumInput(){
    return appconf->getString("visualizer.spectrum.device",true,"viper.monitor");
}
int AppConfigWrapper::getSpectrumRefresh(){
    return appconf->getInt("visualizer.spectrum.interval");
}
void AppConfigWrapper::setSpectrumRefresh(int number){
    appconf->setValue("visualizer.spectrum.interval",QVariant(number));
    emit spectrumReloadRequired();
    saveAppConfig();
}
float AppConfigWrapper::getSpectrumMultiplier(){
    return appconf->getFloat("visualizer.spectrum.multiplier");
}
void AppConfigWrapper::setSpectrumMultiplier(float number){
    appconf->setValue("visualizer.spectrum.multiplier",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
#endif
void AppConfigWrapper::setEqualizerPermanentHandles(bool b){
    appconf->setValue("equalizer.handle.permanent",QVariant(b));
    emit eqChanged();
    saveAppConfig();
}
bool AppConfigWrapper::getEqualizerPermanentHandles(){
    return appconf->getBool("equalizer.handle.permanent");
}
#ifndef VIPER_PLUGINMODE
void AppConfigWrapper::setIntroShown(bool b){
    appconf->setValue("app.firstlaunch",QVariant(b));
    saveAppConfig();
}
bool AppConfigWrapper::getIntroShown(){
    return appconf->getBool("app.firstlaunch");
}
#endif

//--------
QString AppConfigWrapper::getAppConfigFilePath(){
#ifdef VIPER_PLUGINMODE
    QString str = QString("%1/ui.conf").arg(working_dir_plgmode);
    return str;
#else
    return QString("%1/.config/viper4linux/ui.2.conf").arg(QDir::homePath());
#endif
}
