#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "preset.h"
#include "ui_settings.h"
#include "convolver.h"
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <map>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "configlist.h"
#include <ostream>
#include <QApplication>
#include <QMessageBox>
#include <utility>
#include <QProcess>
#include <QDebug>
#include <math.h>
#include <QClipboard>
#include "main.h"
#include <vector>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include "converter.h"
#include <cctype>
#include <cmath>
#include <QStyleFactory>
using namespace std;

static string path;
static string appcpath;
static string style_sheet;
static string qstyle;
static bool autofx;
static bool muteOnRestart;
static bool glava_fix;
static bool settingsdlg_enabled=true;
static bool presetdlg_enabled=true;
static bool lockapply = false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char result[100];
    strcpy(result,homedir);
    strcat(result,"/.config/viper4linux/audio.conf");
    char result2[100];
    strcpy(result2,homedir);
    strcat(result2,"/.config/viper4linux/ui.conf");
    path = result;
    appcpath = result2;

    reloadConfig();
    loadAppConfig();

    QMenu *menu = new QMenu();
    menu->addAction("Reload Viper", this,SLOT(Restart()));
    menu->addAction("Load from file", this,SLOT(LoadExternalFile()));
    menu->addAction("Save to file", this,SLOT(SaveExternalFile()));
    ui->toolButton->setMenu(menu);
    QMenu *menuC = new QMenu();
    menuC->addAction("Slight", this,[this](){ ui->colmpreset->setText("Slight"); updatecolmpreset();});
    menuC->addAction("Level 1", this,[this](){ ui->colmpreset->setText("Level 1"); updatecolmpreset();});
    menuC->addAction("Level 2", this,[this](){ ui->colmpreset->setText("Level 2"); updatecolmpreset();});
    menuC->addAction("Level 3", this,[this](){ ui->colmpreset->setText("Level 3"); updatecolmpreset();});
    menuC->addAction("Level 4", this,[this](){ ui->colmpreset->setText("Level 4"); updatecolmpreset();});
    menuC->addAction("Level 5", this,[this](){ ui->colmpreset->setText("Level 5"); updatecolmpreset();});
    menuC->addAction("Level 6", this,[this](){ ui->colmpreset->setText("Level 6"); updatecolmpreset();});
    menuC->addAction("Level 7", this,[this](){ ui->colmpreset->setText("Level 7"); updatecolmpreset();});
    menuC->addAction("Extreme", this,[this](){ ui->colmpreset->setText("Extreme"); updatecolmpreset();});
    ui->colmpreset->setMenu(menuC);
    SetStyle();
    ConnectActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//---Style
void MainWindow::SetStyle(){
    if(qstyle!="Windows"){
        QString stylepath = "";
        if(style_sheet=="dark_orange")stylepath = ":darkorange/darkorange.qss";
        else if (style_sheet=="blue")stylepath = ":darkblue/darkblue/darkblue.qss";
        else if (style_sheet=="breeze_light")stylepath = ":/lightbreeze/lightbreeze/lightbreeze.qss";
        else if (style_sheet=="breeze_dark")stylepath = ":/darkbreeze/darkbreeze/darkbreeze.qss";
        else if (style_sheet=="amoled")stylepath = ":amoled/amoled/amoled.qss";
        else if (style_sheet=="aqua")stylepath = ":/aqua/aqua/aqua.qss";
        else if (style_sheet=="materialdark")stylepath = ":/materialdark/materialdark/materialdark.qss";
        else if (style_sheet=="ubuntu")stylepath = ":/ubuntu/ubuntu/ubuntu.qss";
        else if (style_sheet=="vsdark")stylepath = ":/vsdark/vsdark/vsdark.qss";
        else if (style_sheet=="vslight")stylepath = ":/vslight/vslight/vslight.qss";
        else stylepath = ":/default.qss";

        QFile f(stylepath);
        if (!f.exists())printf("Unable to set stylesheet, file not found\n");
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
            if(style_sheet=="amoled" || style_sheet=="console" || style_sheet=="materialdark" || style_sheet=="breeze_dark" || style_sheet=="vsdark"){
                QPixmap pix(":/icons/settings-white.svg");
                QIcon icon(pix);
                QPixmap pix2(":/icons/queue-white.svg");
                QIcon icon2(pix2);
                QPixmap pix3(":/icons/menu-white.svg");
                QIcon icon3(pix3);
                ui->set->setIcon(icon);
                ui->cpreset->setIcon(icon2);
                ui->toolButton->setIcon(icon3);
            }else{
                QPixmap pix(":/icons/settings.svg");
                QIcon icon(pix);
                QPixmap pix2(":/icons/queue.svg");
                QIcon icon2(pix2);
                QPixmap pix3(":/icons/menu.svg");
                QIcon icon3(pix3);
                ui->set->setIcon(icon);
                ui->cpreset->setIcon(icon2);
                ui->toolButton->setIcon(icon3);
            }
        }
    }else{
        QFile f(":/default.qss");
        if (!f.exists())printf("Unable to set stylesheet, file not found\n");
        else
        {
            f.open(QFile::ReadOnly | QFile::Text);
            QTextStream ts(&f);
            qApp->setStyleSheet(ts.readAll());
            QPixmap pix(":/icons/settings.svg");
            QIcon icon(pix);
            QPixmap pix2(":/icons/queue.svg");
            QIcon icon2(pix2);
            QPixmap pix3(":/icons/menusvg");
            QIcon icon3(pix3);
            ui->set->setIcon(icon);
            ui->cpreset->setIcon(icon2);
            ui->toolButton->setIcon(icon3);
    }
    }
    if(QStyleFactory::keys().contains(QString::fromStdString(qstyle)))app->setStyle(QString::fromStdString(qstyle));
    else app->setStyle("Fusion");
}

//---Dialogs
void MainWindow::OpenConv(){
    enableConvBtn(false);
    auto c = new Convolver(this);
    c->setFixedSize(c->geometry().width(),c->geometry().height());
    c->show();
}
void MainWindow::OpenSettings(){
    if(settingsdlg_enabled==true){
        enableSetBtn(false);
        auto setting = new settings(this);
        setting->setFixedSize(setting->geometry().width(),setting->geometry().height());
        setting->show();
    }
}
void MainWindow::OpenPreset(){
    if(presetdlg_enabled==true){
        enablePresetBtn(false);
        auto _preset = new Preset(this);
        preset = _preset;
        _preset->setFixedSize(_preset->geometry().width(),_preset->geometry().height());
        _preset->show();
    }
}
void MainWindow::enableSetBtn(bool on){
    settingsdlg_enabled=on;
}
void MainWindow::enablePresetBtn(bool on){
    presetdlg_enabled=on;
}
void MainWindow::enableConvBtn(bool on){
    ui->conv_select->setEnabled(on);
}

//---Apply
void MainWindow::OnUpdate(){
    if(autofx && !lockapply)ConfirmConf();
}
void MainWindow::ConfirmConf(){
    string config = "fx_enable=";
    if(!ui->disableFX->isChecked())config += "true\n";
    else config += "false\n";

    config += getMain();
    config += getBass();
    config += getSurround();
    config += getMaster();
    config += getEQ();
    config += getComp();
    config += getMisc();
    config += getDynsys();

    ofstream myfile(path);
    if (myfile.is_open())
    {
        myfile << config;
        myfile.close();
    }
    else cerr << "Unable to open file";
    Restart();
}
void MainWindow::Reset(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Are you sure?", "Reset Configuration",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        std::filebuf fb;
        fb.open (path,std::ios::out);
        std::ostream os(&fb);
        os << default_config;
        fb.close();

        reloadConfig();
        ConfirmConf();
    }
}
void MainWindow::Restart(){
    if(muteOnRestart) system("pactl set-sink-mute 0 1");
    if(glava_fix) system("killall -r glava");
    system("viper restart");
    if(glava_fix) system("setsid glava -d &");
    if(muteOnRestart) system("pactl set-sink-mute 0 0");
}
void MainWindow::DisableFX(){
    //Apply instantly
    if(!lockapply)ConfirmConf();
}

//---Save/Load Presets
void MainWindow::LoadPresetFile(QString filename){
    const QString src = filename;
    const QString dest = QString::fromStdString(path);
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    cout << "Loading from " << filename.toUtf8().constData() << endl;
    reloadConfig();
    ConfirmConf();
}
void MainWindow::SavePresetFile(QString filename){
    const QString src = QString::fromStdString(path);
    const QString dest = filename;
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    cout << "Saving to " << filename.toUtf8().constData() << endl;
}
void MainWindow::LoadExternalFile(){
    QString filename = QFileDialog::getOpenFileName(this,"Load custom audio.conf","","*.conf");
    if(filename=="")return;
    const QString src = filename;
    const QString dest = QString::fromStdString(path);
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    cout << "Loading from " << filename.toUtf8().constData() << endl;
    reloadConfig();
    ConfirmConf();
}
void MainWindow::SaveExternalFile(){
    QString filename = QFileDialog::getSaveFileName(this,"Save current audio.conf","","*.conf");
    if(filename=="")return;
    QFileInfo fi(filename);
    QString ext = fi.suffix();
    if(ext!="conf")filename.append(".conf");

    const QString src = QString::fromStdString(path);
    const QString dest = filename;
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    cout << "Saving to " << filename.toUtf8().constData() << endl;
}

//---UI Config Loader
void MainWindow::decodeAppConfig(const string& key,const string& value){
    //cout << "AppConf: " << key << " -> " << value << endl;
    switch (resolveAppConfig(key)) {
    case unknownApp: {break;}
    case autoapply: {
        autofx = value=="true";
        break;
    }
    case configpath: {
        if(value.size() <= 2) break;
        path = value.substr(1, value.size() - 2);
        break;
    }
    case glavafix: {
        glava_fix = value=="true";
        break;
    }
    case stylesheet: {
        style_sheet = value;
        break;
    }
    case theme: {
        qstyle = value;
        break;
    }
    case mutedRestart: {
        muteOnRestart = value=="true";
        break;
    }
    }
}
void MainWindow::loadAppConfig(bool once){
    cout << "Reloading App Config..." << endl;
    std::ifstream cFile(appcpath);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
             if(line[0] == '#' || line.empty() || line.empty()) continue;
            auto delimiterPos = line.find('=');
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            decodeAppConfig(name,value);
        }
        cFile.close();
    }
    else {
        cerr << "Couldn't open app config file for reading.\n";
        ofstream outfile(appcpath);
        outfile << default_appconfig << endl;
        outfile.close();
        if(!once)loadAppConfig(true);
    }
}

//---UI Config Generator
void MainWindow::SaveAppConfig(bool afx = autofx, const string& cpath = path, bool muteRestart = muteOnRestart,bool g_fix = glava_fix, const string &ssheet = style_sheet,const string &_theme = qstyle){
    string appconfig;
    stringstream converter1;
    converter1 << boolalpha << afx;
    appconfig += "autoapply=" + converter1.str() + "\n";
    appconfig += "configpath=\"" + cpath + "\"\n";

    stringstream converter2;
    converter2 << boolalpha << muteRestart;
    appconfig += "muteOnRestart=" + converter2.str() + "\n";

    stringstream converter3;
    converter3 << boolalpha << g_fix;
    appconfig += "glavafix=" + converter3.str() + "\n";

    appconfig += "stylesheet=" + ssheet + "\n";
    appconfig += "theme=" + _theme + "\n";

    ofstream myfile(appcpath);
    if (myfile.is_open())
    {
        myfile << appconfig;
        myfile.close();
    }
    else{
        cerr << "Unable to open file. Checking...";
        loadAppConfig();
    }
}

//---Viper Config Loader
void MainWindow::reloadConfig(){
    lockapply=true;
    cout << "Reloading..." << endl;
    std::ifstream cFile(path);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            if(QString::fromStdString(line).trimmed()[0] == '#' || line.empty()) continue; //Skip commented lines
            auto delimiterInlineComment = line.find('#'); //Look for config properties mixed up with comments
            auto extractedProperty = line.substr(0, delimiterInlineComment);
            auto delimiterPos = extractedProperty.find('=');
            auto name = extractedProperty.substr(0, delimiterPos);
            auto value = extractedProperty.substr(delimiterPos + 1);
            loadConfig(name,value);
        }
        cFile.close();
    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
        QMessageBox msgBox;
        msgBox.setText("Viper Configuration File not found at \n" + QString::fromStdString(path) + "");
        msgBox.setInformativeText("You can change the path in the settings.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
    }
    lockapply=false;
}
void MainWindow::loadConfig(const string& key,string value){
    //cout << key << " -> " << value << endl;
    if(value==""||is_only_ascii_whitespace(value)){
        cerr << "[WARNING] Key " + key + " is empty" << endl;
        return;
    }
    switch (resolveConfig(key)) {
    case fx_enable: {
        if(value=="true") ui->disableFX->setChecked(false);
        else ui->disableFX->setChecked(true);
        break;
    }
    case tube_enable: {
        if(value=="true") ui->tubesim->setCheckState(Qt::CheckState::Checked);
        else ui->tubesim->setChecked(false);
        break;
    }
    case colm_enable: {
        if(value=="true") ui->colm->setChecked(true);
        else ui->colm->setChecked(false);
        break;
    }
    case colm_widening: {
        ui->colmwide->setValue(std::stoi(value));
        break;
    }
    case colm_depth: {
        ui->colmdepth->setValue(std::stoi(value));
        break;
    }
    case colm_midimage: {
        ui->colmmidimg->setValue(std::stoi(value));
        break;
    }
    case vc_enable: {
        if(value=="true") ui->clarity->setChecked(true);
        else ui->clarity->setChecked(false);
        break;
    }
    case vc_mode: {
        ui->vcmode->setValue(std::stoi(value));
        break;
    }
    case vc_level: {
        ui->vclvl->setValue(std::stoi(value));
        break;
    }
    case vb_enable: {
        if(value=="true") ui->vb->setChecked(true);
        else ui->vb->setChecked(false);
        break;
    }
    case vb_mode: {
        ui->vbmode->setValue(std::stoi(value));
        break;
    }
    case vb_freq: {
        ui->vbfreq->setValue(std::stoi(value));
        break;
    }
    case vb_gain: {
        ui->vbgain->setValue(std::stoi(value));
        break;
    }
    case vhe_enable: {
        if(value=="true") ui->vhp->setChecked(true);
        else ui->vhp->setChecked(false);
        break;
    }
    case vhe_level: {
        ui->vhplvl->setValue(std::stoi(value));
        break;
    }
    case ds_enable: {
        if(value=="true") ui->diff->setChecked(true);
        else ui->diff->setChecked(false);
        break;
    }
    case ds_level: {
        ui->difflvl->setValue(std::stoi(value));
        break;
    }
    case reverb_enable: {
        if(value=="true") ui->reverb->setChecked(true);
        else ui->reverb->setChecked(false);
        break;
    }
    case reverb_roomsize: {
        ui->roomsize->setValue(std::stoi(value));
        break;
    }
    case reverb_width: {
        ui->roomwidth->setValue(std::stoi(value));
        break;
    }
    case reverb_damp: {
        ui->roomdamp->setValue(std::stoi(value));
        break;
    }
    case reverb_wet: {
        ui->wet->setValue(std::stoi(value));
        break;
    }
    case reverb_dry: {
        ui->dry->setValue(std::stoi(value));
        break;
    }
    case agc_enable: {
        if(value=="true") ui->agc->setChecked(true);
        else ui->agc->setChecked(false);
        break;
    }
    case agc_ratio: {
        ui->gain->setValue(std::stoi(value));
        break;
    }
    case agc_maxgain: {
        ui->maxgain->setValue(std::stoi(value));
        break;
    }
    case agc_volume: {
        ui->maxvol->setValue(std::stoi(value));
        break;
    }
    case lim_threshold: {
        ui->limiter->setValue(std::stoi(value));
        break;
    }
    case out_pan: {
        ui->outputpan->setValue(std::stoi(value));
        break;
    }
    case out_volume: {
        ui->outvolume->setValue(std::stoi(value));
        break;
    }
    case eq_enable: {
        if(value=="true") ui->enable_eq->setChecked(true);
        else ui->enable_eq->setChecked(false);
        break;
    }
    case eq_band1: {
        ui->eq1->setValue(std::stoi(value));
        break;
    }
    case eq_band2: {
        ui->eq2->setValue(std::stoi(value));
        break;
    }
    case eq_band3: {
        ui->eq3->setValue(std::stoi(value));
        break;
    }
    case eq_band4: {
        ui->eq4->setValue(std::stoi(value));
        break;
    }
    case eq_band5: {
        ui->eq5->setValue(std::stoi(value));
        break;
    }
    case eq_band6: {
        ui->eq6->setValue(std::stoi(value));
        break;
    }
    case eq_band7: {
        ui->eq7->setValue(std::stoi(value));
        break;
    }
    case eq_band8: {
        ui->eq8->setValue(std::stoi(value));
        break;
    }
    case eq_band9: {
        ui->eq9->setValue(std::stoi(value));
        break;
    }
    case eq_band10: {
        ui->eq10->setValue(std::stoi(value));
        break;
    }
    case fetcomp_enable: {
        if(value=="true") ui->enable_comp->setChecked(true);
        else ui->enable_comp->setChecked(false);
        break;
    }
    case fetcomp_autogain: {
        if(value=="false") ui->m_gain->setChecked(true);
        else ui->m_gain->setChecked(false);
        break;
    }
    case fetcomp_autoknee: {
        if(value=="false") ui->m_width->setChecked(true);
        else ui->m_width->setChecked(false);
        break;
    }
    case fetcomp_autoattack: {
        if(value=="false") ui->m_attack->setChecked(true);
        else ui->m_attack->setChecked(false);
        break;
    }
    case fetcomp_autorelease: {
        if(value=="false") ui->m_release->setChecked(true);
        else ui->m_release->setChecked(false);
        break;
    }
    case fetcomp_noclip: {
        if(value=="true") ui->noclip->setChecked(true);
        else ui->noclip->setChecked(false);
        break;
    }
    case fetcomp_threshold: {
        ui->comp_thres->setValue(std::stoi(value));
        break;
    }
    case fetcomp_gain: {
        ui->compgain->setValue(std::stoi(value));
        break;
    }
    case fetcomp_kneewidth: {
        ui->compwidth->setValue(std::stoi(value));
        break;
    }
    case fetcomp_ratio: {
        ui->comp_ratio->setValue(std::stoi(value));
        break;
    }
    case fetcomp_attack: {
        ui->compattack->setValue(std::stoi(value));
        break;
    }
    case fetcomp_release: {
        ui->comprelease->setValue(std::stoi(value));
        break;
    }
    case fetcomp_meta_adapt: {
        ui->a_adapt->setValue(std::stoi(value));
        break;
    }
    case fetcomp_meta_crest: {
        ui->a_crest->setValue(std::stoi(value));
        break;
    }
    case fetcomp_meta_maxattack: {
        ui->a_maxatk->setValue(std::stoi(value));
        break;
    }
    case fetcomp_meta_maxrelease: {
        ui->a_maxrel->setValue(std::stoi(value));
        break;
    }
    case fetcomp_meta_kneemulti: {
        ui->a_kneewidth->setValue(std::stoi(value));
        break;
    }
    case cure_enable: {
        if(value=="true") ui->vcure->setChecked(true);
        else ui->vcure->setChecked(false);
        break;
    }
    case cure_level: {
        ui->vcurelvl->setValue(std::stoi(value));
        break;
    }
    case ax_enable: {
        if(value=="true") ui->ax->setChecked(true);
        else ui->ax->setChecked(false);
        break;
    }
    case ax_mode: {
        ui->axmode->setValue(std::stoi(value));
        break;
    }
    case vse_enable: {
        if(value=="true") ui->vse->setChecked(true);
        else ui->vse->setChecked(false);
        break;
    }
    case vse_ref_bark: {
        ui->barkfreq->setValue(std::stoi(value));
        break;
    }
    case vse_bark_cons: {
        ui->barkcon->setValue(std::stoi(value));
        break;
    }
    case conv_enable: {
        if(value=="true") ui->conv->setChecked(true);
        else ui->conv->setChecked(false);
        break;
    }
    case conv_cc_level: {
        ui->convcc->setValue(std::stoi(value));
        break;
    }
    case conv_ir_path: {
        if(value.size() <= 2) break;
        value = value.substr(1, value.size() - 2);
        QString ir = QString::fromStdString(value);
        qDebug() << "READ:" << ir;
        ui->convpath->setText(ir);
        break;
    }
    case dynsys_enable: {
        if(value=="true") ui->dynsys->setChecked(true);
        else ui->dynsys->setChecked(false);
        break;
    }
    case dynsys_xcoeff1: {
        ui->dyn_xcoeff1->setValue(std::stoi(value));
        break;
    }
    case dynsys_xcoeff2: {
        ui->dyn_xcoeff2->setValue(std::stoi(value));
        break;
    }
    case dynsys_ycoeff1: {
        ui->dyn_ycoeff1->setValue(std::stoi(value));
        break;
    }
    case dynsys_ycoeff2: {
        ui->dyn_ycoeff2->setValue(std::stoi(value));
        break;
    }
    case dynsys_bassgain: {
        ui->dyn_bassgain->setValue(std::stoi(value));
        break;
    }
    case dynsys_sidegain1: {
        ui->dyn_sidegain1->setValue(std::stoi(value));
        break;
    }
    case dynsys_sidegain2: {
        ui->dyn_sidegain2->setValue(std::stoi(value));
        break;
    }
    case unknown: {
        cout << "Config-List Enum: Unknown" << endl;
        break;
    }
    }
}

//---Viper Config Generator
string MainWindow::getMisc(){
    string out;
    string n = "\n";

    QString vcurelvl = QString::number(ui->vcurelvl->value());
    QString axmode = QString::number(ui->axmode->value());

    QString refbark = QString::number(ui->barkfreq->value());
    QString conbark = QString::number(ui->barkcon->value());
    QString convcc = QString::number(ui->comprelease->value());
    QString convir = ui->convpath->text();
    qDebug() << "WRITE:" << convir;

    out += "cure_enable=";
    if(ui->vcure->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "cure_level=";
    out += vcurelvl.toUtf8().constData() + n;

    out += "ax_enable=";
    if(ui->ax->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "ax_mode=";
    out += axmode.toUtf8().constData() + n;

    out += "vse_enable=";
    if(ui->vse->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vse_ref_bark=";
    out += refbark.toUtf8().constData() + n;
    out += "vse_bark_cons=";
    out += conbark.toUtf8().constData() + n;

    out += "conv_enable=";
    if(ui->conv->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "conv_cc_level=";
    out += convcc.toUtf8().constData() + n;
    out += "conv_ir_path=\"";
    out += convir.toUtf8().constData();
    out += "\"" + n;

    return out;
}
string MainWindow::getComp() {
    string out;
    string n = "\n";
    QString gain = QString::number(ui->compgain->value());
    QString width = QString::number(ui->compwidth->value());
    QString ratio = QString::number(ui->comp_ratio->value());
    QString thres = QString::number(ui->comp_thres->value());
    QString atk = QString::number(ui->compattack->value());
    QString rel = QString::number(ui->comprelease->value());
    QString adapt = QString::number(ui->a_adapt->value());
    QString crest = QString::number(ui->a_crest->value());
    QString maxatk = QString::number(ui->a_maxatk->value());
    QString maxrel = QString::number(ui->a_maxrel->value());
    QString maxwidth = QString::number(ui->a_kneewidth->value());

    out += "fetcomp_enable=";
    if(ui->enable_comp->isChecked())out += "true" + n;
    else out += "false" + n;

    out += "fetcomp_autogain=";
    if(ui->m_gain->isChecked())out += "false" + n;
    else out += "true" + n;
    out += "fetcomp_autoknee=";
    if(ui->m_width->isChecked())out += "false" + n;
    else out += "true" + n;
    out += "fetcomp_autoattack=";
    if(ui->m_attack->isChecked())out += "false" + n;
    else out += "true" + n;
    out += "fetcomp_autorelease=";
    if(ui->m_release->isChecked())out += "false" + n;
    else out += "true" + n;

    out += "fetcomp_noclip=";
    if(ui->noclip->isChecked())out += "true" + n;
    else out += "false" + n;

    out += "fetcomp_threshold=";
    out += thres.toUtf8().constData() + n;
    out += "fetcomp_gain=";
    out += gain.toUtf8().constData() + n;
    out += "fetcomp_kneewidth=";
    out += width.toUtf8().constData() + n;
    out += "fetcomp_ratio=";
    out += ratio.toUtf8().constData() + n;
    out += "fetcomp_attack=";
    out += atk.toUtf8().constData() + n;
    out += "fetcomp_release=";
    out += rel.toUtf8().constData() + n;
    out += "fetcomp_meta_adapt=";
    out += adapt.toUtf8().constData() + n;
    out += "fetcomp_meta_crest=";
    out += crest.toUtf8().constData() + n;
    out += "fetcomp_meta_maxattack=";
    out += maxatk.toUtf8().constData() + n;
    out += "fetcomp_meta_maxrelease=";
    out += maxrel.toUtf8().constData() + n;
    out += "fetcomp_meta_kneemulti=";
    out += maxwidth.toUtf8().constData() + n;

    return out;
}
string MainWindow::getEQ() {
    string out;
    string n = "\n";
    QString eq1 = QString::number(ui->eq1->value());
    QString eq2 = QString::number(ui->eq2->value());
    QString eq3 = QString::number(ui->eq3->value());
    QString eq4 = QString::number(ui->eq4->value());
    QString eq5 = QString::number(ui->eq5->value());
    QString eq6 = QString::number(ui->eq6->value());
    QString eq7 = QString::number(ui->eq7->value());
    QString eq8 = QString::number(ui->eq8->value());
    QString eq9 = QString::number(ui->eq9->value());
    QString eq10 = QString::number(ui->eq10->value());

    out += "eq_enable=";
    if(ui->enable_eq->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "eq_band1=";
    out += eq1.toUtf8().constData() + n;
    out += "eq_band2=";
    out += eq2.toUtf8().constData() + n;
    out += "eq_band3=";
    out += eq3.toUtf8().constData() + n;
    out += "eq_band4=";
    out += eq4.toUtf8().constData() + n;
    out += "eq_band5=";
    out += eq5.toUtf8().constData() + n;
    out += "eq_band6=";
    out += eq6.toUtf8().constData() + n;
    out += "eq_band7=";
    out += eq7.toUtf8().constData() + n;
    out += "eq_band8=";
    out += eq8.toUtf8().constData() + n;
    out += "eq_band9=";
    out += eq9.toUtf8().constData() + n;
    out += "eq_band10=";
    out += eq10.toUtf8().constData() + n;

    return out;
}
string MainWindow::getBass() {
    string out;
    string n = "\n";

    int c = ui->vbgain->value();
    int d = ui->vbfreq->value();
    int e = ui->vbmode->value();

    char vbgain[sizeof(c)];
    sprintf(vbgain, "%d", c);
    char vbfreq[sizeof(d)];
    sprintf(vbfreq, "%d", d);
    char vbmode[sizeof(e)];
    sprintf(vbmode, "%d", e);

    //BASS
    out += "vb_enable=";
    if(ui->vb->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vb_mode=";
    out += vbmode + n;
    out += "vb_freq=";
    out += vbfreq + n;
    out += "vb_gain=";
    out += vbgain + n;
    return out;
}
string MainWindow::getSurround() {
    string out;
    string n = "\n";

    int a = ui->difflvl->value();
    int b = ui->vhplvl->value();
    int c = ui->roomsize->value();
    int d = ui->roomwidth->value();
    int e = ui->roomdamp->value();
    int f = ui->wet->value();
    int g = ui->dry->value();

    char difflvl[sizeof(a)];
    sprintf(difflvl, "%d", a);

    char vhplvl[sizeof(b)];
    sprintf(vhplvl, "%d", b);

    char re_roomsize[sizeof(c)];
    sprintf(re_roomsize, "%d", c);
    char re_width[sizeof(d)];
    sprintf(re_width, "%d", d);
    char re_damp[sizeof(e)];
    sprintf(re_damp, "%d", e);
    char re_wet[sizeof(f)];
    sprintf(re_wet, "%d", f);
    char re_dry[sizeof(g)];
    sprintf(re_dry, "%d", g);

    //HEADSET ENGINE
    out += "vhe_enable=";
    if(ui->vhp->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vhe_level=";
    out += vhplvl + n;

    //DIFFERENTIAL SOUND
    out += "ds_enable=";
    if(ui->diff->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "ds_level=";
    out += difflvl + n;

    //REVERB
    out += "reverb_enable=";
    if(ui->reverb->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "reverb_roomsize=";
    out += re_roomsize + n;
    out += "reverb_width=";
    out += re_width + n;
    out += "reverb_damp=";
    out += re_damp + n;
    out += "reverb_wet=";
    out += re_wet + n;
    out += "reverb_dry=";
    out += re_dry + n;

    return out;
}
string MainWindow::getMain() {
    string out;
    string n = "\n";

    int a = ui->colmwide->value();
    int b = ui->colmmidimg->value();
    int c = ui->colmdepth->value();
    int d = ui->vclvl->value();
    int e = ui->vcmode->value();

    char colmwide[sizeof(a)];
    sprintf(colmwide, "%d", a);
    char colmmidimg[sizeof(b)];
    sprintf(colmmidimg, "%d", b);
    char colmdepth[sizeof(c)];
    sprintf(colmdepth, "%d", c);
    char vclvl[sizeof(d)];
    sprintf(vclvl, "%d", d);
    char vcmode[sizeof(e)];
    sprintf(vcmode, "%d", e);

    //TUBE SIMULATOR
    out += "tube_enable=";
    if(ui->tubesim->checkState() == Qt::Unchecked)out += "false" + n;
    else out += "true" + n;

    //COLORFUL
    out += "colm_enable=";
    if(ui->colm->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "colm_widening=";
    out += colmwide + n;
    out += "colm_depth=";
    out += colmdepth + n;
    out += "colm_midimage=";
    out += colmmidimg + n;

    //CLARITY
    out += "vc_enable=";
    if(ui->clarity->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vc_mode=";
    out += vcmode + n;
    out += "vc_level=";
    out += vclvl + n;

    return out;
}
string MainWindow::getMaster() {
    string out;
    string n = "\n";

    int a = ui->gain->value();
    int b = ui->maxgain->value();
    int c = ui->maxvol->value();
    int d = ui->limiter->value();
    int e = ui->outputpan->value();
    int f = ui->outvolume->value();

    char gain[sizeof(a)];
    sprintf(gain, "%d", a);
    char maxgain[sizeof(b)];
    sprintf(maxgain, "%d", b);
    char maxvol[sizeof(c)];
    sprintf(maxvol, "%d", c);

    char limiter[sizeof(d)];
    sprintf(limiter, "%d", d);
    char outputpan[sizeof(e)];
    sprintf(outputpan, "%d", e);
    char out_volume[sizeof(f)];
    sprintf(out_volume, "%d", f);


    //GAIN CONTROL
    out += "agc_enable=";
    if(ui->agc->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "agc_ratio=";
    out += gain + n;
    out += "agc_maxgain=";
    out += maxgain + n;
    out += "agc_volume=";
    out += maxvol + n;

    //MASTER
    out += "lim_threshold=";
    out += limiter + n;
    out += "out_pan=";
    out += outputpan + n;
    out += "out_volume=";
    out += out_volume + n;

    return out;
}
string MainWindow::getDynsys() {
    string out;
    string n = "\n";

    //DYNAMIC SYSTEM
    out += "dynsys_enable=";
    if(ui->dynsys->isChecked())out += "true" + n;
    else out += "false" + n;

    out += "dynsys_bassgain=";
    out += to_string(ui->dyn_bassgain->value()) + n;
    out += "dynsys_xcoeff1=";
    out += to_string(ui->dyn_xcoeff1->value()) + n;
    out += "dynsys_xcoeff2=";
    out += to_string(ui->dyn_xcoeff2->value()) + n;
    out += "dynsys_ycoeff1=";
    out += to_string(ui->dyn_ycoeff1->value()) + n;
    out += "dynsys_ycoeff2=";
    out += to_string(ui->dyn_ycoeff2->value()) + n;
    out += "dynsys_sidegain1=";
    out += to_string(ui->dyn_sidegain1->value()) + n;
    out += "dynsys_sidegain2=";
    out += to_string(ui->dyn_sidegain2->value()) + n;
    return out;
}

//---EQ
void MainWindow::setEQ(const int* data){
    lockapply=true;
    ui->eq1->setValue(data[0]);
    ui->eq2->setValue(data[1]);
    ui->eq3->setValue(data[2]);
    ui->eq4->setValue(data[3]);
    ui->eq5->setValue(data[4]);
    ui->eq6->setValue(data[5]);
    ui->eq7->setValue(data[6]);
    ui->eq8->setValue(data[7]);
    ui->eq9->setValue(data[8]);
    ui->eq10->setValue(data[9]);
    lockapply=false;
    OnUpdate();
}
void MainWindow::updatepreset(){
    if(ui->eqpreset->currentText() == "Pop"){
        setEQ(std::initializer_list<int>({0,0,0,125,250,500,-150,-300,-300,-300}).begin());
    }
    else if(ui->eqpreset->currentText() == "Rock"){
        setEQ(std::initializer_list<int>({0,0,300,-1000,-150,75,300,300,300,300}).begin());
    }
    else if(ui->eqpreset->currentText() == "Jazz"){
        setEQ(std::initializer_list<int>({0,0,273,600,-600,-250,250,-75,-75,-75}).begin());
    }
    else if(ui->eqpreset->currentText() == "Classic"){
        setEQ(std::initializer_list<int>({0,0,-900,0,150,0,0,900,900,900}).begin());
    }
    else if(ui->eqpreset->currentText() == "Bass"){
        setEQ(std::initializer_list<int>({1150,850,500,200,0,0,0,0,0,0}).begin());
    }
    else if(ui->eqpreset->currentText() == "Clear"){
        setEQ(std::initializer_list<int>({350,650,950,650,350,125,500,900,1100,900}).begin());
    }
    else if(ui->eqpreset->currentText() == "Volume Boost"){
        setEQ(std::initializer_list<int>({1200,1200,1200,1200,1200,1200,1200,1200,1200,1200}).begin());
    }
    else if(ui->eqpreset->currentText() == "Hip-Hop"){
        setEQ(std::initializer_list<int>({450,400,150,300,-150,-150,150,-100,150,300}).begin());
    }
    else if(ui->eqpreset->currentText() == "Dubstep"){
        setEQ(std::initializer_list<int>({1200,50,-200,-500,-500,-450,-250,0,-300,-50}).begin());
    }
    else if(ui->eqpreset->currentText() == "Movie"){
        setEQ(std::initializer_list<int>({300,600,900,700,600,500,600,350,1050,800}).begin());
    }
    else if(ui->eqpreset->currentText() == "Metal"){
        setEQ(std::initializer_list<int>({1050,750,0,550,0,0,600,0,900,1200}).begin());
    }
    else if(ui->eqpreset->currentText() == "Vocal Booster"){
        setEQ(std::initializer_list<int>({-150,-300,-300,150,350,350,300,150,0,-150}).begin());
    }
    else if(ui->eqpreset->currentText() == "Hardstyle"){
        setEQ(std::initializer_list<int>({600,1200,0,-1200,300,650,0,-450,-800,-1050}).begin());
    }
    else if(ui->eqpreset->currentText() == "Acoustic"){
        setEQ(std::initializer_list<int>({500,450,350,100,150,150,300,350,300,300}).begin());
    }
    else if(ui->eqpreset->currentText() == "R&B"){
        setEQ(std::initializer_list<int>({300,700,600,150,-200,-150,200,300,350,400}).begin());
    }
    else if(ui->eqpreset->currentText() == "Electronic"){
        setEQ(std::initializer_list<int>({400,350,50,-50,-200,150,0,50,300,450}).begin());
    }
    else if(ui->eqpreset->currentText() == "Deep Bass"){
        setEQ(std::initializer_list<int>({1200,0,-1200,-900,-350,-600,0,-500,0,300}).begin());
    }
    else if(ui->eqpreset->currentText() == "Beats"){
        setEQ(std::initializer_list<int>({-550,-450,-400,-300,-150,0,0,0,0,0}).begin());
    }
    else if(ui->eqpreset->currentText() == "Soft Bass"){
        setEQ(std::initializer_list<int>({1200,1034,900,700,600,500,600,350,1050,800}).begin());
    }
    else ResetEQ();
}
void MainWindow::ResetEQ(){
    ui->eq1->setValue(0);
    ui->eq2->setValue(0);
    ui->eq3->setValue(0);
    ui->eq3->setValue(0);
    ui->eq4->setValue(0);
    ui->eq5->setValue(0);
    ui->eq6->setValue(0);
    ui->eq7->setValue(0);
    ui->eq8->setValue(0);
    ui->eq9->setValue(0);
    ui->eq10->setValue(0);
}
void MainWindow::CopyEQ(){
    string s = to_string(ui->eq1->value()) + "," + to_string(ui->eq2->value()) + "," + to_string(ui->eq3->value()) + "," + to_string(ui->eq4->value()) + "," + to_string(ui->eq5->value()) + ",";
    s += to_string(ui->eq6->value()) + "," + to_string(ui->eq7->value()) + "," + to_string(ui->eq8->value()) + "," + to_string(ui->eq9->value()) + "," + to_string(ui->eq10->value());
    QClipboard* a = app->clipboard();
    a->setText(QString::fromStdString(s));
}
void MainWindow::PasteEQ(){
    QClipboard* a = app->clipboard();
    std::string str = a->text().toUtf8().constData();
    std::vector<int> vect;
    std::stringstream ss(str);
    int i;
    while (ss >> i)
    {
        vect.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }
    for (i=0; i< vect.size(); i++)
        std::cout << vect.at(i)<<std::endl;
    int data[100];
    std::copy(vect.begin(), vect.end(), data);
    setEQ(data);
}

//---Dynsys
void MainWindow::updatedynpreset(){
    QString selection = ui->dynsys_preset->currentText();
    if(selection == "Extreme Headphone (v2)"){
        setDynsys(std::initializer_list<int>({140,6200,40,60,10,80}).begin());
    }
    else if(selection == "High-end Headphone (v2)"){
        setDynsys(std::initializer_list<int>({180,5800,55,80,10,70}).begin());
    }
    else if(selection == "Common Headphone (v2)"){
        setDynsys(std::initializer_list<int>({300,5600,60,105,10,50}).begin());
    }
    else if(selection == "Low-end Headphone (v2)"){
        setDynsys(std::initializer_list<int>({600,5400,60,105,10,20}).begin());
    }
    else if(selection == "Common Earphone (v2)"){
        setDynsys(std::initializer_list<int>({100,5600,40,80,50,50}).begin());
    }
    else if(selection == "Extreme Headphone (v1)"){
        setDynsys(std::initializer_list<int>({1200,6200,40,80,0,20}).begin());
    }
    else if(selection == "High-end Headphone (v1)"){
        setDynsys(std::initializer_list<int>({1000,6200,40,80,0,10}).begin());
    }
    else if(selection == "Common Headphone (v1)"){
        setDynsys(std::initializer_list<int>({800,6200,40,80,10,0}).begin());
    }
    else if(selection == "Common Earphone (v1)"){
        setDynsys(std::initializer_list<int>({400,6200,40,80,10,0}).begin());
    }
    else if(selection == "Apple Earphone"){
        setDynsys(std::initializer_list<int>({1200,6200,50,90,15,10}).begin());
    }
    else if(selection == "Monster Earphone"){
        setDynsys(std::initializer_list<int>({1000,6200,50,90,30,10}).begin());
    }
    else if(selection == "Moto Earphone"){
        setDynsys(std::initializer_list<int>({1100,6200,60,100,20,0}).begin());
    }
    else if(selection == "Philips Earphone"){
        setDynsys(std::initializer_list<int>({1200,6200,50,100,10,50}).begin());
    }
    else if(selection == "SHP2000"){
        setDynsys(std::initializer_list<int>({1200,6200,60,100,0,30}).begin());
    }
    else if(selection == "SHP9000"){
        setDynsys(std::initializer_list<int>({1200,6200,40,80,0,30}).begin());
    }
    else if(selection == "Unknown Type I"){
        setDynsys(std::initializer_list<int>({1000,6200,60,100,0,0}).begin());
    }
    else if(selection == "Unknown Type II"){
        setDynsys(std::initializer_list<int>({1000,6200,60,120,0,0}).begin());
    }
    else if(selection == "Unknown Type III"){
        setDynsys(std::initializer_list<int>({1000,6200,80,140,0,0}).begin());
    }
    else if(selection == "Unknown Type IV"){
        setDynsys(std::initializer_list<int>({800,6200,80,140,0,0}).begin());
    }
}
void MainWindow::setDynsys(const int* data){
    lockapply=true;
    ui->dyn_xcoeff1->setValue(data[0]);
    ui->dyn_xcoeff2->setValue(data[1]);
    ui->dyn_ycoeff1->setValue(data[2]);
    ui->dyn_ycoeff2->setValue(data[3]);
    ui->dyn_sidegain1->setValue(data[4]);
    ui->dyn_sidegain2->setValue(data[5]);
    lockapply=false;
    OnUpdate();
}

//---Colorful
void MainWindow::updatecolmpreset(){
    QString selection = ui->colmpreset->text();
    if(selection == "Slight"){
        setColm(std::initializer_list<int>({120,200}).begin());
    }
    else if(selection == "Level 1"){
        setColm(std::initializer_list<int>({130,275}).begin());
    }
    else if(selection == "Level 2"){
        setColm(std::initializer_list<int>({140,350}).begin());
    }
    else if(selection == "Level 3"){
        setColm(std::initializer_list<int>({150,425}).begin());
    }
    else if(selection == "Level 4"){
        setColm(std::initializer_list<int>({160,500}).begin());
    }
    else if(selection == "Level 5"){
        setColm(std::initializer_list<int>({170,575}).begin());
    }
    else if(selection == "Level 6"){
        setColm(std::initializer_list<int>({180,650}).begin());
    }
    else if(selection == "Level 7"){
        setColm(std::initializer_list<int>({190,725}).begin());
    }
    else if(selection == "Extreme"){
        setColm(std::initializer_list<int>({200,800}).begin());
    }
}
void MainWindow::setColm(const int* data){
    lockapply=true;
    ui->colmwide->setValue(data[0]);
    ui->colmdepth->setValue(data[1]);
    lockapply=false;
    OnUpdate();
}

//---Updates Unit-Label
void MainWindow::update(int d){
    if(lockapply)return;//Skip if lockapply-flag is set (when setting presets, ...)
    QObject* obj = sender();
    QString pre = "";
    QString post = "";
    if(obj==ui->vbmode){
        //Bass
        if(d==0) ui->info->setText("Natural Bass");
        else if(d==1) ui->info->setText("Pure Bass+");
        else if(d==2) ui->info->setText("Subwoofer");
        else ui->info->setText("Mode "+QString::number( d ));
    }
    else if(obj==ui->vcmode){
        //Clarity
        if(d==0) ui->info->setText("Natural");
        else if(d==1) ui->info->setText("OZone+");
        else if(d==2) ui->info->setText("XHiFi");
        else ui->info->setText("Mode "+QString::number( d ));
    }
    else if(obj==ui->gain){
        //AGC
        if(d < 50) ui->info->setText("Very Slight (" + QString::number( d ) + ")");
        else if(d < 100) ui->info->setText("Slight (" + QString::number( d )+")");
        else if(d < 300) ui->info->setText("Moderate (" + QString::number( d )+")");
        else if(d >= 300) ui->info->setText("Extreme (" + QString::number( d )+")");
        else ui->info->setText(QString::number( d ));
    }
    else if(obj==ui->axmode){
        //AnalogX
        if(d==0) ui->info->setText("Slight");
        else if(d==1) ui->info->setText("Moderate");
        else if(d==2) ui->info->setText("Extreme");
        else ui->info->setText("Mode "+QString::number( d ));
    }
    else if(obj==ui->vcurelvl){
        //Cure+
        if(d==0) ui->info->setText("Slight");
        else if(d==1) ui->info->setText("Moderate");
        else if(d==2) ui->info->setText("Extreme");
        else ui->info->setText("Mode "+QString::number( d ));
    }
    //Dynsys
    else if(obj==ui->dyn_bassgain) ui->info->setText(QString::number( (d-100)/20 ) + "%");
    else if(obj==ui->eq1||obj==ui->eq2||obj==ui->eq3||obj==ui->eq4||obj==ui->eq5||obj==ui->eq6||obj==ui->eq7||obj==ui->eq8||obj==ui->eq9||obj==ui->eq10){
        updateeq(d);
    }
    //Diff-Surround
    else if(obj==ui->difflvl)ui->info->setText(QString::number(translate(d,0,100,0,20))+"ms (" + QString::number(d) + "%)");
    //AGC
    else if(obj==ui->maxgain)ui->info->setText(QString::number((int)translate(d,100,800,1,10))+"x (" + QString::number(d) + ")");
    //Bass
    else if(obj==ui->vbgain)ui->info->setText(QString::number(roundf(translate(d,0,600,0,17)*100)/100)+"dB (" + QString::number(d) + ")");
    //Clarity
    else if(obj==ui->vclvl)ui->info->setText(QString::number(roundf(translate(d,0,450,0,14.8)*100)/100)+"dB (" + QString::number(d) + ")");
    //Volume
    else if(obj==ui->outvolume)ui->info->setText(QString::number(roundf(translate(d,0,100,-40,0)*100)/100)+"dB (" + QString::number(d) + ")");
    //Headphone Engine
    else if(obj==ui->vhplvl)ui->info->setText("Level " + QString::number(d+1));
    //Reverb
    else if(obj==ui->roomsize){
        ui->info->setText(QString::number((int)translate(d,0,100,25,1200))+"m\u00B2 (" + QString::number(d) + ")");
    }
    else if(obj==ui->roomwidth){
        ui->info->setText(QString::number((int)translate(d,0,100,5,36))+"m (" + QString::number(d) + ")");
    }
    else{
        //Reverb
        if(obj==ui->roomdamp)post = "%";
        else if(obj==ui->wet)post = "%";
        else if(obj==ui->dry)post = "%";
        //Bass
        else if(obj==ui->vbfreq)post = "Hz";
        //Volume
        else if(obj==ui->limiter)post = "%";
        //Spectrum Expend
        else if(obj==ui->barkcon)pre = "Level ";
        else if(obj==ui->barkfreq)post = "Hz";
        //Convolver
        else if(obj==ui->convcc)post = "%";        
        //Compressor
        else if(obj==ui->comp_ratio)post = "%";
        else if(obj==ui->compgain)post = "%";
        else if(obj==ui->compwidth)post = "%";
        else if(obj==ui->comp_thres)post = "%";
        else if(obj==ui->compattack)post = "%";
        else if(obj==ui->comprelease)post = "%";
        else if(obj==ui->a_adapt)post = "%";
        else if(obj==ui->a_crest)post = "%";
        else if(obj==ui->a_maxrel)post = "%";
        else if(obj==ui->a_maxrel)post = "%";
        else if(obj==ui->a_kneewidth)post = "%";

        ui->info->setText(pre + QString::number(d) + post);
    }
    OnUpdate();
}
void MainWindow::updateeq(int f){
    QString pre = "";
    if(f < 0 ) pre = "-";
    char buffer[9];
    QString s;
    if(to_string(abs(f)%100).length()==1)
    {
        snprintf(buffer, sizeof(buffer), "%02d", abs(f)%100);
        s = pre + QString::number(abs(f)/100) + "."  + QString::fromUtf8(buffer) + "dB";
    }
    else{
        s = pre + QString::number(abs(f)/100) + "."  + QString::number(abs(f%100)) + "dB";
    }
    ui->info->setText(s);
}
float MainWindow::translate(int value,int leftMin,int leftMax,float rightMin,float rightMax){
    float leftSpan = leftMax - leftMin;
    float rightSpan = rightMax - rightMin;
    float valueScaled = float(value - leftMin) / float(leftSpan);
    return rightMin + (valueScaled * rightSpan);
}

//---Getter/Setter
bool MainWindow::getAutoFx(){
    return autofx;
}
void MainWindow::setAutoFx(bool afx){
    autofx = afx;
    SaveAppConfig();
}
bool MainWindow::getMuteOnRestart(){
    return muteOnRestart;
}
void MainWindow::setMuteOnRestart(bool on){
    muteOnRestart = on;
    SaveAppConfig();
}
void MainWindow::setIRS(const string& irs,bool apply){
    ui->convpath->setText(QString::fromStdString((irs)));
    qDebug() << "SET:" << QString::fromStdString((irs));
    if(apply)ConfirmConf();
}
void MainWindow::setGFix(bool f){
    glava_fix = f;
}
bool MainWindow::getGFix(){
    return glava_fix;
}
void MainWindow::setPath(string npath){
    path = std::move(npath);
    SaveAppConfig();
}
string MainWindow::getPath(){
    return path;
}
void MainWindow::setStylesheet(string s){
    style_sheet = std::move(s);
    SetStyle();
    SaveAppConfig();
}
string MainWindow::getStylesheet(){
    return style_sheet;
}
void MainWindow::setQStyle(string s){
    qstyle = std::move(s);
    SetStyle();
    SaveAppConfig();
}
string MainWindow::getQStyle(){
    return qstyle;
}
//---Connect UI-Signals
void MainWindow::ConnectActions(){
    connect(ui->apply, SIGNAL(clicked()), this, SLOT(ConfirmConf()));
    connect(ui->disableFX, SIGNAL(clicked()), this, SLOT(DisableFX()));
    connect(ui->reset_eq, SIGNAL(clicked()), this, SLOT(ResetEQ()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(Reset()));
    connect(ui->conv_select, SIGNAL(clicked()), this, SLOT(OpenConv()));
    connect(ui->copy_eq, SIGNAL(clicked()), this, SLOT(CopyEQ()));
    connect(ui->paste_eq, SIGNAL(clicked()), this, SLOT(PasteEQ()));

    connect(ui->cpreset, SIGNAL(clicked()), this, SLOT(OpenPreset()));
    connect(ui->set, SIGNAL(clicked()), this, SLOT(OpenSettings()));

    connect( ui->convcc , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->vbfreq , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->vbgain, SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->vbmode , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->difflvl , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->vhplvl , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->roomsize , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->roomwidth , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->roomdamp , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->wet , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->dry , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->colmwide , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->colmmidimg , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->colmdepth, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->vclvl, SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->vcmode, SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->gain , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->maxgain , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->maxvol , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->outputpan , SIGNAL(valueChanged(int)),this,  SLOT(update(int)));
    connect( ui->limiter , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->outvolume , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->vcurelvl, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->axmode, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->barkfreq, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->barkcon, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->comprelease, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->compgain, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->compwidth, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->comp_ratio, SIGNAL(valueChanged(int)),this,SLOT(update(int)));
    connect(ui->comp_thres, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->compattack, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->comprelease, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->a_adapt, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->a_crest, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->a_maxatk, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->a_maxrel, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->a_kneewidth, SIGNAL(valueChanged(int)),this, SLOT(update(int)));

    connect(ui->eq1, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq2, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq3, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq4, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq5, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq6, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq7, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq8, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq9, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eq10, SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect(ui->eqpreset, SIGNAL(currentIndexChanged(int)),this,SLOT(updatepreset()));

    connect( ui->vb , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->clarity , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->vcure , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->tubesim , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->vhp , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->diff , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->reverb , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->enable_eq , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->enable_comp , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->noclip , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->m_gain , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->m_width , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->m_attack , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->m_release , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->vb , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->clarity , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->vcure , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->tubesim , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->agc , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->colm , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->vse , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->conv , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->ax , SIGNAL(clicked()),this, SLOT(OnUpdate()));

    connect(ui->dynsys_preset, SIGNAL(currentIndexChanged(int)),this,SLOT(updatedynpreset()));
    connect( ui->dynsys , SIGNAL(clicked()),this, SLOT(OnUpdate()));
    connect( ui->dyn_xcoeff1 , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->dyn_xcoeff2 , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->dyn_ycoeff1 , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->dyn_ycoeff2 , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->dyn_bassgain , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->dyn_sidegain1 , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
    connect( ui->dyn_sidegain2 , SIGNAL(valueChanged(int)),this, SLOT(update(int)));
}

//---Helper
bool MainWindow::is_only_ascii_whitespace( const std::string& str )
{
    auto it = str.begin();
    do {
        if (it == str.end()) return true;
    } while (*it >= 0 && *it <= 0x7f && std::isspace(*(it++)));
    return false;
}
