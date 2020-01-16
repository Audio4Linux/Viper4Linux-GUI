#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H
#include "config/dbusproxy.h"

#include <QDialog>

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDialog(DBusProxy* dbus, QWidget *parent = nullptr);
    ~StatusDialog();

private:
    Ui::StatusDialog *ui;
};

#endif // STATUSDIALOG_H
