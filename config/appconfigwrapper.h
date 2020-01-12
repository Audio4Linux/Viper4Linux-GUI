#ifndef APPCONFIGWRAPPER_H
#define APPCONFIGWRAPPER_H

#include <QObject>
#include "misc/stylehelper.h"
#include "container.h"
#include "io.h"

using namespace std;

class AppConfigWrapper
{
public:
    AppConfigWrapper(StyleHelper* stylehelper);
    void saveAppConfig();
    void loadAppConfig();
    static QString getAppConfigFilePath();

    void setConv_DefTab(int mode);
    int getConv_DefTab();
    void setIrsPath(QString npath);
    QString getIrsPath();
    void setTheme(QString);
    QString getTheme();
    void setAutoFxMode(int);
    int getAutoFxMode();
    void setWhiteIcons(bool b);
    bool getWhiteIcons();
    void setCustompalette(QString s);
    QString getCustompalette();
    void setColorpalette(QString);
    QString getColorpalette();
    void setAutoFx(bool autofx);
    bool getAutoFx();
    int getThememode();
    void setThememode(int mode);
    bool getGFix();
    void setGFix(bool);
    bool getMuteOnRestart();
    void setMuteOnRestart(bool on);
    QString getPath();
    void setPath(QString npath);
    QString getStylesheet();
    void setStylesheet(QString);
private:
    ConfigContainer* appconf;
    StyleHelper*     m_stylehelper;
};

inline QString chopFirstLastChar(QString i){
    if(i.size() > 2){
        i.remove(0,1);
        i.chop(1);
    }
    return i;
}

#endif // APPCONFIGWRAPPER_H
