#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include "ui_settings.h"
class settings : public QDialog
{
    Q_OBJECT

public:
    settings(QWidget *parent = nullptr);
    Ui::settings *ui;
    ~settings();
public slots:
        void submit();
private:
};

#endif // SETTINGS_H
