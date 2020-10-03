TARGET = Viper4Linux-GUI
TEMPLATE = lib
CONFIG += staticlib
DEFINES += VIPER_PLUGINMODE

CONFIG += c++11

include($$PWD/V4L_Frontend.pri)

QMAKE_CXXFLAGS += "-Wno-old-style-cast -Wno-unused-function"

LIBS += -ldl

unix {
    QMAKE_LFLAGS += -ldl -lutil
    QMAKE_CXXFLAGS += -g
    target.path = /usr/lib
}
else: error("Static linking only available on Linux systems")

!isEmpty(target.path): INSTALLS += target
