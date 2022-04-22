#include "viper_window.h"
#include "misc/findbinary.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QCommandLineParser>
#include <QDesktopWidget>
#include <QScreen>
#include <string>
#include <iostream>
#include <unistd.h>
#include <clocale>
#include <QLocale>
#include <QStyle>

#include <gst/gst.h>

#define FORCE_CRASH_HANDLER

#if defined(Q_OS_UNIX) && defined(QT_NO_DEBUG) || defined(FORCE_CRASH_HANDLER)
#define ENABLE_CRASH_HANDLER
#endif

#ifdef ENABLE_CRASH_HANDLER
#include "crashhandler/airbag.h"
#include "crashhandler/stacktrace.h"
#include <sys/stat.h>
#include <sys/time.h>

#include <config/appconfigwrapper.h>
void crash_handled(int fd){
    safe_printf(STDERR_FILENO, "Done! Crash report saved to /tmp/viper-gui.\n");
}
#endif

using namespace std;
int main(int argc, char *argv[])
{
    findyourself_init(argv[0]);
    char exepath[PATH_MAX];
    find_yourself(exepath, sizeof(exepath));

#ifdef ENABLE_CRASH_HANDLER
    EXECUTION_FILENAME = exepath;
    mkdir("/tmp/viper-gui/", S_IRWXU);
    int fd = safe_open_wo_fd("/tmp/viper-gui/crash.dmp");
    airbag_init_fd(fd,crash_handled,EXECUTION_FILENAME);
#endif

    gst_init(&argc, &argv);

    QApplication a(argc, argv);
    auto* appConf = new AppConfigWrapper();
    appConf->loadAppConfig();
    auto* translator = new QTranslator();
    translator->load("lang_" + appConf->getLanguage(), QLatin1String(":/translations"));
    appConf->deleteLater();

    QApplication::installTranslator(translator);

    QApplication::setFallbackSessionManagementEnabled(false);
    QCommandLineParser parser;
    parser.setApplicationDescription("Graphical User Interface for Viper4Linux2");
    parser.addHelpOption();

    QCommandLineOption tray(QStringList() << "t" << "tray", "Start minimized in systray delayed (forced)");
    parser.addOption(tray);
    QCommandLineOption nowtray(QStringList() << "i" << "instant-tray", "Start minimized in systray instantly (forced)");
    parser.addOption(nowtray);
    QCommandLineOption sviper(QStringList() << "s" << "startviper", "Start viper on launch");
    parser.addOption(sviper);
    QCommandLineOption minst(QStringList() << "m" << "allow-multiple-instances", "Allow multiple instances of this app");
    parser.addOption(minst);
    parser.process(a);

    if(parser.isSet(sviper)){
        system("viper start");
        usleep(300);
    }

    if(parser.isSet(tray)){
        //Delay startup to make sure that the DE is ready
        usleep(1000);
    }

    QLocale::setDefault(QLocale::c());
    std::setlocale(LC_NUMERIC, "C");
    
    bool startInTray = parser.isSet(tray) || parser.isSet(nowtray);

    QApplication::setQuitOnLastWindowClosed( false );
    QApplication::setStyle("Fusion");
    ViperWindow w(QString::fromLocal8Bit(exepath),
                 startInTray,
                 parser.isSet(minst));

   // w.setFixedSize(w.geometry().width(),w.geometry().height());
    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            QGuiApplication::primaryScreen()->geometry()
        )
    );
    w.setWindowFlags(Qt::WindowContextHelpButtonHint | Qt::WindowCloseButtonHint);
    w.show();
    w.setVisible(!startInTray);

    QApplication::setQuitOnLastWindowClosed( true );
    return QApplication::exec();
}
