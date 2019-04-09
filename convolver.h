#ifndef CONVOLVER_H
#define CONVOLVER_H

#include <QDialog>

namespace Ui {
class Convolver;
}

class Convolver : public QDialog
{
    Q_OBJECT

public:
    explicit Convolver(QWidget *parent = nullptr);
    ~Convolver();
private slots:
    void reject();
    void reload();
    void closeWindow();
    void updateIR();
private:
    Ui::Convolver *ui;
};

#endif // CONVOLVER_H
