#include "loghelper.h"

void LogHelper::writeLog(const QString& log,int mode){
    //Mode: 0-Log+Stdout 1-Log 2-Stdout
    QString path;

#ifndef VIPER_PLUGINMODE
    path = "/tmp/viper4linux/ui.log";
#else
    path = "/tmp/gsteffectmanager/viper_ui.log";
#endif

    QFile f(path);
    QString o = "[" + QTime::currentTime().toString() + "] " + log;

    if(mode==0||mode==1){
        if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
            f.write(QString("%1\n").arg(o).toUtf8().constData());
        }
        f.close();
    }
    if(mode==0||mode==2)
        qDebug().noquote().nospace() << o.toUtf8().constData();
}
void LogHelper::writeLogF(const QString& log,const QString& _path){
    QFile f(_path);
    QString o = "[" + QTime::currentTime().toString() + "] " + log + "\n";
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        f.write(o.toUtf8().constData());
    }
    f.close();
}
void LogHelper::clearLog(){
    QString path;

#ifndef VIPER_PLUGINMODE
    path = "/tmp/viper4linux/ui.log";
#else
    path = "/tmp/gsteffectmanager/viper_ui.log";
#endif

    QFile file (path);
    if(file.exists())
        file.remove();
}
