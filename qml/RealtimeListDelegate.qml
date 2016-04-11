import QtQuick 2.0

Rectangle {
    height: parent.height
    width: parent.width
    color: "#696969"
    Text {
        id: textLeft
        color: "#ffffff"
        text: lValue
        font.bold: true
        font.pointSize: 14
        verticalAlignment: Text.AlignVCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.width*0.02
        anchors.verticalCenter: parent.verticalCenter

    }
    Text {
        id: textCenter
        text: ":"
        font.bold: true
        font.pointSize: 14
        verticalAlignment: Text.AlignVCenter
        x:parent.width*0.5
        color: "#ffffff"
        anchors.verticalCenter: parent.verticalCenter

    }
    Text {
        id: textRight
        color: "#ffffff"
        text: rValue
        font.bold: true
        font.pointSize: 14
        verticalAlignment: Text.AlignVCenter
        anchors.left: textCenter.right
        anchors.leftMargin: parent.width*0.05
        anchors.verticalCenter: parent.verticalCenter

    }

}

