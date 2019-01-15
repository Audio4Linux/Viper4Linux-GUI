#include "mainwindow.h"
#include "settings.h"
#include <QApplication>
#include <string>
#include <iostream>

using namespace std;
MainWindow *mainwin;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed( false );
    MainWindow w;
    settings s;
    mainwin = &w;
    w.setFixedSize(w.geometry().width(),w.geometry().height());
    w.show();
    a.setQuitOnLastWindowClosed( true );

    return a.exec();
}
