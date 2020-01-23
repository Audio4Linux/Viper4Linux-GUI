#ifndef OVERLAYMSGPROXY_H
#define OVERLAYMSGPROXY_H

#include <QObject>

class OverlayMsgProxy : public QObject
{
    Q_OBJECT
public:
    OverlayMsgProxy();
    static void openBase(QWidget* obj, QString title, QString desc, QString icon);
    static void openError(QWidget* obj, QString title, QString desc);
    static void openNormal(QWidget* obj, QString title, QString desc);
};

#endif // OVERLAYMSGPROXY_H
