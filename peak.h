#ifndef PEAK_H
#define PEAK_H
#include <pulse/pulseaudio.h>
#include <QDialog>
#include <qscale.h>
#include <string>
namespace Ui {
class peak;
}

class peak : public QDialog
{
    Q_OBJECT

public:
    explicit peak(QWidget *parent = nullptr);
    ~peak();
    Ui::peak *ui;
private slots:
    static void pa_state_cb(pa_context *c, void *userdata);
    static void sink_info_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
    static void stream_read_cb(pa_stream *s, size_t length, void *userdata);
public slots:
    void reject();
protected:
    void timerEvent(QTimerEvent *event);
private:
    static char* deblank(char* input);
    void initPeak();
    int timerId;
    pa_threaded_mainloop* _mainloop;
    pa_mainloop_api* _mainloop_api;
    pa_context* context;
    int refreshspd = 2;
};

#endif // PEAK_H
