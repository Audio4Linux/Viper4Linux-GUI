QT       += core gui xml network svg
!contains(DEFINES, VIPER_PLUGINMODE) {
    QT       += dbus multimedia
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/dialog/FlatTabWidget/FlatTabWidget.pri)

!contains(DEFINES, VIPER_PLUGINMODE) {
    include($$PWD/phantom/phantom.pri)
    include($$PWD/3rdparty/WAF/WAF.pri)
    include($$PWD/visualization/SpectrumAudioViewer.pri)
}

SOURCES += \
    $$PWD/config/appconfigwrapper.cpp \
    $$PWD/config/container.cpp \
    $$PWD/config/io.cpp \
    $$PWD/dialog/androidimporterdlg.cpp \
    $$PWD/dialog/animatedvipericon.cpp \
    $$PWD/dialog/convolverdlg.cpp \
    $$PWD/dialog/liquidequalizerwidget.cpp \
    $$PWD/dialog/logdlg.cpp \
    $$PWD/dialog/palettedlg.cpp \
    $$PWD/dialog/presetdlg.cpp \
    $$PWD/dialog/qanimatedslider.cpp \
                $$PWD/dialog/qmenueditor.cpp \
    $$PWD/dialog/qmessageoverlay.cpp \
    $$PWD/dialog/settingsdlg.cpp \
    $$PWD/dialog/slidingstackedwidget.cpp \
    $$PWD/misc/biquad.cpp \
    $$PWD/misc/converter.cpp \
    $$PWD/misc/loghelper.cpp \
    $$PWD/misc/overlaymsgproxy.cpp \
    $$PWD/misc/presetprovider.cpp \
    $$PWD/misc/stylehelper.cpp \
    $$PWD/viper_window.cpp

HEADERS += \
    $$PWD/config/appconfigwrapper.h \
    $$PWD/config/container.h \
    $$PWD/config/io.h \
    $$PWD/dialog/androidimporterdlg.h \
    $$PWD/dialog/animatedvipericon.h \
    $$PWD/dialog/convolverdlg.h \
    $$PWD/dialog/liquidequalizerwidget.h \
    $$PWD/dialog/logdlg.h \
    $$PWD/dialog/palettedlg.h \
    $$PWD/dialog/presetdlg.h \
    $$PWD/dialog/qanimatedslider.h \
        $$PWD/dialog/qmenueditor.h \
    $$PWD/dialog/qmessageoverlay.h \
    $$PWD/dialog/settingsdlg.h \
    $$PWD/dialog/slidingstackedwidget.h \
    $$PWD/misc/biquad.h \
    $$PWD/misc/common.h \
    $$PWD/misc/converter.h \
    $$PWD/misc/eventfilter.h \
    $$PWD/misc/initializableqmap.h \
    $$PWD/misc/loghelper.h \
    $$PWD/misc/mathfunctions.h \
    $$PWD/misc/overlaymsgproxy.h \
    $$PWD/misc/presetprovider.h \
    $$PWD/misc/stylehelper.h \
    $$PWD/misc/versioncontainer.h \
    $$PWD/viper_window.h

FORMS += \
    $$PWD/dialog/convolver.ui \
    $$PWD/dialog/importandroid.ui \
    $$PWD/dialog/log.ui \
    $$PWD/dialog/menueditor.ui \
    $$PWD/dialog/palettedlg.ui \
    $$PWD/dialog/preset.ui \
    $$PWD/dialog/settings.ui \
    $$PWD/viper_window.ui

!contains(DEFINES, VIPER_PLUGINMODE) {
    SOURCES += \
        $$PWD/crashhandler/airbag.c \
        $$PWD/dialog/firstlaunchwizard.cpp \
        $$PWD/dbus/clientproxy.cpp \
        $$PWD/dbus/serveradaptor.cpp \
        $$PWD/config/dbusproxy.cpp \
        $$PWD/dialog/statusfragment.cpp \
        $$PWD/misc/autostartmanager.cpp \

    HEADERS += \
        $$PWD/crashhandler/airbag.h \
        $$PWD/crashhandler/killer.h \
        $$PWD/crashhandler/safecall.h \
        $$PWD/crashhandler/stacktrace.h \
        $$PWD/misc/findbinary.h \
        $$PWD/dialog/firstlaunchwizard.h \
        $$PWD/dbus/clientproxy.h \
        $$PWD/dbus/serveradaptor.h \
        $$PWD/config/dbusproxy.h \
        $$PWD/dialog/statusfragment.h \
        $$PWD/misc/GstRegistryHelper.h \
        $$PWD/misc/autostartmanager.h

    FORMS += \
        $$PWD/dialog/firstlaunchwizard.ui \
        $$PWD/dialog/statusfragment.ui
}

TRANSLATIONS += $$PWD/translations/lang_en.ts \
                $$PWD/translations/lang_de.ts \
                $$PWD/translations/lang_zh_CN.ts \
                $$PWD/translations/lang_zh_HK.ts

RESOURCES += \
    $$PWD/styles/v4l_frontend_styles.qrc \
    $$PWD/v4l_frontend_resources.qrc

INCLUDEPATH += $$PWD

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gstreamer-1.0 gstreamer-plugins-bad-1.0
unix: PKGCONFIG += libpulse
