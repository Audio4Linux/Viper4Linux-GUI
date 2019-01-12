#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std;

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
    ui->path->setText(result);

    connect(ui->apply, SIGNAL(clicked()), this, SLOT(ConfirmConf()));
    connect(ui->reset_eq, SIGNAL(clicked()), this, SLOT(ResetEQ()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::ConfirmConf(){
    cout << getEQ() << endl;

    QString qpath = ui->path->text();
    string path = qpath.toUtf8().constData();

    string config = "fx_enable=true\n";
    config += getMain();
    config += getBass();
    config += getSurround();
    config += getMaster();
    config += getEQ();
    config += getComp();
    config += "#PLACEHOLDER\n";
    config += "conv_enable=false\nconv_ir_path=\nconv_cc_level=0\nvse_enable=false\nvse_ref_bark=7600\nvse_bark_cons=10\ncure_enable=false\ncure_level=0\nax_enable=false\nax_mode=0\n";

    ofstream myfile(path);
    if (myfile.is_open())
    {
      myfile << config;
      myfile.close();
    }
    else cout << "Unable to open file";
    system("pactl set-sink-mute 0 1");
    system("viper restart");
    system("pactl set-sink-mute 0 0");

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
