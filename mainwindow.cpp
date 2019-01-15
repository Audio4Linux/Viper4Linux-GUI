#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "ui_settings.h"
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

using namespace std;
std::map<std::string, std::string> options;
string path;
string appcpath;
bool autofx;
bool muteOnRestart;

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

    loadAppConfig();
    reloadConfig();
    connect(ui->apply, SIGNAL(clicked()), this, SLOT(ConfirmConf()));
    connect(ui->reset_eq, SIGNAL(clicked()), this, SLOT(ResetEQ()));
    connect(ui->reset, SIGNAL(clicked()), this, SLOT(Reset()));
    connect(ui->restart, SIGNAL(clicked()), this, SLOT(Restart()));
    connect(ui->reload, SIGNAL(clicked()), this, SLOT(reloadConfig()));

    connect(ui->settingsBtn, SIGNAL(clicked()), this, SLOT(OpenSettings()));
    connect( ui->convcc , SIGNAL(valueChanged(int)),this, SLOT(updatecc()));
    connect( ui->vbfreq , SIGNAL(valueChanged(int)),this, SLOT(updatevbfreq()));
    connect( ui->vbmode , SIGNAL(valueChanged(int)),this, SLOT(updatevbmode()));
    connect( ui->difflvl , SIGNAL(valueChanged(int)),this, SLOT(updatedifflvl()));
    connect( ui->vhplvl , SIGNAL(valueChanged(int)),this, SLOT(updatevhplvl()));
    connect( ui->roomsize , SIGNAL(valueChanged(int)),this, SLOT(updateroomsize()));
    connect( ui->roomwidth , SIGNAL(valueChanged(int)),this, SLOT(updateroomwidth()));
    connect( ui->roomdamp , SIGNAL(valueChanged(int)),this, SLOT(updateroomdamp()));
    connect( ui->wet , SIGNAL(valueChanged(int)),this, SLOT(updatewet()));
    connect( ui->dry , SIGNAL(valueChanged(int)),this, SLOT(updatedry()));
    connect( ui->colmwide , SIGNAL(valueChanged(int)),this, SLOT(updatecolmwide()));
    connect( ui->colmmidimg , SIGNAL(valueChanged(int)),this, SLOT(updatecolmmidimg()));
    connect( ui->colmdepth, SIGNAL(valueChanged(int)),this, SLOT(updatecolmdepth()));
    connect( ui->vclvl, SIGNAL(valueChanged(int)),this, SLOT(updatevclvl()));
    connect( ui->vcmode, SIGNAL(valueChanged(int)),this, SLOT(updatevcmode()));
    connect( ui->gain , SIGNAL(valueChanged(int)),this, SLOT(updategain()));
    connect( ui->maxgain , SIGNAL(valueChanged(int)),this, SLOT(updatemaxgain()));
    connect( ui->maxvol , SIGNAL(valueChanged(int)),this, SLOT(updatemaxvol()));
    connect( ui->limiter , SIGNAL(valueChanged(int)),this, SLOT(updatelimiter()));
    connect( ui->outputpan , SIGNAL(valueChanged(int)),this, SLOT(updateoutputpan()));
    connect( ui->outvolume , SIGNAL(valueChanged(int)),this, SLOT(updateoutvolume()));
    connect(ui->vcurelvl, SIGNAL(valueChanged(int)),this, SLOT(updatevcurelvl()));
    connect(ui->axmode, SIGNAL(valueChanged(int)),this, SLOT(updateaxmode()));
    connect(ui->barkfreq, SIGNAL(valueChanged(int)),this, SLOT(updatebarkfreq()));
    connect(ui->barkcon, SIGNAL(valueChanged(int)),this, SLOT(updatebarkcon()));
    connect(ui->comprelease, SIGNAL(valueChanged(int)),this, SLOT(updatecomprelease()));
    connect(ui->compgain, SIGNAL(valueChanged(int)),this, SLOT(updatecompgain()));
    connect(ui->compwidth, SIGNAL(valueChanged(int)),this, SLOT(updatecompwidth()));
    connect(ui->comp_ratio, SIGNAL(valueChanged(int)),this, SLOT(updatecomp_ratio()));
    connect(ui->comp_thres, SIGNAL(valueChanged(int)),this, SLOT(updatecomp_thres()));
    connect(ui->compattack, SIGNAL(valueChanged(int)),this, SLOT(updatecompattack()));
    connect(ui->comprelease, SIGNAL(valueChanged(int)),this, SLOT(updatecomprelease()));
    connect(ui->a_adapt, SIGNAL(valueChanged(int)),this, SLOT(updatea_adapt()));
    connect(ui->a_crest, SIGNAL(valueChanged(int)),this, SLOT(updatea_crest()));
    connect(ui->a_maxatk, SIGNAL(valueChanged(int)),this, SLOT(updatea_maxatk()));
    connect(ui->a_maxrel, SIGNAL(valueChanged(int)),this, SLOT(updatea_maxrel()));
    connect(ui->a_kneewidth, SIGNAL(valueChanged(int)),this, SLOT(updatea_kneewidth()));
    connect(ui->eq1, SIGNAL(valueChanged(int)),this, SLOT(updateeq1()));
    connect(ui->eq2, SIGNAL(valueChanged(int)),this, SLOT(updateeq2()));
    connect(ui->eq3, SIGNAL(valueChanged(int)),this, SLOT(updateeq3()));
    connect(ui->eq4, SIGNAL(valueChanged(int)),this, SLOT(updateeq4()));
    connect(ui->eq5, SIGNAL(valueChanged(int)),this, SLOT(updateeq5()));
    connect(ui->eq6, SIGNAL(valueChanged(int)),this, SLOT(updateeq6()));
    connect(ui->eq7, SIGNAL(valueChanged(int)),this, SLOT(updateeq7()));
    connect(ui->eq8, SIGNAL(valueChanged(int)),this, SLOT(updateeq8()));
    connect(ui->eq9, SIGNAL(valueChanged(int)),this, SLOT(updateeq9()));
    connect(ui->eq10, SIGNAL(valueChanged(int)),this, SLOT(updateeq10()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::loadConfig(string key,string value){
    cout << key << " -> " << value << endl;
    switch (resolveConfig(key)) {
    case fx_enable: {
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
        ui->convpath->setText(ir);
        break;
    }
    case unknown: {
        cout << "Config-List Enum: Unknown" << endl;
        break;
    }
    }
}
void MainWindow::decodeAppConfig(string key,string value){
    cout << "AppConf: " << key << " -> " << value << endl;
    switch (resolveAppConfig(key)) {
    case autoapply: {
        if(value=="true") autofx=true;
        else autofx=false;
        break;
    }
    case configpath: {
        if(value.size() <= 2) break;
        path = value.substr(1, value.size() - 2);
        break;
    }
    case mutedRestart: {
        if(value=="true") muteOnRestart=true;
        else muteOnRestart=false;
        break;
    }
    }
}
void MainWindow::SaveAppConfig(bool afx = autofx, string cpath = path, bool muteRestart = muteOnRestart){
    string appconfig;
    stringstream converter1;
    converter1 << boolalpha << afx;
    appconfig += "autoapply=" + converter1.str() + "\n";
    appconfig += "configpath=\"" + cpath + "\"\n";

    stringstream converter2;
    converter2 << boolalpha << muteRestart;
    appconfig += "muteOnRestart=" + converter2.str() + "\n";

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

void MainWindow::ConfirmConf(){

    string config = "fx_enable=true\n";
    config += getMain();
    config += getBass();
    config += getSurround();
    config += getMaster();
    config += getEQ();
    config += getComp();
    config += getMisc();

    ofstream myfile(path);
    if (myfile.is_open())
    {
        myfile << config;
        myfile.close();
    }
    else cerr << "Unable to open file";

    if(muteOnRestart) system("pactl set-sink-mute 0 1");
    system("viper restart");
    if(muteOnRestart) system("pactl set-sink-mute 0 0");

}
void MainWindow::Restart(){
    if(muteOnRestart) system("pactl set-sink-mute 0 1");
    system("viper restart");
    if(muteOnRestart) system("pactl set-sink-mute 0 0");
}
void MainWindow::loadAppConfig(bool once){
    cout << "Reloading App Config..." << endl;
    std::ifstream cFile(appcpath);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
                       line.end());

            if(line[0] == '#' || line.empty() || line == "") continue;
            auto delimiterPos = line.find("=");
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
void MainWindow::OpenSettings(){
    auto setting = new settings(this);
    setting->setFixedSize(setting->geometry().width(),setting->geometry().height());
    setting->show();
}
string MainWindow::getPath(){
    return path;
}
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
void MainWindow::setPath(string npath){
    path = npath;
    reloadConfig();
    SaveAppConfig();
}

void MainWindow::Reset(){
    std::filebuf fb;
    fb.open (path,std::ios::out);
    std::ostream os(&fb);
    os << default_config;
    fb.close();

    cout << default_config << endl;
    reloadConfig();
    ConfirmConf();
}
void MainWindow::reloadConfig(){
    cout << "Reloading..." << endl;
    std::ifstream cFile(path);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
                       line.end());

            if(line[0] == '#' || line.empty() || line == "") continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            loadConfig(name,value);
        }
        cFile.close();
    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
        QMessageBox msgBox;
        msgBox.setText("Couldn't open config file for reading.");
        msgBox.setInformativeText("Change the path to the audio.conf file in the bottom left corner.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Icon::Critical);
        msgBox.exec();
    }
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
string MainWindow::getMisc(){
    string out;
    string n = "\n";

    QString vcurelvl = QString::number(ui->vcurelvl->value());
    QString axmode = QString::number(ui->axmode->value());

    QString refbark = QString::number(ui->barkfreq->value());
    QString conbark = QString::number(ui->barkcon->value());
    QString convcc = QString::number(ui->comprelease->value());
    QString convir = ui->convpath->text();

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
    out += colmdepth + n;

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

void MainWindow::updatevbfreq(){
    ui->info->setText(QString::number( ui->vbfreq->value() ));
    OnUpdate();
}
void MainWindow::updatevbmode(){
    ui->info->setText(QString::number( ui->vbmode->value() ));
    OnUpdate();
}
void MainWindow::updatedifflvl(){
    ui->info->setText(QString::number( ui->difflvl->value() ));
    OnUpdate();
}
void MainWindow::updatevhplvl(){
    ui->info->setText(QString::number( ui->vhplvl->value() ));
    OnUpdate();
}
void MainWindow::updateroomsize(){
    ui->info->setText(QString::number( ui->roomsize->value()));
    OnUpdate();
}
void MainWindow::updateroomwidth(){
    ui->info->setText(QString::number( ui->roomwidth->value() ));
    OnUpdate();
}
void MainWindow::updateroomdamp(){
    ui->info->setText(QString::number( ui->roomdamp->value() ));
    OnUpdate();
}
void MainWindow::updatewet(){
    ui->info->setText(QString::number( ui->wet->value() ));
    OnUpdate();
}
void MainWindow::updatedry(){
    ui->info->setText(QString::number( ui->dry->value() ));
    OnUpdate();
}
void MainWindow::updatecolmwide(){
    ui->info->setText(QString::number( ui->colmwide->value()));
    OnUpdate();
}
void MainWindow::updatecolmmidimg(){
    ui->info->setText(QString::number( ui->colmmidimg->value() ));
    OnUpdate();
}
void MainWindow::updatecolmdepth(){
    ui->info->setText(QString::number( ui->colmdepth->value() ));
    OnUpdate();
}
void MainWindow::updatevclvl(){
    ui->info->setText(QString::number( ui->vclvl->value() ));
    OnUpdate();
}
void MainWindow::updatevcmode(){
    ui->info->setText(QString::number( ui->vcmode->value() ));
    OnUpdate();
}
void MainWindow::updategain(){
    ui->info->setText(QString::number( ui->gain->value() ));
    OnUpdate();
}
void MainWindow::updatemaxgain(){
    ui->info->setText(QString::number( ui->maxgain->value() ));
    OnUpdate();
}
void MainWindow::updatemaxvol(){
    ui->info->setText(QString::number( ui->maxvol->value() ));
    OnUpdate();
}
void MainWindow::updatelimiter(){
    ui->info->setText(QString::number( ui->limiter->value() ));
    OnUpdate();
}
void MainWindow::updateoutputpan(){
    ui->info->setText(QString::number( ui->outputpan->value()));
    OnUpdate();
}
void MainWindow::updateoutvolume(){
    ui->info->setText(QString::number( ui->outvolume->value() ));
    OnUpdate();
}
void MainWindow::updatevcurelvl(){
    ui->info->setText(QString::number(ui->vcurelvl->value()));
    OnUpdate();
}
void MainWindow::updateaxmode(){
    ui->info->setText(QString::number(ui->axmode->value()));
    OnUpdate();
}
void MainWindow::updatebarkfreq(){
    ui->info->setText(QString::number(ui->barkfreq->value()));
    OnUpdate();
}
void MainWindow::updatebarkcon(){
    ui->info->setText(QString::number(ui->barkcon->value()));
    OnUpdate();
}
void MainWindow::updatecomprelease(){
    ui->info->setText(QString::number(ui->comprelease->value()));
    OnUpdate();
}
void MainWindow::updatecompgain(){
    ui->info->setText(QString::number(ui->compgain->value()));
    OnUpdate();
}
void MainWindow::updatecompwidth(){
    ui->info->setText(QString::number(ui->compwidth->value()));
    OnUpdate();
}
void MainWindow::updatecomp_ratio(){
    ui->info->setText(QString::number(ui->comp_ratio->value()));
    OnUpdate();
}
void MainWindow::updatecomp_thres(){
    ui->info->setText(QString::number(ui->comp_thres->value()));
    OnUpdate();
}
void MainWindow::updatecompattack(){
    ui->info->setText(QString::number(ui->compattack->value()));
    OnUpdate();
}
void MainWindow::updatea_adapt(){
    ui->info->setText(QString::number(ui->a_adapt->value()));
    OnUpdate();
}
void MainWindow::updatea_crest(){
    ui->info->setText(QString::number(ui->a_crest->value()));
    OnUpdate();
}
void MainWindow::updatea_maxatk(){
    ui->info->setText(QString::number(ui->a_maxatk->value()));
    OnUpdate();
}
void MainWindow::updatea_maxrel(){
    ui->info->setText(QString::number(ui->a_maxrel->value()));
    OnUpdate();
}
void MainWindow::updatea_kneewidth(){
    ui->info->setText(QString::number(ui->a_kneewidth->value()));
    OnUpdate();
}
void MainWindow::updatecc(){
    ui->info->setText(QString::number(ui->convcc->value()));
    OnUpdate();
}
void MainWindow::updateeq1(){
    ui->info->setText(QString::number(ui->eq1->value()));
    OnUpdate();
}
void MainWindow::updateeq2(){
    ui->info->setText(QString::number(ui->eq2->value()));
    OnUpdate();
}
void MainWindow::updateeq3(){
    ui->info->setText(QString::number(ui->eq3->value()));
    OnUpdate();
}
void MainWindow::updateeq4(){
    ui->info->setText(QString::number(ui->eq4->value()));
    OnUpdate();
}
void MainWindow::updateeq5(){
    ui->info->setText(QString::number(ui->eq5->value()));
    OnUpdate();
}
void MainWindow::updateeq6(){
    ui->info->setText(QString::number(ui->eq6->value()));
    OnUpdate();
}
void MainWindow::updateeq7(){
    ui->info->setText(QString::number(ui->eq7->value()));
    OnUpdate();
}
void MainWindow::updateeq8(){
    ui->info->setText(QString::number(ui->eq8->value()));
    OnUpdate();
}
void MainWindow::updateeq9(){
    ui->info->setText(QString::number(ui->eq9->value()));
    OnUpdate();
}
void MainWindow::updateeq10(){
    ui->info->setText(QString::number(ui->eq10->value()));
    OnUpdate();
}
void MainWindow::OnUpdate(){
    if(autofx)ConfirmConf();
}
