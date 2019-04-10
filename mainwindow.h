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
    Ui::MainWindow *ui;
    string getPath();
    string getPeakSource();
    void enablePeakBtn(bool on);
    void enableSetBtn(bool on);
    void enableConvBtn(bool on);
    bool getAutoFx();
    void setAutoFx(bool autofx);
    bool getMuteOnRestart();
    void setMuteOnRestart(bool on);
    void setPath(string npath);
    void setIRS(const string& irs,bool apply=true);
    void loadAppConfig(bool once = false);
    void UpdatePeakSource(string source);
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void Reset();
    void Restart();
    void reloadConfig();
private slots:
    void OnUpdate();
    void OpenPeak();
    void OpenConv();
    void ConfirmConf();
    void ResetEQ();
    void OpenSettings();
    void updatevbgain();
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
    void loadConfig(const string& key, string value);
    void decodeAppConfig(const string& key, const string& value);
    void SaveAppConfig(bool, const string&,bool);
    void closeEvent (QCloseEvent *event);
    string getMain();
    string getBass();
    string getSurround();
    string getMaster();
    string getEQ();
    string getComp();
    string getMisc();
    void ConnectActions();
};

#endif // MAINWINDOW_H
