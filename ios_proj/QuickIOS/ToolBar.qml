import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQml.Models 2.1
import "./def"

Rectangle {
    id : toolBar

    height: 44

    property color tintColor : parent && parent.tintColor ? parent.tintColor : Constant.tintColor

    property color barTintColor : Constant.barTintColor

    property alias content : viewport.children

    color : barTintColor

    MouseArea { // Block all the mouse event
        anchors.fill: parent
    }

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
        color : Constant.dividerColor
    }

    Item {
        id : viewport
        anchors.fill: parent
    }

}

