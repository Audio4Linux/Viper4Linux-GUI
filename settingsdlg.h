#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ui_settings.h"
#include "config/appconfigwrapper.h"

class MainWindow;

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    SettingsDlg(MainWindow* mainwin, QWidget *parent = nullptr);
    Ui::settings *ui;
    ~SettingsDlg();
private slots:
        void openPalConfig();
        void github();
        void glava_help();
        void changeStyle(const QString&);
        void changeThemeMode();
        void changePalette(const QString&);
        void updateAutoFX();
        void updateMuteRestart();
        void updatePath();
        void updateIrsPath();
        void updateGLava();
        void updateAutoFxMode();
        void updateCDefTab();
        void updateTheme();
private:
        AppConfigWrapper* appconf;
        MainWindow* m_mainwin;
};

#endif // SETTINGS_H
