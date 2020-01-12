#include "loghelper.h"

void LogHelper::writeLog(const QString& log,int mode){
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
    if(mode==0||mode==2)
        qDebug().noquote().nospace() << o_alt.toUtf8().constData() << endl;
}
void LogHelper::writeLogF(const QString& log,const QString& _path){
    QFile f(_path);
    QString o = "[" + QTime::currentTime().toString() + "] " + log;
    if (f.open(QIODevice::WriteOnly | QIODevice::Append)) {
        f.write(o.toUtf8().constData());
    }
    f.close();
}
void LogHelper::clearLog(){
    QFile file ("/tmp/viper4linux/ui.log");
    if(file.exists())file.remove();
    QFile file2 ("/tmp/viper4linux/ui_viper.log");
    if(file2.exists())file2.remove();
}
