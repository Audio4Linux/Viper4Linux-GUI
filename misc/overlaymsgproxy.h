#ifndef OVERLAYMSGPROXY_H
#define OVERLAYMSGPROXY_H

#include <QObject>

class OverlayMsgProxy : public QObject
{
    Q_OBJECT
public:
    OverlayMsgProxy();
    static void openBase(QWidget* obj, QString title, QString desc, QString icon = "", QString close = tr("Close"), QString color = "#d72828");
    static void openError(QWidget* obj, QString title, QString desc, QString close = tr("Close"));
    static void openNormal(QWidget* obj, QString title, QString desc, QString color = "#d72828");
};

#endif // OVERLAYMSGPROXY_H
