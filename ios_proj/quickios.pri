QT += qml quick widgets

QML_IMPORT_PATH += $$PWD

INCLUDEPATH += $$PWD

RESOURCES += $$PWD/QuickIOS/quickios.qrc

HEADERS += $$PWD/quickios.h \
    $$PWD/qiviewdelegate.h \
    $$PWD/qialertview.h \
    $$PWD/qidevice.h \
    $$PWD/qiactionsheet.h \
    $$PWD/qiactivityindicator.h \
    $$PWD/qisystemmessenger.h

SOURCES += $$PWD/quickios.cpp \
    $$PWD/qialertview.cpp \
    $$PWD/qidevice.cpp \
    $$PWD/qiactionsheet.cpp \
    $$PWD/qiactivityindicator.cpp \
    $$PWD/qisystemmessenger.cpp

ios {
    QT += gui-private

    QMAKE_CXXFLAGS += -fobjc-arc

    OBJECTIVE_SOURCES += \
        $$PWD/qisystemutils.mm \
        $$PWD/qiviewdelegate.mm \
        $$PWD/qidevice.mm

    #Qt Quick Controls
    QTPLUGIN += qtquickcontrolsplugin modelsplugin

    # Fix for linking issues for Qt 5.3.2
    QTQUICKCONTROLS_LIBS = -L$$clean_path($$QMAKESPEC/../../qml/QtQuick/Controls)
    QTQMLMODELS_LIBS = -L$$clean_path($$QMAKESPEC/../../qml/QtQml/Models.2)
    LIBS += $$QTQUICKCONTROLS_LIBS $$QTQMLMODELS_LIBS

    QMAKE_POST_LINK += /usr/libexec/PlistBuddy -c \"Add :UIViewControllerBasedStatusBarAppearance bool false\" $${OUT_PWD}/Info.plist
}

