#ifndef SETTINGS_H
#define SETTINGS_H

#include <pulse/pulseaudio.h>
#include <QDialog>
#include <qscale.h>
#include "ui_settings.h"
class settings : public QDialog
{
    Q_OBJECT

public:
    settings(QWidget *parent = nullptr);
    Ui::settings *ui;
    ~settings();
private slots:
    static void pa_state_cb(pa_context *c, void *userdata);
    static void sink_info_cb(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
        void submit();
        void reject();
private:
        pa_threaded_mainloop* _mainloop{};
        pa_mainloop_api* _mainloop_api{};
        pa_context* context{};
        void initPeak();
};

#endif // SETTINGS_H
