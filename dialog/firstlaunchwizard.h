#ifndef FIRSTLAUNCHWIZARD_H
#define FIRSTLAUNCHWIZARD_H

#include <QWidget>
#include "config/appconfigwrapper.h"

class ViperWindow;

namespace Ui {
class FirstLaunchWizard;
}

class FirstLaunchWizard : public QWidget
{
    Q_OBJECT

public:
    explicit FirstLaunchWizard(AppConfigWrapper* _appconf, ViperWindow*,QWidget *parent = nullptr);
    ~FirstLaunchWizard();

signals:
    void wizardFinished();

private:
    Ui::FirstLaunchWizard *ui;
    AppConfigWrapper* appconf;
    bool lockslot = false;

    void refreshDevices();
};

#endif // FIRSTLAUNCHWIZARD_H
