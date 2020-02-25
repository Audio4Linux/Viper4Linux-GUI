/*
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  ThePBone <tim.schneeberger(at)outlook.de> (c) 2020
 */
#ifndef APPCONFIGWRAPPER_H
#define APPCONFIGWRAPPER_H

#include <QObject>
#include "misc/stylehelper.h"
#include "container.h"
#include "io.h"

using namespace std;

typedef enum class ReloadMethod{
    DIRECT_DBUS,
    USE_VIPERSCRIPT
}ReloadMethod;

class AppConfigWrapper : public QObject
{
    Q_OBJECT
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
    ReloadMethod getReloadMethod();
    void setReloadMethod(ReloadMethod mode);
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
    void setTrayMode(int);
    int getTrayMode();
    void setSpectrumEnable(bool b);
    bool getSpetrumEnable();
    int getSpectrumBands();
    void setSpectrumBands(int number);
    int getSpectrumMinFreq();
    void setSpectrumMinFreq(int number);
    int getSpectrumMaxFreq();
    void setSpectrumMaxFreq(int number);
    void setSpectrumGrid(bool b);
    bool getSpetrumGrid();
    int getSpectrumTheme();
    void setSpectrumTheme(int number);
    void setSpectrumInput(QString npath);
    QString getSpectrumInput();
    int getSpectrumRefresh();
    void setSpectrumRefresh(int number);
    float getSpectrumMultiplier();
    void setSpectrumMultiplier(float number);
    void setEqualizerPermanentHandles(bool b);
    bool getEqualizerPermanentHandles();
    void setIntroShown(bool b);
    bool getIntroShown();
signals:
    void spectrumChanged();
    void spectrumReloadRequired();
    void styleChanged();
    void eqChanged();
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
