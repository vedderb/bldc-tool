import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1

ApplicationWindow {
    id:mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("BLDC")
    Component.onCompleted: {
        mainLoader.source = Qt.resolvedUrl("qrc:/qml/ApplicationMain.qml");
    }
    Loader {
        id: mainLoader
        width: mainWindow.width
        height: mainWindow.height
    }

}
