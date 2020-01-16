#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "config/io.h"
#include "config/container.h"
#include "settings.h"
#include "preset.h"
#include "ui_settings.h"
#include "convolver.h"
#include "converter.h"
#include "log.h"
#include "misc/stylehelper.h"
#include "config/appconfigwrapper.h"
#include "misc/mathfunctions.h"
#include "misc/loghelper.h"
#include "misc/presetextension.h"
#include "misc/common.h"
#include "config/dbusproxy.h"

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
    void EnableSettingButton(bool on);
    void EnableLogButton(bool on);
    void EnableConvolverButton(bool on);
    void EnablePresetButton(bool on);
    AppConfigWrapper* getACWrapper();
    explicit MainWindow(QWidget *parent = nullptr);
    void SetEQ(const int *data);
    void SetIRS(const string& irs,bool apply);
    ~MainWindow();
public slots:
    void Reset();
    void Restart();
    void ColmPresetSelectionUpdated();
    void ApplyConfig(bool restart=true);
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
private:
    ConfigContainer* conf;
    AppConfigWrapper* m_appwrapper;
    StyleHelper* m_stylehelper;
    DBusProxy* m_dbus;
    bool m_irsNeedUpdate = false;
    bool settingsdlg_enabled=true;
    bool presetdlg_enabled=true;
    bool logdlg_enabled=true;
    bool lockapply = false;
    string activeirs = "";

    void UpdateTooltipLabelUnit(QObject* sender,QString text,bool);
    void LoadConfig();
    void ConnectActions();
};

#endif // MAINWINDOW_H
