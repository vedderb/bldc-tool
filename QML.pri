TEMPLATE = app

QT += qml quick widgets
QT       += printsupport
QT       += serialport
QT       += network

SOURCES += main.cpp \
    lib-qt-qml-tricks/src/qqmlhelpers.cpp \
    digitalfiltering.cpp \
    packetinterface.cpp \
    utility.cpp \
    serialization.cpp \
    $$PWD/bldcinterface.cpp \
    $$PWD/mcconfiguration.cpp \
    $$PWD/appconfiguration.cpp

HEADERS += lib-qt-qml-tricks/src/qqmlhelpers.h \
    datatypes.h \
    digitalfiltering.h \
    packetinterface.h \
    utility.h \
    serialization.h \
    $$PWD/bldcinterface.h \
    $$PWD/mcconfiguration.h \
    $$PWD/appconfiguration.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
