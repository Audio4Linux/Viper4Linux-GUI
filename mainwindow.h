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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include "config/io.h"
#include "config/container.h"
#include "dialog/settingsdlg.h"
#include "dialog/presetdlg.h"
#include "ui_settings.h"
#include "dialog/convolverdlg.h"
#include "misc/converter.h"
#include "dialog/logdlg.h"
#include "misc/stylehelper.h"
#include "config/appconfigwrapper.h"
#include "misc/mathfunctions.h"
#include "misc/loghelper.h"
#include "misc/presetextension.h"
#include "misc/common.h"
#include "config/dbusproxy.h"
#include "misc/overlaymsgproxy.h"

//Minimum required version of gst-plugin-viperfx
#define MINIMUM_PLUGIN_VERSION "2.0.0"

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    void LoadPresetFile(const QString&);
    void SavePresetFile(const QString&);
    AppConfigWrapper* getACWrapper();
    explicit MainWindow(QString exepath,bool traySwitch,bool allowMultipleInst,QWidget *parent = nullptr);
    void SetEQ(const int *data);
    void SetIRS(const QString& irs,bool apply);
    QString GetExecutablePath();
    void setVisible(bool visible) override;
    void setTrayVisible(bool visible);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
public slots:
    void Reset();
    void Restart();
    void raiseWindow();
    void ColmPresetSelectionUpdated();
    void ApplyConfig(bool restart=true);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
private slots:
    void DisableFX();
    void CopyEQ();
    void PasteEQ();
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
    void updateTrayPresetList();
private:
    ConfigContainer* conf;
    AppConfigWrapper* m_appwrapper;
    StyleHelper* m_stylehelper;
    DBusProxy* m_dbus;
    QString m_exepath;

    bool m_startupInTraySwitch;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *quitAction;
    QAction *disableAction;
    QMenu *presetMenu;

    OverlayMsgProxy *msg_notrunning;
    OverlayMsgProxy *msg_launchfail;
    OverlayMsgProxy *msg_versionmismatch;

    ConvolverDlg *conv_dlg;
    SettingsDlg *settings_dlg;
    PresetDlg *preset_dlg;
    LogDlg *log_dlg;

    bool m_irsNeedUpdate = false;
    bool settingsdlg_enabled=true;
    bool presetdlg_enabled=true;
    bool logdlg_enabled=true;
    bool lockapply = false;
    QString activeirs = "";

    void createTrayIcon();
    void UpdateTooltipLabelUnit(QObject* sender,QString text,bool);
    void LoadConfig();
    void ConnectActions();
    void ShowDBusError();
    void CheckDBusVersion();
    QVariantMap readConfig();
};

#endif // MAINWINDOW_H
