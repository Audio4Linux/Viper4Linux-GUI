#ifndef PEAK_H
#define PEAK_H

#include <QDialog>

namespace Ui {
class peak;
}

class peak : public QDialog
{
    Q_OBJECT

public:
    explicit peak(QWidget *parent = nullptr);
    ~peak();

private:
    Ui::peak *ui;
};

#endif // PEAK_H
