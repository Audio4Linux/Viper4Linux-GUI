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
    void setEQ(const int *data);
    ~MainWindow();
public slots:
    void Reset();
    void Restart();
    void reloadConfig();
private slots:
    void CopyEQ();
    void PasteEQ();
    void OnUpdate();
    void OpenConv();
    void ConfirmConf();
    void ResetEQ();
    void OpenSettings();
    void updatepreset();
    void update(int);
private:
    void updateeq(int);
    void loadConfig(const string& key, string value);
    void decodeAppConfig(const string& key, const string& value);
    void SaveAppConfig(bool, const string&,bool);
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
