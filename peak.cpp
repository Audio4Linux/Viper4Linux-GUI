#include "peak.h"
#include "ui_peak.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <pulse/pulseaudio.h>
#include <pulse/thread-mainloop.h>
#include <pulse/context.h>
#include <pulse/stream.h>
#include "main.h"
#include <QCloseEvent>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <qscale.h>
#include <QLayout>
#include <QtConcurrent/QtConcurrent>
static char* SINKNAME = "";
#define RATE 50
static int oldp;
static int lcount;
static peak *peakobj;
static QScale* qscale;
peak::peak(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::peak)
{
    ui->setupUi(this);
    peakobj = this;
    qscale = new QScale();
    qscale->setMaximum(127);
    oldp = 0;
    lcount = 0;

    string temp = mainwin->getPeakSource();
    SINKNAME = new char [temp.length()+1];
    strcpy(SINKNAME, temp.c_str());

    srand(time(nullptr));

    QFuture<void> future = QtConcurrent::run(peakobj,&peak::initPeak);
    auto layout = new QVBoxLayout();

    layout->addWidget(qscale);
    ui->frame->setLayout(layout);

    timerId = startTimer(1000);
}
void peak::reject()
{
    pa_threaded_mainloop_stop(_mainloop);
    pa_threaded_mainloop_free(_mainloop);
    mainwin->enablePeakBtn(true);
    QDialog::reject();
}
peak::~peak()
{
    pa_threaded_mainloop_stop(_mainloop);
    pa_threaded_mainloop_free(_mainloop);
    killTimer(timerId);
    delete ui;
}
void peak::timerEvent(QTimerEvent *event)
{
    this->repaint();
}
void peak::initPeak(){
    const char *name = "V4L Frontend PeakViewer";
    _mainloop = pa_threaded_mainloop_new();
    _mainloop_api = pa_threaded_mainloop_get_api(_mainloop);
    context = pa_context_new(_mainloop_api, name);

    pa_context_set_state_callback(context, peak::pa_state_cb,nullptr);

    pa_context_connect(context, nullptr, PA_CONTEXT_NOFLAGS, nullptr);
    pa_threaded_mainloop_start(_mainloop);
}

void peak::pa_state_cb(pa_context *c, void *userdata) {
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
        std::cerr << "PulseAudio Context failed" << std::endl;
        break;
    case PA_CONTEXT_TERMINATED:
        std::cerr << "PulseAudio Context terminated" << std::endl;
        break;
    case PA_CONTEXT_READY:
        std::cout << "PulseAudio Context ready" << std::endl;
        pa_operation* o = pa_context_get_sink_info_list(c, peak::sink_info_cb, nullptr);
        pa_operation_unref(o);
        break;
    }
}


void peak::sink_info_cb(pa_context *c, const pa_sink_info *sink_info, int eol, void *userdata){
    if (!sink_info) return;
    if (SINKNAME[0] == '\0')strcpy(SINKNAME,sink_info->name);

    if (strstr(sink_info->name, deblank(SINKNAME)) != nullptr){
        std::cout << "Source loaded: " << sink_info->name << std::endl;
        pa_sample_spec samplespec = pa_sample_spec();
        samplespec.channels = 1;
        samplespec.format = PA_SAMPLE_U8;
        samplespec.rate = RATE;
        const pa_sample_spec* spec = &samplespec;
        const auto *index = &sink_info->index;
        pa_stream* pa_str = pa_stream_new(c, "V4L_Frontend_PeakView", spec, nullptr);
        pa_stream_set_read_callback(pa_str,peak::stream_read_cb,&index);
        pa_stream_connect_record(pa_str,sink_info->monitor_source_name,nullptr,PA_STREAM_PEAK_DETECT);
    }
}

void peak::stream_read_cb(pa_stream *s, size_t length, void *userdata){
    //cout << "C:" << lcount << endl;
    //if(refreshspd==0){
    //   peakobj->ui->lcd->display("OFF");
    //   peakobj->ui->vu->setValue(0);
    //   return;
    //}
    //if((lcount % refreshspd) == 0){
    const void* data = &userdata;
    pa_stream_peek(s, &data, &length);
    auto* cdata = (unsigned char*) data;
    int p = cdata[0] - 128;
    //std::cout << p << std::endl;
    if(p == oldp && p != 0){
        int num = 0;
        int upper = 1, lower = -1, count = 1;
        for (int i = 0; i < count; i++) {
            num = (rand() %
                   (upper - lower + 1)) + lower;
        }
        p = p + num;
    }
    //peakobj->ui->vu->setValue(p);
    qscale->setValue(p);
    //if(refreshspd==1)peakobj->ui->lcd->display("---");
    //else peakobj->ui->lcd->display(p);
    oldp = p;
    pa_stream_drop(s);
    //usleep(20000);
    //}
    //lcount++;
}
char* peak::deblank(char* input)
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
        if (input[i]!=' ')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}
