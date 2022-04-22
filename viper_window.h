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
#ifndef VIPERWINDOW_H
#define VIPERWINDOW_H

#include <QFrame>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTranslator>
#include "misc/common.h"

#ifndef VIPER_PLUGINMODE
#include "visualization/audiostreamengine.h"
#include "visualization/spectrograph.h"
#endif

#ifndef VIPER_PLUGINMODE
#include "config/dbusproxy.h"
#endif

//Minimum required version of gst-plugin-viperfx
#define MINIMUM_PLUGIN_VERSION "2.0.0"

using namespace std;
namespace Ui {
class ViperWindow;
}

class AppConfigWrapper;
class ConfigContainer;
class OverlayMsgProxy;
class SettingsDlg;
class ConvolverDlg;
class PresetDlg;
class LogDlg;
class StyleHelper;

class ViperWindow : public QWidget
{
    Q_OBJECT
    enum class Context;
public:
    Ui::ViperWindow *ui;
#ifndef VIPER_PLUGINMODE
    explicit ViperWindow(QString exepath = "",
                         bool statupInTray = false,
                         bool allowMultipleInst = false,
                         QWidget *parent = nullptr);
#else
    Q_INVOKABLE explicit ViperWindow(QString working_dir,
                                     QWidget *parent = nullptr);
#endif
    ~ViperWindow();

    void LoadPresetFile(const QString&);
    void SavePresetFile(const QString&);
    AppConfigWrapper* getACWrapper();
    void SetEQ(const QVector<float>& data);
    void SetIRS(const QString& irs,bool apply);

    void setVisible(bool visible) override;

    void setPopupButtonEnabled(bool b);

    QMenu* buildAvailableActions();
    QMenu *buildDefaultActions();

    void LoadConfig(Context ctx = Context::Application);
    ConfigContainer* conf;

#ifndef VIPER_PLUGINMODE   
    QMenu *getTrayContextMenu();
    void updateTrayMenu(QMenu *menu);
    void setTrayVisible(bool visible);

    QString GetExecutablePath();
    void LaunchFirstRunSetup();
#endif

    SettingsDlg *settings_dlg;

    void loadConfigFromContainer(ConfigContainer c);
#ifdef VIPER_PLUGINMODE
    QString getLegacyPath();
#endif

signals:
    void popupButtonPressed();

protected:
    void closeEvent(QCloseEvent *event) override;
public slots:
    void Reset();

    void ColmPresetSelectionUpdated();
    void ApplyConfig(bool restart=true);

#ifndef VIPER_PLUGINMODE
    void Restart();

    void RestartSpectrum();
    void raiseWindow();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif
private slots:
    void DisableFX();
    void OnUpdate(bool = true);
    void OnRelease();
    void ResetEQ();
    void EqPresetSelectionUpdated();
    void DynsysPresetSelectionUpdated();
    void UpdateUnitLabel(int,QObject*alt=nullptr);
    void UpdateAllUnitLabels();
    void LoadExternalFile();
    void SaveExternalFile();
    void OpenLog();
    void DialogHandler();
    void UpdateEqStringFromWidget();
    void UpdateDynsysStringFromWidget();
    void UpdateColmStringFromWidget();

#ifndef VIPER_PLUGINMODE
    void RefreshSpectrumParameters();
    void updateTrayPresetList();
#endif

signals:
    void changesApplied();

protected:

private:
    AppConfigWrapper* m_appwrapper;
    StyleHelper* m_stylehelper;
#ifndef VIPER_PLUGINMODE
    DBusProxy* m_dbus;
#endif
    ConvolverDlg *conv_dlg;
    PresetDlg *preset_dlg;
    LogDlg *log_dlg;

#ifndef VIPER_PLUGINMODE
    QString m_exepath;
    bool m_startupInTraySwitch;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *quitAction;
    QAction *tray_disableAction;
    QMenu *tray_presetMenu;
    QMenu *tray_convMenu;
    OverlayMsgProxy *msg_launchfail;
    OverlayMsgProxy *msg_versionmismatch;

    QAction *spectrum;

    QScopedPointer<QFrame> analysisLayout;
    Spectrograph* m_spectrograph;
    AudioStreamEngine* m_audioengine;
#endif
    OverlayMsgProxy *msg_notrunning;

    bool settingsdlg_enabled=true;
    bool presetdlg_enabled=true;
    bool logdlg_enabled=true;
    bool lockapply = false;
    QString activeirs = "";

#ifndef VIPER_PLUGINMODE
    void InitializeSpectrum();
    void ToggleSpectrum(bool on,bool ctrl_visibility);
    void SetSpectrumVisibility(bool v);

    void createTrayIcon();
    void initGlobalTrayActions();
    void updateTrayConvolverList();

    void CheckDBusVersion();
    void RunDiagnosticChecks();
#endif

    void ShowDBusError();
    void UpdateTooltipLabelUnit(QObject* sender,const QString& text,bool);
    void ConnectActions();

    QVariantMap readConfig(const QString& path = "");


    enum class Context{
        DBus,
        Application
    };


    static void replaceTab(QTabWidget* tab, int index, QWidget *page, QString title = ""){
        if(title.isEmpty()) title = tab->tabText(index);
        auto toDelete = tab->widget(index);
        tab->removeTab(index);
        toDelete->deleteLater();
        tab->insertTab(index, page, title);
    }
};

#endif // VIPERWINDOW_H
