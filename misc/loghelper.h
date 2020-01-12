#ifndef LOGHELPER_H
#define LOGHELPER_H
#include <QFile>
#include <QTime>
#include <QString>
#include <QDebug>

class LogHelper
{
public:
    static void writeLog(const QString& log,int mode = 0);
    static void writeLogF(const QString& log,const QString& _path);
    static void clearLog();
};

#endif // LOGHELPER_H
