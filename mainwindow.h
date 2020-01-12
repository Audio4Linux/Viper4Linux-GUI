#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "config/io.h"
#include "config/container.h"
#include "settings.h"
#include "preset.h"
#include "ui_settings.h"
#include "convolver.h"
#include "configlist.h"
#include "converter.h"
#include "log.h"
#include "misc/stylehelper.h"
#include "config/appconfigwrapper.h"
#include "misc/mathfunctions.h"
#include "misc/loghelper.h"

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Ui::MainWindow *ui;
    void LoadPresetFile(const QString&);
    void SavePresetFile(const QString&);
    void enableSetBtn(bool on);
    void enableLogBtn(bool on);
    void enableConvBtn(bool on);
    void enablePresetBtn(bool on);
    void UpdatePeakSource(string source);
    AppConfigWrapper* getACWrapper();
    explicit MainWindow(QWidget *parent = nullptr);
    void setEQ(const int *data);
    void setIRS(const string& irs,bool apply);
    ~MainWindow();
public slots:
    void Reset();
    void Restart();
    void updatecolmpreset();
    void ConfirmConf(bool restart=true);
private slots:
    void DisableFX();
    void OpenPreset();
    void CopyEQ();
    void PasteEQ();
    void OnUpdate(bool = true);
    void OnRelease();
    void OpenConv();
    void ResetEQ();
    void OpenSettings();
    void updatepreset();
    void updatedynpreset();
    void update(int,QObject*alt=nullptr);
    void updateAll();
    void LoadExternalFile();
    void SaveExternalFile();
    void OpenLog();
private:
    ConfigContainer* conf;
    AppConfigWrapper* m_appwrapper;
    StyleHelper* m_stylehelper;
    void updateWidgetUnit(QObject* sender,QString text,bool);
    void updateeq(int);
    void setColm(const int* data);
    void setDynsys(const int* data);
    void loadConfig();
    void ConnectActions();
};

static std::string default_config = "fx_enable=true\nconv_enable=false\nconv_ir_path=\nconv_cc_level=0\nvhe_enable=false\nvhe_level=0\nvse_enable=false\nvse_ref_bark=7600\nvse_bark_cons=10\neq_enable=false\neq_band1=0\neq_band2=0\neq_band3=0\neq_band4=0\neq_band5=0\neq_band6=0\neq_band7=0\neq_band8=0\neq_band9=0\neq_band10=0\ncolm_enable=false\ncolm_widening=100\ncolm_depth=0\nds_enable=false\nds_level=0\nreverb_enable=false\nreverb_roomsize=30\nreverb_width=40\nreverb_damp=10\nreverb_wet=20\nreverb_dry=80\nagc_enable=false\nagc_ratio=100\nagc_volume=100\nagc_maxgain=100\nvb_enable=false\nvb_mode=0\nvb_freq=76\nvb_gain=0\nvc_enable=false\nvc_mode=0\nvc_level=0\ncure_enable=false\ncure_level=0\ntube_enable=false\nax_enable=false\nax_mode=0\nfetcomp_enable=false\nfetcomp_threshold=0\nfetcomp_ratio=0\nfetcomp_kneewidth=0\nfetcomp_autoknee=true\nfetcomp_gain=0\nfetcomp_autogain=true\nfetcomp_attack=51\nfetcomp_autoattack=true\nfetcomp_release=38\nfetcomp_autorelease=true\nfetcomp_meta_kneemulti=50\nfetcomp_meta_maxattack=88\nfetcomp_meta_maxrelease=88\nfetcomp_meta_crest=61\nfetcomp_meta_adapt=66\nfetcomp_noclip=true\nout_volume=100\nout_pan=0\nlim_threshold=100\ndynsys_bassgain=0\ndynsys_xcoeff1=0\ndynsys_xcoeff2=0\ndynsys_ycoeff1=0\ndynsys_ycoeff2=0\ndynsys_sidegain1=0\ndynsys_sidegain2=0\ndynsys_enable=false";
static std::string default_appconfig = "configpath=\"\"\nautoapply=false\nglavafix=false\nmuteOnRestart=true";

static inline bool is_number(const string& s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
static inline bool is_only_ascii_whitespace( const std::string& str )
{
    auto it = str.begin();
    do {
        if (it == str.end()) return true;
    } while (*it >= 0 && *it <= 0x7f && std::isspace(*(it++)));
    return false;
}
static inline float translate(int value,int leftMin,int leftMax,float rightMin,float rightMax){
    float leftSpan = leftMax - leftMin;
    float rightSpan = rightMax - rightMin;
    float valueScaled = float(value - leftMin) / float(leftSpan);
    return rightMin + (valueScaled * rightSpan);
}
#endif // MAINWINDOW_H
