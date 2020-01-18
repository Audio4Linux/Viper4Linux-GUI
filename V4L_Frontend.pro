#-------------------------------------------------
#
# Project created by QtCreator 2019-01-11T21:34:04
#
#-------------------------------------------------

QT       += core gui xml network dbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = V4L_Frontend
TEMPLATE = app
QMAKE_CXXFLAGS += "-Wno-old-style-cast -Wdouble-promotion"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    config/appconfigwrapper.cpp \
    config/container.cpp \
    config/dbusproxy.cpp \
    config/io.cpp \
    converter.cpp \
    convolverdlg.cpp \
    importandroid.cpp \
    items/configitem.cpp \
    logdlg.cpp \
        main.cpp \
        mainwindow.cpp \
    misc/autostartmanager.cpp \
    misc/loghelper.cpp \
    misc/statusdialog.cpp \
    misc/stylehelper.cpp \
    palette.cpp \
    presetdlg.cpp \
    settingsdlg.cpp

HEADERS += \
    config/appconfigwrapper.h \
    config/container.h \
    config/dbusproxy.h \
    config/io.h \
    converter.h \
    convolverdlg.h \
    importandroid.h \
    items/configitem.h \
    items/delegates.h \
    logdlg.h \
        mainwindow.h \
    misc/autostartmanager.h \
    misc/common.h \
    misc/findbinary.h \
    misc/loghelper.h \
    misc/mathfunctions.h \
    misc/presetextension.h \
    misc/statusdialog.h \
    misc/stylehelper.h \
    palette.h \
    presetdlg.h \
    settingsdlg.h

FORMS += \
    importandroid.ui \
    items/configitem.ui \
    log.ui \
        mainwindow.ui \
    misc/statusdialog.ui \
    palette.ui \
    settings.ui \
    convolver.ui \
    preset.ui

TRANSLATIONS += translations/lang_en.ts \
                translations/lang_de.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin/

# Set BUILDPATH variable to change the output path
!isEmpty($$BUILDPATH): target.path = $$BUILDPATH
!isEmpty($$BUILDPATH): DESTDIR = $$BUILDPATH

!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    translations/lang_de.ts

RESOURCES += \
    resources.qrc \
    styles/styles.qrc
