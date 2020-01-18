#ifndef LOG_H
#define LOG_H

#include <QDialog>

namespace Ui {
class log;
}

class LogDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LogDlg(QWidget *parent = nullptr);
    ~LogDlg();
private slots:
    void reject();
    void updateLog();
private:
    Ui::log *ui;
};

#endif // LOG_H
