
#DEFINES += QML # for test

android | ios : DEFINES += QML


contains(DEFINES,QML){
    include(QML.pri)
} else {
    include(Widgets.pri)
}
