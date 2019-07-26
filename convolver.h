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
    void reloadFav();
    void addFav();
    void renameFav();
    void removeFav();
    void closeWindow();
    void updateIR();
    void updateIR_Fav();
    void selectFolder();
private:
    Ui::Convolver *ui;
};

#endif // CONVOLVER_H
