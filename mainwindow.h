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
    string getMisc();
    ~MainWindow();
public slots:
    void ConfirmConf();
    void ResetEQ();
    void Reset();
    void reloadConfig();

    void updatevbfreq();
    void updatevbmode();
    void updatedifflvl();
    void updatevhplvl();
    void updateroomsize();
    void updateroomwidth();
    void updateroomdamp();
    void updatewet();
    void updatedry();
    void updatecolmwide();
    void updatecolmmidimg();
    void updatecolmdepth();
    void updatevclvl();
    void updatevcmode();
    void updategain();
    void updatemaxgain();
    void updatemaxvol();
    void updatelimiter();
    void updateoutputpan();
    void updateoutvolume();
    void updatevcurelvl();
    void updateaxmode();
    void updatebarkfreq();
    void updatebarkcon();
    void updatecomprelease();
    void updatecompgain();
    void updatecompwidth();
    void updatecomp_ratio();
    void updatecomp_thres();
    void updatecompattack();
    void updatea_adapt();
    void updatea_crest();
    void updatea_maxatk();
    void updatea_maxrel();
    void updatea_kneewidth();
    void updateeq1();
    void updateeq2();
    void updateeq3();
    void updateeq4();
    void updateeq5();
    void updateeq6();
    void updateeq7();
    void updateeq8();
    void updateeq9();
    void updateeq10();
    void updatecc();
private:
    Ui::MainWindow *ui;
    constexpr unsigned int str2int(const char* str, int h = 0);
    void loadConfig(string key, string value);
};

#endif // MAINWINDOW_H
