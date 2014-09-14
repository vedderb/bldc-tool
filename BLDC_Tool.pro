#-------------------------------------------------
#
# Project created by QtCreator 2012-11-18T21:19:27
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BLDC_Tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    serialport.cpp \
    digitalfiltering.cpp \
    packetinterface.cpp \
    utility.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    serialport.h \
    digitalfiltering.h \
    packetinterface.h \
    utility.h

FORMS    += mainwindow.ui
