#ifndef CONVOLVER_H
#define CONVOLVER_H

#include <QDialog>
class MainWindow;

namespace Ui {
class Convolver;
}

class ConvolverDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConvolverDlg(MainWindow* mainwin,QWidget *parent = nullptr);
    ~ConvolverDlg();
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
    MainWindow* m_mainwin;
    QString configpath = "";
};

#endif // CONVOLVER_H
