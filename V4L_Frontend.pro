#-------------------------------------------------
#
# Project created by QtCreator 2019-01-11T21:34:04
#
#-------------------------------------------------

TARGET = V4L_Frontend
TEMPLATE = app
QMAKE_CXXFLAGS += "-Wno-old-style-cast -Wno-unused-function"

include(V4L_Frontend.pri)

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

SOURCES += main.cpp

LIBS += -ldl

unix {
    QMAKE_LFLAGS += -ldl -lutil
    QMAKE_CXXFLAGS += -g
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/bin/

# Set BUILDPATH variable to change the output path
# !isEmpty($$BUILDPATH): target.path = $$BUILDPATH
# !isEmpty($$BUILDPATH): DESTDIR = $$BUILDPATH

!isEmpty(target.path): INSTALLS += target
