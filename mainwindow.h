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
    void writeLogF(QString,QString);
    void writeLog(QString,int mode = 0);
    string getPath();
    string getStylesheet();
    string getCustompalette();
    void setWhiteIcons(bool b);
    bool getWhiteIcons();
    void setCustompalette(string s);
    void setColorpalette(string);
    void switchPalette(const QPalette& palette);
    void LoadPresetFile(QString);
    void SavePresetFile(QString);
    void enableSetBtn(bool on);
    void enableLogBtn(bool on);
    void enableConvBtn(bool on);
    void enablePresetBtn(bool on);
    string getColorpalette();
    bool getAutoFx();
    void setAutoFx(bool autofx);
    int getThememode();
    void setThememode(int mode);
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
    float translate(int value,int leftMin,int leftMax,float rightMin,float rightMax);
    ~MainWindow();
public slots:
    void Reset();
    void Restart();
    void reloadConfig();
    void updatecolmpreset();
    void ConfirmConf(bool restart=true);
private slots:
    void DisableFX();
    void OpenPreset();
    void CopyEQ();
    void PasteEQ();
    void OnUpdate();
    void OpenConv();
    void ResetEQ();
    void OpenSettings();
    void updatepreset();
    void updatedynpreset();
    void update(int);
    void LoadExternalFile();
    void SaveExternalFile();
    void OpenLog();
    void processProcOutput();
private:
    float CompMathA(float f, float f2, float f3);
    float CompMathB(float f, float f2, float f3);
    QString DoCompressorMath(int mode, float f);
    void loadIcons(bool);
    void setPalette(QColor base,QColor background,QColor foreground,QColor,QColor);
    int loadColor(int index,int rgb_index);
    void updateeq(int);
    void setColm(const int* data);
    void setDynsys(const int* data);
    void loadConfig(const string& key, string value);
    void decodeAppConfig(const string& key, const string& value);
    void SaveAppConfig(bool,const string&,bool,bool,const string&,int,const string&,const string&,bool);
    string getMain();
    string getBass();
    string getSurround();
    string getMaster();
    string getEQ();
    string getComp();
    string getMisc();
    string getDynsys();
    void ConnectActions();
    void SetStyle();
    bool is_only_ascii_whitespace(const string&);
};

#endif // MAINWINDOW_H
