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
#include <QWhatsThis>
#include "log.h"
#include <QObject>
using namespace std;

static string path;
static string irs_path;
static string appcpath;
static string style_sheet;
static string color_palette;
static string custom_palette;
static int theme_mode = 0;
static int conv_deftab = 0;
static int autofxmode = 0;
static bool custom_whiteicons;
static bool autofx;
static bool muteOnRestart;
static bool glava_fix;
static bool settingsdlg_enabled=true;
static bool presetdlg_enabled=true;
static bool logdlg_enabled=true;
static bool lockapply = false;
static string activeirs = "";
static QProcess* process;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    process = new QProcess(this);
    connect (process, SIGNAL(readyReadStandardOutput()), this, SLOT(processProcOutput()));
    connect (process, SIGNAL(readyReadStandardError()), this, SLOT(processProcOutput()));

    //Clear log
    QFile file ("/tmp/viper4linux/ui.log");
    if(file.exists())file.remove();
    QFile file2 ("/tmp/viper4linux/ui_viper.log");
    if(file2.exists())file2.remove();

    writeLog("UI launched...");

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
    menu->addAction("Context Help", this,[this](){QWhatsThis::enterWhatsThisMode();});
    menu->addAction("Load from file", this,SLOT(LoadExternalFile()));
    menu->addAction("Save to file", this,SLOT(SaveExternalFile()));
    menu->addAction("View Logs", this,SLOT(OpenLog()));

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

//--Process/Logs
void MainWindow::processProcOutput(){
    QString out = process->readAllStandardOutput();
    QString err = process->readAllStandardError();
    QString logpath("/tmp/viper4linux/ui_viper.log");
    if(out!=""){
        //qDebug()<<out;
        writeLogF(out,logpath);
    }
    if(err!=""){
        //qDebug()<<err;
        writeLogF(err,logpath);
    }
}
void MainWindow::writeLog(const QString& log,int mode){
    //Mode: 0-Log+Stdout 1-Log 2-Stdout
    QFile f("/tmp/viper4linux/ui.log");
    QString o = "[" + QTime::currentTime().toString() + "] " + log + "\n";
    QString o_alt = "[" + QTime::currentTime().toString() + "] " + log;

    if(mode==0||mode==1){
        if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
          f.write(o.toUtf8().constData());
        }
        f.close();
    }
    if(mode==0||mode==2) cout << o_alt.toUtf8().constData() << endl;
}
void MainWindow::writeLogF(const QString& log,const QString& _path){
    QFile f(_path);
    QString o = "[" + QTime::currentTime().toString() + "] " + log;
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
      f.write(o.toUtf8().constData());
    }
    f.close();
}

//---Style
void MainWindow::SetStyle(){
    if(theme_mode==0){
        QApplication::setPalette(this->style()->standardPalette());
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
        ui->vb->setContentsMargins(4,4,4,4);
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
        loadIcons(false);
        if(color_palette=="dark"){
            QColor background = QColor(53,53,53);
            QColor foreground = Qt::white;
            QColor base = QColor(25,25,25);
            QColor selection = QColor(42, 130, 218);
            setPalette(base,background,foreground,selection,Qt::black);
        }else if(color_palette=="purple"){
            loadIcons(true);
            QColor background = QColor(26, 0, 25);
            QColor foreground = Qt::white;
            QColor base = QColor(23, 0, 19);
            QColor selection = QColor(42, 130, 218);
            setPalette(base,background,foreground,selection,Qt::black);
         }else if(color_palette=="gray"){
            loadIcons(true);
            QColor background = QColor(49,49,74);
            QColor foreground = Qt::white;
            QColor base = QColor(83,83,125);
            QColor selection = QColor(85,85,127);
            setPalette(base,background,foreground,selection,Qt::black,QColor(144,144,179));
         }else if(color_palette=="white"){
            QColor background = Qt::white;
            QColor foreground = Qt::black;
            QColor base = Qt::white;
            QColor selection = QColor(56,161,227);
            setPalette(base,background,foreground,selection,Qt::black);
         }
        else if(color_palette=="blue"){
            loadIcons(true);
            QColor background = QColor(0,0,50);
            QColor foreground = Qt::white;
            QColor base = QColor(0,0,38);
            QColor selection = QColor(85,0,255);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="darkblue"){
            loadIcons(true);
            QColor background = QColor(19,25,38);
            QColor foreground = Qt::white;
            QColor base = QColor(14,19,29);
            QColor selection = QColor(70,79,89);
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="honeycomb"){
                    QColor background = QColor(212,215,208);
                    QColor foreground = Qt::black;
                    QColor base = QColor(185,188,182);
                    QColor selection = QColor(243,193,41);
                    setPalette(base,background,foreground,selection,Qt::white);
        }
        else if(color_palette=="black"){
                    loadIcons(true);
                    QColor background = QColor(16,16,16);
                    QColor foreground = QColor(222,222,222);
                    QColor base = Qt::black;
                    QColor selection = QColor(132,132,132);
                    setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="solarized"){
                    loadIcons(true);
                    QColor background = QColor(15,30,49);
                    QColor foreground = QColor(154,174,180);
                    QColor base = Qt::black;
                    QColor selection = QColor(3,50,63);
                    setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="silver"){
            QColor background = QColor(176,180,196);
            QColor foreground = QColor(20,20,20);
            QColor base = QColor(176,180,196);
            QColor selection = Qt::white;
            setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="darkgreen"){
                    loadIcons(true);
                    QColor background = QColor(27,34,36);
                    QColor foreground = QColor(197,209,217);
                    QColor base = QColor(30,30,30);
                    QColor selection = QColor(21,67,58);
                    setPalette(base,background,foreground,selection,Qt::black);
        }
        else if(color_palette=="custom"){
            QColor base = QColor(loadColor(0,0),loadColor(0,1),loadColor(0,2));
            QColor background = QColor(loadColor(1,0),loadColor(1,1),loadColor(1,2));
            QColor foreground = QColor(loadColor(2,0),loadColor(2,1),loadColor(2,2));
            QColor selection = QColor(loadColor(3,0),loadColor(3,1),loadColor(3,2));
            QColor disabled = QColor(loadColor(4,0),loadColor(4,1),loadColor(4,2));
            QColor selectiontext = QColor(255-loadColor(3,0),255-loadColor(3,1),255-loadColor(3,2));

            setPalette(base,background,foreground,selection,selectiontext,disabled);
            loadIcons(getWhiteIcons());
        }
        else{
            QApplication::setPalette(this->style()->standardPalette());
            QFile f(":/default.qss");
            if (!f.exists())printf("Unable to set stylesheet, file not found\n");
            else
            {
                f.open(QFile::ReadOnly | QFile::Text);
                QTextStream ts(&f);
                qApp->setStyleSheet(ts.readAll());
            }
        }

    }
}
void MainWindow::setPalette(const QColor& base,const QColor& background,const QColor& foreground,const QColor& selection = QColor(42,130,218),const QColor& selectiontext = Qt::black,const QColor& disabled){
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Window, background);
    palette->setColor(QPalette::WindowText, foreground);
    palette->setColor(QPalette::Base, base);
    palette->setColor(QPalette::AlternateBase, background);
    palette->setColor(QPalette::ToolTipBase, background);
    palette->setColor(QPalette::ToolTipText, foreground);
    palette->setColor(QPalette::Text, foreground);
    palette->setColor(QPalette::Button, background);
    palette->setColor(QPalette::ButtonText, foreground);
    palette->setColor(QPalette::BrightText, Qt::red);
    palette->setColor(QPalette::Link, QColor(42, 130, 218));
    palette->setColor(QPalette::Highlight, selection);
    palette->setColor(QPalette::HighlightedText, selectiontext);
    app->setPalette(*palette);
    QString rgbdisabled = disabled.name();
    app->setStyleSheet("QFrame[frameShape=\"4\"], QFrame[frameShape=\"5\"]{ color: gray; }*::disabled { color: " + rgbdisabled +";}QToolButton::disabled { color: " + rgbdisabled +";}QComboBox::disabled { color: " + rgbdisabled +";}");
}
void MainWindow::loadIcons(bool white){
    if(white){
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
int MainWindow::loadColor(int index,int rgb_index){
    QStringList elements = QString::fromStdString(mainwin->getCustompalette()).split(';');
    if(elements.length()<5||elements[index].split(',').size()<3){
        if(index==0)return 25;
        else if(index==1)return 53;
        else if(index==2)return 255;
        else if(index==3){
            if(rgb_index==0)return 42;
            else if(rgb_index==1)return 130;
            else if(rgb_index==2)return 218;
        }
        else if(index==4) return 85;
    }
    QStringList rgb = elements[index].split(',');
    return rgb[rgb_index].toInt();
}
void MainWindow::switchPalette(const QPalette& palette)
{
    app->setPalette(palette);

    QList<QWidget*> widgets = this->findChildren<QWidget*>();

    foreach (QWidget* w, widgets)
    {
        w->setPalette(palette);
    }
}

//---Dialogs
void MainWindow::OpenConv(){
    enableConvBtn(false);
    auto c = new Convolver(this);
    //c->setFixedSize(c->geometry().width(),c->geometry().height());
    c->show();
}
void MainWindow::OpenLog(){
    if(logdlg_enabled){
        enableLogBtn(false);
        auto c = new class log(this);
        //c->setFixedSize(c->geometry().width(),c->geometry().height());
        c->show();
    }
}
void MainWindow::OpenSettings(){
    if(settingsdlg_enabled){
        enableSetBtn(false);
        auto setting = new settings(this);
        //setting->setFixedSize(setting->geometry().width(),setting->geometry().height());
        setting->show();
    }
}
void MainWindow::OpenPreset(){
    if(presetdlg_enabled){
        enablePresetBtn(false);
        auto _preset = new Preset(this);
        preset = _preset;
        //_preset->setFixedSize(_preset->geometry().width(),_preset->geometry().height());
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
void MainWindow::enableLogBtn(bool on){
    logdlg_enabled=on;
}

//---Apply
void MainWindow::OnUpdate(bool ignoremode){
    //Will be called when slider has been moved, dynsys/eq preset set or spinbox changed
    if((autofx&&(ignoremode||autofxmode==0)) && !lockapply){
        ConfirmConf();
    }
}
void MainWindow::OnRelease(){
    if((autofx&&(autofxmode==1)) && !lockapply){
        ConfirmConf();
    }
}
void MainWindow::ConfirmConf(bool restart){
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
        //writeLog("Updating Viper Config... (main/writer)");
        myfile.close();
    }
    else writeLog("Unable to write to '" + QString::fromStdString(path) + "'; cannot update viper config (main/confirmconf)");
    if(restart)Restart();
}
void MainWindow::Reset(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Reset Configuration","Are you sure?",
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
    if(process->state()!=QProcess::ProcessState::NotRunning)return;
    if(muteOnRestart)system("pactl set-sink-mute 0 1");
    //process->kill();
    if(glava_fix)system("killall -r glava");
    process->start("viper", QStringList(initializer_list<QString>({"restart"})));
    if(glava_fix)system("setsid glava -d >/dev/null 2>&1 &");
    if(muteOnRestart)system("pactl set-sink-mute 0 0");

}
void MainWindow::DisableFX(){
    //Apply instantly
    if(!lockapply)ConfirmConf();
}

//---Save/Load Presets
void MainWindow::LoadPresetFile(const QString& filename){
    const QString& src = filename;
    const QString dest = QString::fromStdString(path);
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    writeLog("Loading from " + filename+ " (main/loadpreset)");
    reloadConfig();
    ConfirmConf();
}
void MainWindow::SavePresetFile(const QString& filename){
    const QString src = QString::fromStdString(path);
    const QString& dest = filename;
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    writeLog("Saving to " + filename+ " (main/savepreset)");
}
void MainWindow::LoadExternalFile(){
    QString filename = QFileDialog::getOpenFileName(this,"Load custom audio.conf","","*.conf");
    if(filename=="")return;
    const QString& src = filename;
    const QString dest = QString::fromStdString(path);
    if (QFile::exists(dest))QFile::remove(dest);

    QFile::copy(src,dest);
    writeLog("Loading from " + filename+ " (main/loadexternal)");
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
    writeLog("Saving to " + filename+ " (main/saveexternal)");
}

//---UI Config Loader
void MainWindow::decodeAppConfig(const string& key,const string& value){
    //cout << "AppConf: " << key << " -> " << value << endl;
    switch (resolveAppConfig(key)) {
    case unknownApp: {
        writeLog("Unable to resolve key: " + QString::fromStdString(key) + " (main/parser)");
        break;
    }
    case autoapply: {
        autofx = value=="true";
        break;
    }
    case autoapplymode: {
        autofxmode = atoi(value.c_str());
        break;
    }
    case configpath: {
        if(value.size() <= 2) break;
        path = value.substr(1, value.size() - 2);
        break;
    }
    case irspath: {
        if(value.size() <= 2) break;
        irs_path = value.substr(1, value.size() - 2);
        break;
    }
    case glavafix: {
        glava_fix = value=="true";
        break;
    }
    case customwhiteicons: {
        custom_whiteicons = value=="true";
        break;
    }
    case stylesheet: {
        style_sheet = value;
        break;
    }
    case thememode: {
        theme_mode = atoi(value.c_str());
        break;
    }
    case convolver_defaulttab: {
        if(value.empty()) conv_deftab = 0;
        else conv_deftab = atoi(value.c_str());
        break;
    }
    case colorpalette: {
        color_palette = value;
        break;
    }
    case custompalette: {
        custom_palette = value;
        break;
    }
    case mutedRestart: {
        muteOnRestart = value=="true";
        break;
    }
    }
}
void MainWindow::loadAppConfig(bool once){
    writeLog("Reloading UI-Config... (main/uiparser)");
    std::ifstream cFile(appcpath);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            if(line[0] == '#' || line.empty()) continue;
            auto delimiterPos = line.find('=');
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            decodeAppConfig(name,value);
        }
        cFile.close();
    }
    else {
        writeLog("Couldn't open UI-Config file for reading (main/uiparser))");
        ofstream outfile(appcpath);
        outfile << default_appconfig << endl;
        outfile.close();
        if(!once)loadAppConfig(true);
    }
}

//---UI Config Generator
void MainWindow::SaveAppConfig(bool afx = autofx, const string& cpath = path, bool muteRestart = muteOnRestart,bool g_fix = glava_fix, const string &ssheet = style_sheet,int tmode = theme_mode,const string &cpalette = color_palette,const string &custompal = custom_palette,bool w_ico = custom_whiteicons,int aamode=autofxmode,const string& ipath = irs_path,int c_deftab = conv_deftab){
    string appconfig;
    stringstream converter1;
    converter1 << boolalpha << afx;
    appconfig += "autoapply=" + converter1.str() + "\n";
    appconfig += "autoapplymode=" + to_string(aamode) + "\n";

    appconfig += "configpath=\"" + cpath + "\"\n";
    appconfig += "irspath=\"" + ipath + "\"\n";

    appconfig += "convolver_defaulttab=" + to_string(c_deftab) + "\n";

    stringstream converter2;
    converter2 << boolalpha << muteRestart;
    appconfig += "muteOnRestart=" + converter2.str() + "\n";

    stringstream converter3;
    converter3 << boolalpha << g_fix;
    appconfig += "glavafix=" + converter3.str() + "\n";

    appconfig += "stylesheet=" + ssheet + "\n";
    appconfig += "thememode=" + to_string(tmode) + "\n";

    appconfig += "colorpalette=" + cpalette + "\n";
    stringstream converter4;
    converter4 << boolalpha << w_ico;
    appconfig += "customwhiteicons=" + converter4.str() + "\n";
    appconfig += "custompalette=" + custompal + "\n";

    ofstream myfile(appcpath);
    if (myfile.is_open())
    {
        writeLog("Updating UI-Config... (main/uiwriter)");
        myfile << appconfig;
        myfile.close();
    }
    else{
        writeLog("Unable to write to file at '" + QString::fromStdString(appcpath) + "'; attempting to reloading ui-config... (main/uiwriter)");
        loadAppConfig();
    }
}

//---Viper Config Loader
void MainWindow::reloadConfig(){
    lockapply=true;
    writeLog("Reloading Viper Config... (main/parser)");
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
        writeLog("Unable to read file at '" + QString::fromStdString(path) + "'; viper config not found (main/parser)");
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

    if(value.empty()||is_only_ascii_whitespace(value)){
        mainwin->writeLog("Key " + QString::fromStdString(key) + " is empty (main/parser)");
        return;
    }
    int i = -1;
    if(is_number(value))i = stoi(value);
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
        update(i,ui->colmwide);
        break;
    }
    case colm_depth: {
        ui->colmdepth->setValue(std::stoi(value));
        update(i,ui->colmdepth);
        break;
    }
    case colm_midimage: {
        ui->colmmidimg->setValue(std::stoi(value));
        update(i,ui->colmmidimg);
        break;
    }
    case vc_enable: {
        if(value=="true") ui->clarity->setChecked(true);
        else ui->clarity->setChecked(false);
        break;
    }
    case vc_mode: {
        ui->vcmode->setValue(std::stoi(value));
        update(i,ui->vcmode);
        break;
    }
    case vc_level: {
        ui->vclvl->setValue(std::stoi(value));
        update(i,ui->vclvl);
        break;
    }
    case vb_enable: {
        if(value=="true") ui->vb->setChecked(true);
        else ui->vb->setChecked(false);
        break;
    }
    case vb_mode: {
        ui->vbmode->setValue(std::stoi(value));
        update(i,ui->vbmode);
        break;
    }
    case vb_freq: {
        ui->vbfreq->setValue(std::stoi(value));
        update(i,ui->vbfreq);
        break;
    }
    case vb_gain: {
        ui->vbgain->setValue(std::stoi(value));
        update(i,ui->vbgain);
        break;
    }
    case vhe_enable: {
        if(value=="true") ui->vhp->setChecked(true);
        else ui->vhp->setChecked(false);
        break;
    }
    case vhe_level: {
        ui->vhplvl->setValue(std::stoi(value));
        update(i,ui->vhplvl);
        break;
    }
    case ds_enable: {
        if(value=="true") ui->diff->setChecked(true);
        else ui->diff->setChecked(false);
        break;
    }
    case ds_level: {
        ui->difflvl->setValue(std::stoi(value));
        update(i,ui->difflvl);
        break;
    }
    case reverb_enable: {
        if(value=="true") ui->reverb->setChecked(true);
        else ui->reverb->setChecked(false);
        break;
    }
    case reverb_roomsize: {
        ui->roomsize->setValue(std::stoi(value));
        update(i,ui->roomsize);
        break;
    }
    case reverb_width: {
        ui->roomwidth->setValue(std::stoi(value));
        update(i,ui->roomwidth);
        break;
    }
    case reverb_damp: {
        ui->roomdamp->setValue(std::stoi(value));
        update(i,ui->roomdamp);
        break;
    }
    case reverb_wet: {
        ui->wet->setValue(std::stoi(value));
        update(i,ui->wet);
        break;
    }
    case reverb_dry: {
        ui->dry->setValue(std::stoi(value));
        update(i,ui->dry);
        break;
    }
    case agc_enable: {
        if(value=="true") ui->agc->setChecked(true);
        else ui->agc->setChecked(false);
        break;
    }
    case agc_ratio: {
        ui->gain->setValue(std::stoi(value));
        update(i,ui->gain);
        break;
    }
    case agc_maxgain: {
        ui->maxgain->setValue(std::stoi(value));
        update(i,ui->maxgain);
        break;
    }
    case agc_volume: {
        ui->maxvol->setValue(std::stoi(value));
        update(i,ui->maxvol);
        break;
    }
    case lim_threshold: {
        ui->limiter->setValue(std::stoi(value));
        update(i,ui->limiter);
        break;
    }
    case out_pan: {
        ui->outputpan->setValue(std::stoi(value));
        update(i,ui->outputpan);
        break;
    }
    case out_volume: {
        ui->outvolume->setValue(std::stoi(value));
        update(i,ui->outvolume);
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
        update(i,ui->comp_thres);
        break;
    }
    case fetcomp_gain: {
        ui->compgain->setValue(std::stoi(value));
        update(i,ui->compgain);
        break;
    }
    case fetcomp_kneewidth: {
        ui->compwidth->setValue(std::stoi(value));
        update(i,ui->compwidth);
        break;
    }
    case fetcomp_ratio: {
        ui->comp_ratio->setValue(std::stoi(value));
        update(i,ui->comp_ratio);
        break;
    }
    case fetcomp_attack: {
        ui->compattack->setValue(std::stoi(value));
        update(i,ui->compattack);
        break;
    }
    case fetcomp_release: {
        ui->comprelease->setValue(std::stoi(value));
        update(i,ui->comprelease);
        break;
    }
    case fetcomp_meta_adapt: {
        ui->a_adapt->setValue(std::stoi(value));
        update(i,ui->a_adapt);
        break;
    }
    case fetcomp_meta_crest: {
        ui->a_crest->setValue(std::stoi(value));
        update(i,ui->a_crest);
        break;
    }
    case fetcomp_meta_maxattack: {
        ui->a_maxatk->setValue(std::stoi(value));
        update(i,ui->a_maxatk);
        break;
    }
    case fetcomp_meta_maxrelease: {
        ui->a_maxrel->setValue(std::stoi(value));
        update(i,ui->a_maxrel);
        break;
    }
    case fetcomp_meta_kneemulti: {
        ui->a_kneewidth->setValue(std::stoi(value));
        update(i,ui->a_kneewidth);
        break;
    }
    case cure_enable: {
        if(value=="true") ui->vcure->setChecked(true);
        else ui->vcure->setChecked(false);
        break;
    }
    case cure_level: {
        ui->vcurelvl->setValue(std::stoi(value));
        update(i,ui->vcurelvl);
        break;
    }
    case ax_enable: {
        if(value=="true") ui->ax->setChecked(true);
        else ui->ax->setChecked(false);
        break;
    }
    case ax_mode: {
        ui->axmode->setValue(std::stoi(value));
        update(i,ui->axmode);
        break;
    }
    case vse_enable: {
        if(value=="true") ui->vse->setChecked(true);
        else ui->vse->setChecked(false);
        break;
    }
    case vse_ref_bark: {
        ui->barkfreq->setValue(std::stoi(value));
        update(i,ui->barkfreq);
        break;
    }
    case vse_bark_cons: {
        ui->barkcon->setValue(std::stoi(value));
        update(i,ui->barkcon);
        break;
    }
    case conv_enable: {
        if(value=="true") ui->conv->setChecked(true);
        else ui->conv->setChecked(false);
        break;
    }
    case conv_cc_level: {
        ui->convcc->setValue(std::stoi(value));
        update(i,ui->convcc);
        break;
    }
    case conv_ir_path: {
        if(value.size() <= 2) break;
        value = value.substr(1, value.size() - 2);
        QString ir = QString::fromStdString(value);
        activeirs = value;
        QFileInfo irsInfo(ir);
        ui->convpath->setText(irsInfo.baseName());
        ui->convpath->setCursorPosition(0);
        break;
    }
    case dynsys_enable: {
        if(value=="true") ui->dynsys->setChecked(true);
        else ui->dynsys->setChecked(false);
        break;
    }
    case dynsys_xcoeff1: {
        ui->dyn_xcoeff1->setValue(std::stoi(value));
        update(i,ui->dyn_xcoeff1);
        break;
    }
    case dynsys_xcoeff2: {
        ui->dyn_xcoeff2->setValue(std::stoi(value));
        update(i,ui->dyn_xcoeff2);
        break;
    }
    case dynsys_ycoeff1: {
        ui->dyn_ycoeff1->setValue(std::stoi(value));
        update(i,ui->dyn_ycoeff1);
        break;
    }
    case dynsys_ycoeff2: {
        ui->dyn_ycoeff2->setValue(std::stoi(value));
        update(i,ui->dyn_ycoeff2);
        break;
    }
    case dynsys_bassgain: {
        ui->dyn_bassgain->setValue(std::stoi(value));
        update(i,ui->dyn_bassgain);
        break;
    }
    case dynsys_sidegain1: {
        ui->dyn_sidegain1->setValue(std::stoi(value));
        update(i,ui->dyn_sidegain1);
        break;
    }
    case dynsys_sidegain2: {
        ui->dyn_sidegain2->setValue(std::stoi(value));
        update(i,ui->dyn_sidegain2);
        break;
    }
    case unknown: {
        writeLog("Unable to resolve key: " + QString::fromStdString(key)+ " (main/uiparser)");
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
    QString convir = QString::fromStdString(activeirs);

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
    //BASS
    out += "vb_enable=";
    if(ui->vb->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vb_mode=";
    out += to_string(ui->vbmode->value()) + n;
    out += "vb_freq=";
    out += to_string(ui->vbfreq->value()) + n;
    out += "vb_gain=";
    out += to_string(ui->vbgain->value()) + n;
    return out;
}
string MainWindow::getSurround() {
    string out;
    string n = "\n";
    //HEADSET ENGINE
    out += "vhe_enable=";
    if(ui->vhp->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vhe_level=";
    out += to_string(ui->vhplvl->value()) + n;

    //DIFFERENTIAL SOUND
    out += "ds_enable=";
    if(ui->diff->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "ds_level=";
    out += to_string(ui->difflvl->value()) + n;

    //REVERB
    out += "reverb_enable=";
    if(ui->reverb->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "reverb_roomsize=";
    out += to_string(ui->roomsize->value()) + n;
    out += "reverb_width=";
    out += to_string(ui->roomwidth->value()) + n;
    out += "reverb_damp=";
    out += to_string(ui->roomdamp->value()) + n;
    out += "reverb_wet=";
    out += to_string(ui->wet->value()) + n;
    out += "reverb_dry=";
    out += to_string(ui->dry->value()) + n;

    return out;
}
string MainWindow::getMain() {
    string out;
    string n = "\n";

    //TUBE SIMULATOR
    out += "tube_enable=";
    if(ui->tubesim->checkState() == Qt::Unchecked)out += "false" + n;
    else out += "true" + n;

    //COLORFUL
    out += "colm_enable=";
    if(ui->colm->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "colm_widening=";
    out += to_string(ui->colmwide->value()) + n;
    out += "colm_depth=";
    out += to_string(ui->colmdepth->value()) + n;
    out += "colm_midimage=";
    out += to_string(ui->colmmidimg->value()) + n;

    //CLARITY
    out += "vc_enable=";
    if(ui->clarity->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "vc_mode=";
    out += to_string(ui->vcmode->value()) + n;
    out += "vc_level=";
    out += to_string(ui->vclvl->value()) + n;

    return out;
}
string MainWindow::getMaster() {
    string out;
    string n = "\n";
    //GAIN CONTROL
    out += "agc_enable=";
    if(ui->agc->isChecked())out += "true" + n;
    else out += "false" + n;
    out += "agc_ratio=";
    out += to_string(ui->gain->value()) + n;
    out += "agc_maxgain=";
    out += to_string(ui->maxgain->value()) + n;
    out += "agc_volume=";
    out += to_string(ui->maxvol->value()) + n;

    //MASTER
    out += "lim_threshold=";
    out += to_string(ui->limiter->value()) + n;
    out += "out_pan=";
    out += to_string(ui->outputpan->value()) + n;
    out += "out_volume=";
    out += to_string(ui->outvolume->value()) + n;

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
    OnUpdate(true);
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
    OnUpdate(true);
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
    OnUpdate(true);
}

//---Updates Unit-Label
void MainWindow::update(int d,QObject *alt){
    if(lockapply&&alt==nullptr)return;//Skip if lockapply-flag is set (when setting presets, ...)
    QObject* obj;

    if(alt==nullptr)obj = sender();
    else obj = alt;

    QString pre = "";
    QString post = "";
    if(obj==ui->vbmode){
        //Bass
        if(d==0) updateWidgetUnit(obj,"Natural Bass",alt==nullptr);
        else if(d==1) updateWidgetUnit(obj,"Pure Bass+",alt==nullptr);
        else if(d==2) updateWidgetUnit(obj,"Subwoofer",alt==nullptr);
        else updateWidgetUnit(obj,"Mode "+QString::number( d ),alt==nullptr);
    }
    else if(obj==ui->vcmode){
        //Clarity
        if(d==0) updateWidgetUnit(obj,"Natural",alt==nullptr);
        else if(d==1) updateWidgetUnit(obj,"OZone+",alt==nullptr);
        else if(d==2) updateWidgetUnit(obj,"XHiFi",alt==nullptr);
        else updateWidgetUnit(obj,"Mode "+QString::number( d ),alt==nullptr);
    }
    else if(obj==ui->gain){
        //AGC
        if(d < 50) updateWidgetUnit(obj,"Very Slight (" + QString::number( d ) + ")",alt==nullptr);
        else if(d < 100) updateWidgetUnit(obj,"Slight (" + QString::number( d )+")",alt==nullptr);
        else if(d < 300) updateWidgetUnit(obj,"Moderate (" + QString::number( d )+")",alt==nullptr);
        else updateWidgetUnit(obj,"Extreme (" + QString::number( d )+")",alt==nullptr);
    }
    else if(obj==ui->axmode){
        //AnalogX
        if(d==0) updateWidgetUnit(obj,"Slight",alt==nullptr);
        else if(d==1) updateWidgetUnit(obj,"Moderate",alt==nullptr);
        else if(d==2) updateWidgetUnit(obj,"Extreme",alt==nullptr);
        else updateWidgetUnit(obj,"Mode "+QString::number( d ),alt==nullptr);
    }
    else if(obj==ui->vcurelvl){
        //Cure+
        if(d==0) updateWidgetUnit(obj,"Slight",alt==nullptr);
        else if(d==1) updateWidgetUnit(obj,"Moderate",alt==nullptr);
        else if(d==2) updateWidgetUnit(obj,"Extreme",alt==nullptr);
        else updateWidgetUnit(obj,"Mode "+QString::number( d ),alt==nullptr);
    }
    //Dynsys
    else if(obj==ui->dyn_bassgain) updateWidgetUnit(obj,QString::number( (d-100)/20 ) + "%",alt==nullptr);
    else if(obj==ui->eq1||obj==ui->eq2||obj==ui->eq3||obj==ui->eq4||obj==ui->eq5||obj==ui->eq6||obj==ui->eq7||obj==ui->eq8||obj==ui->eq9||obj==ui->eq10){
        updateeq(d);
    }
    //Diff-Surround
    else if(obj==ui->difflvl)updateWidgetUnit(obj,QString::number(translate(d,0,100,0,20))+"ms (" + QString::number(d) + "%)",alt==nullptr);
    //AGC
    else if(obj==ui->maxgain)updateWidgetUnit(obj,QString::number((int)translate(d,100,800,1,8))+"x (" + QString::number(d) + ")",alt==nullptr);
    else if(obj==ui->maxvol)updateWidgetUnit(obj,QString::number(roundf(translate(d,100,0,0,-30)*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    //Bass
    else if(obj==ui->vbgain){
        long double x = d;
        long double in =1.472577725 * pow(10L,-18L) * pow(x,7L) - 3.011526005 * pow(10L,-15L) * pow(x,6L) + 2.29923043 * pow(10L,-12L) * pow(x,5L) - 9.530124502 * pow(10L,-10L) * pow(x,4L) + 3.960377639 * pow(10L,-7L) * pow(x,3L) - 1.965034894 * pow(10L,-4L) * pow(x,2L) + 7.693150538 * pow(10L,-2L) * x + 1.965508847 * pow(10L,-2L);
        updateWidgetUnit(obj,QString::number(roundf(in*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    }
    //Clarity
    else if(obj==ui->vclvl)updateWidgetUnit(obj,QString::number(roundf(translate(d,0,450,0,14.8)*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    //Volume
    else if(obj==ui->outvolume){
        long double x = d;
        //Note:
        //V4A uses hardcoded arrays to display the output dB values when updating the slider in the app;
        //Although this makes it easier to implement, it is not really responsive.
        //Unlike the other sliders, this one increases exponentially rather than linearly, because of that I had to recreate a formula from the values in the v4a app, to calculate an accurate dB value (using polynomial regression; this might not be the most efficient solution).
        long double in = -7.095691001L* pow(10L,-13L) * pow(x,8L) + 3.130488467L* pow(10L,-10L) * pow(x,7L) - 5.667388779* pow(10L,-8L) * pow(x,6L) + 5.394863197L* pow(10L,-6L) * pow(x,5L) - 2.864305503L* pow(10L,-4L) * pow(x,4L)+ 8.264191247L* pow(10L,-3L) * pow(x,3L) - 1.218006784L* pow(10L,-1L) * pow(x,2L)+ 1.529341362L * x - 40.00317088L;
        updateWidgetUnit(obj,QString::number(roundf(in*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    }
    else if(obj==ui->limiter)updateWidgetUnit(obj,QString::number(roundf(translate(d,100,0,0,-30)*100)/100)+"dB (" + QString::number(d) + ")",alt==nullptr);
    //Headphone Engine
    else if(obj==ui->vhplvl)updateWidgetUnit(obj,"Level " + QString::number(d+1),alt==nullptr);
    //Reverb
    else if(obj==ui->roomsize){
        long double x = d;
        updateWidgetUnit(obj,QString::number(roundf(3.958333333* pow(10L,-7L) *pow(x,5L)- 4.106206294* pow(10L,-5L) *pow(x,4L)+ 1.189175408* pow(10L,-3L) *pow(x,3L)+ 4.16448133 * pow(10L,-3L) *pow(x,2L)+ 9.190238943 * pow(10L,-1L) * x+ 25.11013978))+"m\u00B2 (" + QString::number(d) + ")",alt==nullptr);
    }
    else if(obj==ui->roomwidth){
        long double x = d;
        long double in = -1.121794872 * pow(10L,-8L) * pow(x,5L) + 3.270687646 * pow(10L,-6L) * pow(x,4L) - 2.643502331 * pow(10L,-4L) * pow(x,3L) + 7.749854312 * pow(10L,-3L) * pow(x,2L) + 2.916958039 * pow(10L,-2L) * x+ 5.036713287;
        updateWidgetUnit(obj,QString::number(roundf(in))+"m (" + QString::number(d) + ")",alt==nullptr);
    }
    //Compressor
    else if(obj==ui->comp_thres)updateWidgetUnit(obj,DoCompressorMath(0,d),alt==nullptr);
    else if(obj==ui->comp_ratio)updateWidgetUnit(obj,DoCompressorMath(1,d),alt==nullptr);
    else if(obj==ui->compwidth)updateWidgetUnit(obj,DoCompressorMath(2,d),alt==nullptr);
    else if(obj==ui->compgain)updateWidgetUnit(obj,DoCompressorMath(3,d),alt==nullptr);
    else if(obj==ui->compattack)updateWidgetUnit(obj,DoCompressorMath(4,d),alt==nullptr);
    else if(obj==ui->comprelease)updateWidgetUnit(obj,DoCompressorMath(5,d),alt==nullptr);
    else if(obj==ui->a_kneewidth)updateWidgetUnit(obj,DoCompressorMath(6,d),alt==nullptr);
    else if(obj==ui->a_maxatk)updateWidgetUnit(obj,DoCompressorMath(7,d),alt==nullptr);
    else if(obj==ui->a_maxrel)updateWidgetUnit(obj,DoCompressorMath(8,d),alt==nullptr);
    else if(obj==ui->a_crest)updateWidgetUnit(obj,DoCompressorMath(9,d),alt==nullptr);
    else if(obj==ui->a_adapt)updateWidgetUnit(obj,DoCompressorMath(10,d),alt==nullptr);
    else{
        //Reverb
        if(obj==ui->roomdamp)post = "%";
        else if(obj==ui->wet)post = "%";
        else if(obj==ui->dry)post = "%";
        //Bass
        else if(obj==ui->vbfreq)post = "Hz";
        //Spectrum Expend
        else if(obj==ui->barkcon)pre = "Level ";
        else if(obj==ui->barkfreq)post = "Hz";
        //Convolver
        else if(obj==ui->convcc)post = "%";
        updateWidgetUnit(obj,pre + QString::number(d) + post,alt==nullptr);
    }
    if(!lockapply||obj!=nullptr)OnUpdate(false);
}
QString MainWindow::DoCompressorMath(int mode, float f){
    //Mode: 0-Threshold, 1-Ratio, 2-Knee, 3-Gain, 4-Atk, 5-Rel, 6-Kneemulti, 7-MaxAtk, 8-MaxRel, 9-Crest, 10-Adapt
    float orig = f;
    long double in;
    f = f/100;
    switch (mode) {
        case 0:
            in = log10(pow(10.0L, ((double) (-60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 1:
            if(f>0.99f)return "\u221E:1";
            in = 1.0L / (1.0L - ((double) f));
            return QString::number(roundf((in)*100)/100,'f',2) + ":1 (" + QString::number(orig) + ")";
        case 2:
            in = log10(pow(10.0L, ((double) (60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 3:
            in = log10(pow(10.0L, ((double) (60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 4:
            in = ((double) CompMathA(f, 1.0E-4f, 0.2f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        case 5:
            in = ((double) CompMathA(f, 0.005f, 2.0f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        case 6:
            in = ((double) CompMathB(f, 0.0f, 4.0f));
            return QString::number(roundf((in)*100)/100,'f',2) + "x (" + QString::number(orig) + ")";
        case 7:
            in = ((double) CompMathA(f, 1.0E-4f, 0.2f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
        case 8:
            in = ((double) CompMathA(f, 0.005f, 2.0f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";

        case 9:
            in = log10(pow(10.0L, ((double) (60.0f * f)) / 20.0L)) * 20.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "dB (" + QString::number(orig) + ")";
        case 10:
            in = ((double) CompMathA(f, 1.0f, 4.0f)) * 1000.0L;
            return QString::number(roundf((in)*100)/100,'f',2) + "ms (" + QString::number(orig) + ")";
     }
    return "E: Mode out of range";
}
float MainWindow::CompMathA(float f, float f2, float f3) {
    return (float) exp(std::log((double) f2) + (((double) f) * (std::log((double) f3) - std::log((double) f2))));
}
float MainWindow::CompMathB(float f, float f2, float f3) {
    return ((f3 - f2) * f) + f2;
}
void MainWindow::updateeq(int f){
    QString pre = "";
    if(f < 0 ) pre = "-";
    
    QString s;
    if(to_string(abs(f)%100).length()==1)
    {	
	char buffer[5];
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
void MainWindow::updateWidgetUnit(QObject* sender,QString text,bool viasignal){
    QWidget *w = qobject_cast<QWidget*>(sender);
    w->setToolTip(text);
    if(viasignal)ui->info->setText(text);
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
    activeirs = irs;
    QFileInfo irsInfo(QString::fromStdString(irs));
    ui->convpath->setText(irsInfo.baseName());
    ui->convpath->setCursorPosition(0);
    if(apply)ConfirmConf();
}
void MainWindow::setGFix(bool f){
    glava_fix = f;
    SaveAppConfig();
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
int MainWindow::getThememode(){
    return theme_mode;
}
void MainWindow::setThememode(int mode){
    //Modes:
    //  0 - Default/QSS
    //  1 - Color Palette
    theme_mode = mode;
    SetStyle();
    SaveAppConfig();
}
void MainWindow::setColorpalette(string s){
    color_palette = std::move(s);
    SetStyle();
    SaveAppConfig();
}
string MainWindow::getColorpalette(){
    return color_palette;
}
void MainWindow::setCustompalette(string s){
    custom_palette = std::move(s);
    SetStyle();
    SaveAppConfig();
}
string MainWindow::getCustompalette(){
    return custom_palette;
}
void MainWindow::setWhiteIcons(bool b){
    custom_whiteicons = b;
    SetStyle();
    SaveAppConfig();
}
bool MainWindow::getWhiteIcons(){
    return custom_whiteicons;
}
int MainWindow::getAutoFxMode(){
    return autofxmode;
}
void MainWindow::setAutoFxMode(int mode){
    autofxmode = mode;
    SaveAppConfig();
}
void MainWindow::setIrsPath(string npath){
    irs_path = std::move(npath);
    SaveAppConfig();
}
int MainWindow::getConv_DefTab(){
    return conv_deftab;
}
void MainWindow::setConv_DefTab(int mode){
    //Modes:
    //  0 - Fav
    //  1 - Filesys
    conv_deftab = mode;
    SaveAppConfig();
}

string MainWindow::getIrsPath(){
    if(irs_path.empty()){
        struct passwd *pw = getpwuid(getuid());
        const char *homedir = pw->pw_dir;
        char result[100];
        strcpy(result,homedir);
        strcat(result,"/IRS");
        return result;
    }
    return irs_path;
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


    connect( ui->convcc , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->vbfreq , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->vbgain, SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->vbmode , SIGNAL(valueChanged(int)),this,  SLOT(OnRelease()));
    connect( ui->difflvl , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->vhplvl , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->roomsize , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->roomwidth , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->roomdamp , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->wet , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->dry , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->colmwide , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->colmmidimg , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->colmdepth, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->vclvl, SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->vcmode, SIGNAL(valueChanged(int)),this,  SLOT(OnRelease()));
    connect( ui->gain , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->maxgain , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->maxvol , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->outputpan , SIGNAL(sliderReleased()),this,  SLOT(OnRelease()));
    connect( ui->limiter , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->outvolume , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->vcurelvl, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->axmode, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->barkfreq, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->barkcon, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->comprelease, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->compgain, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->compwidth, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->comp_ratio, SIGNAL(sliderReleased()),this,SLOT(OnRelease()));
    connect(ui->comp_thres, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->compattack, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->comprelease, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->a_adapt, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->a_crest, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->a_maxatk, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->a_maxrel, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->a_kneewidth, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));

    connect( ui->dyn_xcoeff1 , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->dyn_xcoeff2 , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->dyn_ycoeff1 , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->dyn_ycoeff2 , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->dyn_bassgain , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->dyn_sidegain1 , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect( ui->dyn_sidegain2 , SIGNAL(sliderReleased()),this, SLOT(OnRelease()));

    connect(ui->eq1, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq2, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq3, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq4, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq5, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq6, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq7, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq8, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq9, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
    connect(ui->eq10, SIGNAL(sliderReleased()),this, SLOT(OnRelease()));
}

//---Helper
bool MainWindow::is_number(const string& s)
{
    return !s.empty() && std::find_if(s.begin(),
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
bool MainWindow::is_only_ascii_whitespace( const std::string& str )
{
    auto it = str.begin();
    do {
        if (it == str.end()) return true;
    } while (*it >= 0 && *it <= 0x7f && std::isspace(*(it++)));
    return false;
}
