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
    auto map = ConfigIO::readFile(QString("%1/.config/viper4linux/ui.2.conf").arg(QDir::homePath()));
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
void AppConfigWrapper::setStylesheet(const QString& s){
    appconf->setValue("theme.stylesheet",QVariant(s));
    m_stylehelper->SetStyle();
    emit styleChanged();
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
    emit styleChanged();
    saveAppConfig();
}
void AppConfigWrapper::setColorpalette(const QString& s){
    appconf->setValue("theme.palette",QVariant(s));
    m_stylehelper->SetStyle();
    emit styleChanged();
    saveAppConfig();
}
QString AppConfigWrapper::getColorpalette(){
    return appconf->getString("theme.palette");
}
void AppConfigWrapper::setCustompalette(const QString& s){
    appconf->setValue("theme.palette.custom",QVariant(s));
    m_stylehelper->SetStyle();
    emit styleChanged();
    saveAppConfig();
}
QString AppConfigWrapper::getCustompalette(){
    return appconf->getString("theme.palette.custom");
}
void AppConfigWrapper::setWhiteIcons(bool b){
    appconf->setValue("theme.icons.white",QVariant(b));
    m_stylehelper->SetStyle();
    emit styleChanged();
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
    return appconf->getInt("visualizer.spectrum.theme");
}
void AppConfigWrapper::setSpectrumTheme(int number){
    appconf->setValue("visualizer.spectrum.theme",QVariant(number));
    emit spectrumChanged();
    saveAppConfig();
}
void AppConfigWrapper::setSpectrumInput(const QString& npath){
    appconf->setValue("visualizer.spectrum.device",QVariant(npath));
    emit spectrumReloadRequired();
    saveAppConfig();
}
QString AppConfigWrapper::getSpectrumInput(){
    QString in = appconf->getString("visualizer.spectrum.device",true);
    if(in.length() < 1)
        return QString("viper.monitor");
    return in;
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
void AppConfigWrapper::setEqualizerPermanentHandles(bool b){
    appconf->setValue("equalizer.handle.permanent",QVariant(b));
    emit eqChanged();
    saveAppConfig();
}
bool AppConfigWrapper::getEqualizerPermanentHandles(){
    return appconf->getBool("equalizer.handle.permanent");
}
void AppConfigWrapper::setIntroShown(bool b){
    appconf->setValue("app.firstlaunch",QVariant(b));
    saveAppConfig();
}
bool AppConfigWrapper::getIntroShown(){
    return appconf->getBool("app.firstlaunch");
}
//--------
QString AppConfigWrapper::getAppConfigFilePath(){
    return QString("%1/.config/viper4linux/ui.2.conf").arg(QDir::homePath());
}
