#-------------------------------------------------
#
# Project created by QtCreator 2012-11-18T21:19:27
#
#-------------------------------------------------

QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BLDC_Tool
TEMPLATE = app


SOURCES += \
    mainwindow.cpp \
    qcustomplot.cpp \
    mrichtextedit.cpp \
    mtextedit.cpp \
    rtdatawidget.cpp

HEADERS  += \
    mainwindow.h \
    qcustomplot.h \
    mrichtextedit.h \
    mtextedit.h \
    rtdatawidget.h \

FORMS    += mainwindow.ui \
    mrichtextedit.ui

RESOURCES += \
    resources.qrc
