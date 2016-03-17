
DEFINES += QML # for test

android | ios : DEFINES += QML

QT += bluetooth

contains(DEFINES,QML){
    include(QML.pri)
} else {
    include(Widgets.pri)
}

HEADERS += \
    mcvalues.h \
    bleinterface.h

SOURCES += \
    mcvalues.cpp \
    bleinterface.cpp
