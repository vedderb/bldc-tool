import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.0
import bldc 1.0

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
        height: mainWindow.height - statusbar.height
    }
    MessageDialog {
        id: messageDialog
        modality: Qt.ApplicationModal
        function show(title, text){
            messageDialog.title = title
            messageDialog.text = text
            messageDialog.visible = true;
        }
        function showCritical(title, text){
            icon = StandardIcon.Critical
            show(title, text)
        }
        function showWarrning(title, text){
            icon = StandardIcon.Warning
            show(title, text)
        }
        onAccepted: visible = false
        Component.onCompleted: {
            onMsgCritical.connect(showCritical);
            onMsgwarning.connect( showWarrning );
        }
    }
    statusBar: StatusBar {
        id: statusbar
        visible: true
        RowLayout {
            anchors.fill: parent
            Rectangle{
                id: background
                anchors.fill: parent
                color: 'white'//'transparent'
            }
            Label {
                id: label
                anchors.fill: parent
                anchors.leftMargin: 10
                text: "Ready"
            }
        }
        function showStatus(text, good){
            label.text = text
            if(good)
                background.color = 'lightgreen'
            else
                background.color = 'red'
        }
        Component.onCompleted: {
            onStatusInfoChanged.connect(showStatus);
            if (os == OS.Android) {
                statusBar.height = label.font.pixelSize * 1.5;
            }
        }
    }
}
