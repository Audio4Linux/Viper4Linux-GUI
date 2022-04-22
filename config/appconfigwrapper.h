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

using namespace std;

class StyleHelper;
class ConfigContainer;

typedef enum class ReloadMethod{
    DIRECT_DBUS,
    USE_VIPERSCRIPT
}ReloadMethod;

class AppConfigWrapper : public QObject
{
    Q_OBJECT
public:
    AppConfigWrapper(StyleHelper* stylehelper=nullptr, QString _working_dir_plgmode = "");
    void saveAppConfig();
    void loadAppConfig();
    QString getAppConfigFilePath();

    void setConv_DefTab(int mode);
    int getConv_DefTab();
    void setIrsPath(const QString& npath);
    QString getIrsPath();
    void setTheme(const QString&);
    QString getTheme();
    void setLanguage(const QString&);
    QString getLanguage();

    void setWhiteIcons(bool b);
    bool getWhiteIcons();
    void setCustompalette(const QString& s);
    QString getCustompalette();
    void setColorpalette(const QString&);
    QString getColorpalette();
    void setEqualizerPermanentHandles(bool b);
    bool getEqualizerPermanentHandles();


    int getAutoFxMode();
    bool getAutoFx();

#ifndef VIPER_PLUGINMODE
    void setAutoFxMode(int);
    void setAutoFx(bool afx);

    void setPath(const QString& npath);
    QString getPath();

    ReloadMethod getReloadMethod();
    void setReloadMethod(ReloadMethod mode);
    bool getGFix();
    void setGFix(bool);
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
    void setSpectrumInput(const QString& npath);
    QString getSpectrumInput();
    int getSpectrumRefresh();
    void setSpectrumRefresh(int number);
    float getSpectrumMultiplier();
    void setSpectrumMultiplier(float number);
    void setIntroShown(bool b);
    bool getIntroShown();
    QString getTrayContextMenu();
    void setTrayContextMenu(const QString &ctx);
    void setSpectrumShape(int number);
    int getSpectrumShape();
    void setSyncDisabled(bool b);
    bool getSyncDisabled();
#endif

signals:
    void spectrumChanged();
    void spectrumReloadRequired();
    void styleChanged();
    void eqChanged();
    void languageChanged();
private:
    ConfigContainer* appconf;
    StyleHelper*     m_stylehelper;
    QString          working_dir_plgmode;
};

inline QString chopFirstLastChar(QString i){
    if(i.size() > 2){
        i.remove(0,1);
        i.chop(1);
    }
    return i;
}

#endif // APPCONFIGWRAPPER_H
