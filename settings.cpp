#include "settings.h"
#include "ui_settings.h"
#include "main.h"
#include <string>
#include <iostream>
#include <fstream>
#include <QDialog>
#include <unistd.h>
#include <pwd.h>
#include <QCloseEvent>
#include <cstdio>
#include <cstdlib>
#include <pulse/pulseaudio.h>
#include <pulse/thread-mainloop.h>
#include <pulse/context.h>
#include <pulse/stream.h>

using namespace std;
static settings* obj;
settings::settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settings){
    ui->setupUi(this);
    obj = this;
    initPeak();
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    char result[100];
    strcpy(result,homedir);
    strcat(result,"/.config/viper4linux/audio.conf");
    string path = mainwin->getPath();
    if(path.empty()) ui->path->setText(QString::fromUtf8(result));
    else ui->path->setText(QString::fromStdString(path));
    ui->autofx->setChecked(mainwin->getAutoFx());
    ui->muteonrestart->setChecked(mainwin->getMuteOnRestart());
    connect(ui->save, SIGNAL(clicked()), this, SLOT(submit()));
}
settings::~settings(){
    delete ui;
}
void settings::submit(){
    mainwin->setPath(ui->path->text().toUtf8().constData());
    mainwin->setAutoFx(ui->autofx->isChecked());
    mainwin->setMuteOnRestart(ui->muteonrestart->isChecked());
    mainwin->UpdatePeakSource(ui->source->currentText().toUtf8().constData());
    this->close();
}
void settings::reject()
{
    mainwin->enableSetBtn(true);
    pa_threaded_mainloop_stop(_mainloop);
    pa_threaded_mainloop_free(_mainloop);
    QDialog::reject();
}
void settings::initPeak(){
    const char *name = "V4L Frontend PeakInfoViewer";
    _mainloop = pa_threaded_mainloop_new();
    _mainloop_api = pa_threaded_mainloop_get_api(_mainloop);
    context = pa_context_new(_mainloop_api, name);

    pa_context_set_state_callback(context, settings::pa_state_cb,nullptr);

    pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    pa_threaded_mainloop_start(_mainloop);
}

void settings::pa_state_cb(pa_context *c, void *userdata) {
    pa_context_state_t state;
    // int *pa_ready = userdata;
    state = pa_context_get_state(c);
    switch  (state) {
    // These are just here for reference
    case PA_CONTEXT_UNCONNECTED:
    case PA_CONTEXT_CONNECTING:
    case PA_CONTEXT_AUTHORIZING:
    case PA_CONTEXT_SETTING_NAME:
    default:
        break;
    case PA_CONTEXT_FAILED:
        std::cerr << "PA failed" << std::endl;
        break;
    case PA_CONTEXT_TERMINATED:
        std::cerr << "PA terminated" << std::endl;
        break;
    case PA_CONTEXT_READY:
        std::cout << "PA ready" << std::endl;
        pa_operation* o = pa_context_get_sink_info_list(c, settings::sink_info_cb, nullptr);
        pa_operation_unref(o);
        break;
    }
}

void settings::sink_info_cb(pa_context *c, const pa_sink_info *sink_info, int eol, void *userdata){
    if (!sink_info) return;
    std::cout << sink_info->name << std::endl;
    obj->ui->source->addItem(sink_info->name);
}
