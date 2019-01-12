#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <iostream>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    string getMain();
    string getBass();
    string getSurround();
    string getMaster();
    string getEQ();
    string getComp();
    ~MainWindow();
public slots:
    void ConfirmConf();
    void ResetEQ();
private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
