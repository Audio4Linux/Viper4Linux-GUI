#ifndef MAIN
#define MAIN
#endif

#include "mainwindow.h"
#include "misc/findbinary.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QCommandLineParser>
#include <QDesktopWidget>
#include <QScreen>
#include <string>
#include <iostream>

using namespace std;
int main(int argc, char *argv[])
{
    findyourself_init(argv[0]);
    char exepath[PATH_MAX];
    find_yourself(exepath, sizeof(exepath));

    QApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("Graphical User Interface for Viper4Linux2");
    parser.addHelpOption();

    QCommandLineOption tray(QStringList() << "t" << "tray", "Start minimized in systray (forced)");
    parser.addOption(tray);
    QCommandLineOption sviper(QStringList() << "s" << "startviper", "Start viper on launch");
    parser.addOption(sviper);
    QCommandLineOption minst(QStringList() << "m" << "allow-multiple-instances", "Allow multiple instances of this app");
    parser.addOption(minst);
    parser.process(a);
    if(parser.isSet(sviper)) system("viper start");

    QApplication::setQuitOnLastWindowClosed( false );
    MainWindow w(QString::fromLocal8Bit(exepath),parser.isSet(tray),parser.isSet(minst));
    w.setFixedSize(w.geometry().width(),w.geometry().height());
    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            QGuiApplication::primaryScreen()->geometry()
        )
    );
    w.setWindowFlags(Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint);
    if(!parser.isSet(tray)) w.show();

    QApplication::setQuitOnLastWindowClosed( true );
    return QApplication::exec();
}
