#-------------------------------------------------
#
# Project created by QtCreator 2019-01-11T21:34:04
#
#-------------------------------------------------

QT       += core gui xml network dbus svg multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = V4L_Frontend
TEMPLATE = app
QMAKE_CXXFLAGS += "-Wno-old-style-cast -Wno-unused-function"

include(visualization/SpectrumAudioViewer.pri)
include(phantom/phantom.pri)

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
    3rdparty/WAF/Animation/Animation.cpp \
    3rdparty/WAF/Animation/CircleFill/CircleFillAnimator.cpp \
    3rdparty/WAF/Animation/CircleFill/CircleFillDecorator.cpp \
    3rdparty/WAF/Animation/Expand/ExpandAnimator.cpp \
    3rdparty/WAF/Animation/Expand/ExpandDecorator.cpp \
    3rdparty/WAF/Animation/SideSlide/SideSlideAnimator.cpp \
    3rdparty/WAF/Animation/SideSlide/SideSlideDecorator.cpp \
    3rdparty/WAF/Animation/Slide/SlideAnimator.cpp \
    3rdparty/WAF/Animation/Slide/SlideForegroundDecorator.cpp \
    config/appconfigwrapper.cpp \
    config/container.cpp \
    config/dbusproxy.cpp \
    config/io.cpp \
    dbus/clientproxy.cpp \
    dbus/serveradaptor.cpp \
    dialog/androidimporterdlg.cpp \
    dialog/animatedvipericon.cpp \
    dialog/convolverdlg.cpp \
    dialog/firstlaunchwizard.cpp \
    dialog/liquidequalizerwidget.cpp \
    dialog/logdlg.cpp \
    dialog/palettedlg.cpp \
    dialog/presetdlg.cpp \
    dialog/pulseeffectscompatibility.cpp \
    dialog/qanimatedslider.cpp \
    dialog/qmenueditor.cpp \
    dialog/qmessageoverlay.cpp \
    dialog/settingsdlg.cpp \
    dialog/slidingstackedwidget.cpp \
    dialog/statusfragment.cpp \
    main.cpp \
    mainwindow.cpp \
    misc/autostartmanager.cpp \
    misc/biquad.cpp \
    misc/converter.cpp \
    misc/loghelper.cpp \
    misc/overlaymsgproxy.cpp \
    misc/presetprovider.cpp \
    misc/stylehelper.cpp

HEADERS += \
    3rdparty/WAF/AbstractAnimator.h \
    3rdparty/WAF/Animation/Animation.h \
    3rdparty/WAF/Animation/AnimationPrivate.h \
    3rdparty/WAF/Animation/CircleFill/CircleFillAnimator.h \
    3rdparty/WAF/Animation/CircleFill/CircleFillDecorator.h \
    3rdparty/WAF/Animation/Expand/ExpandAnimator.h \
    3rdparty/WAF/Animation/Expand/ExpandDecorator.h \
    3rdparty/WAF/Animation/SideSlide/SideSlideAnimator.h \
    3rdparty/WAF/Animation/SideSlide/SideSlideDecorator.h \
    3rdparty/WAF/Animation/Slide/SlideAnimator.h \
    3rdparty/WAF/Animation/Slide/SlideForegroundDecorator.h \
    3rdparty/WAF/WAF.h \
    config/appconfigwrapper.h \
    config/container.h \
    config/dbusproxy.h \
    config/io.h \
    dbus/clientproxy.h \
    dbus/serveradaptor.h \
    dialog/androidimporterdlg.h \
    dialog/animatedvipericon.h \
    dialog/convolverdlg.h \
    dialog/firstlaunchwizard.h \
    dialog/liquidequalizerwidget.h \
    dialog/logdlg.h \
    dialog/palettedlg.h \
    dialog/presetdlg.h \
    dialog/pulseeffectscompatibility.h \
    dialog/qanimatedslider.h \
    dialog/qmenueditor.h \
    dialog/qmessageoverlay.h \
    dialog/settingsdlg.h \
    dialog/slidingstackedwidget.h \
    dialog/statusfragment.h \
    mainwindow.h \
    misc/autostartmanager.h \
    misc/biquad.h \
    misc/common.h \
    misc/converter.h \
    misc/eventfilter.h \
    misc/findbinary.h \
    misc/initializableqmap.h \
    misc/loghelper.h \
    misc/mathfunctions.h \
    misc/overlaymsgproxy.h \
    misc/presetprovider.h \
    misc/stylehelper.h \
    misc/versioncontainer.h

FORMS += \
    dialog/convolver.ui \
    dialog/firstlaunchwizard.ui \
    dialog/importandroid.ui \
    dialog/log.ui \
    dialog/menueditor.ui \
    dialog/palettedlg.ui \
    dialog/preset.ui \
    dialog/pulseeffectscompatibility.ui \
    dialog/settings.ui \
    dialog/statusfragment.ui \
    mainwindow.ui

TRANSLATIONS += translations/lang_en.ts \
                translations/lang_de.ts

INCLUDEPATH += $$PWD/3rdparty/WAF

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin/

# Set BUILDPATH variable to change the output path
# !isEmpty($$BUILDPATH): target.path = $$BUILDPATH
# !isEmpty($$BUILDPATH): DESTDIR = $$BUILDPATH

!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    dbus/manifest.xml

RESOURCES += \
    resources.qrc \
    styles/styles.qrc
