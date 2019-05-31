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
    string getStylesheet();
    void LoadPresetFile(QString);
    void SavePresetFile(QString);
    void enableSetBtn(bool on);
    void enableConvBtn(bool on);
    void enablePresetBtn(bool on);
    bool getAutoFx();
    void setAutoFx(bool autofx);
    bool getMuteOnRestart();
    bool getGFix();
    void setGFix(bool);
    void setMuteOnRestart(bool on);
    void setPath(string npath);
    void setStylesheet(string);
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
    void OpenPreset();
    void CopyEQ();
    void PasteEQ();
    void OnUpdate();
    void OpenConv();
    void ConfirmConf();
    void ResetEQ();
    void OpenSettings();
    void updatepreset();
    void update(int);
    void LoadExternalFile();
    void SaveExternalFile();
private:
    void updateeq(int);
    void loadConfig(const string& key, string value);
    void decodeAppConfig(const string& key, const string& value);
    void SaveAppConfig(bool, const string&,bool,bool,const string&);
    string getMain();
    string getBass();
    string getSurround();
    string getMaster();
    string getEQ();
    string getComp();
    string getMisc();
    void ConnectActions();
    void SetStyle();
    bool is_only_ascii_whitespace(const string&);
    float translate(int value,int leftMin,int leftMax,float rightMin,float rightMax);
};

#endif // MAINWINDOW_H
