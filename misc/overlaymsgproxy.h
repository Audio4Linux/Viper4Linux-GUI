#ifndef OVERLAYMSGPROXY_H
#define OVERLAYMSGPROXY_H

#include <QObject>
#include <QPushButton>
#include "dialog/qmessageoverlay.h"

class OverlayMsgProxy : public QObject
{
    Q_OBJECT
public:
    OverlayMsgProxy(QWidget* obj);
    void openBase( QString title, QString desc, QString icon = "", QString close = tr("Close"), QString color = "#d72828");
    void openError(QString title, QString desc, QString close = tr("Close"));
    void openNormal(QString title, QString desc, QString color = "#d72828");
    void hide();
signals:
    void buttonPressed();
    void fadedOut();
private:
    QMessageOverlay *lightBox;
    QWidget* obj;
};

#endif // OVERLAYMSGPROXY_H
