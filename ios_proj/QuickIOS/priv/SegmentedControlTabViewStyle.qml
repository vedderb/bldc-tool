import QtQuick 2.0
import QtQuick.Controls.Styles 1.2
import "../def"

TabViewStyle {
    id : style
    property color tintColor : Constant.tintColor;

    readonly property int contentHeight: 50
    readonly property real radius : 5
    property color backgroundColor : "#ffffff"

    tabsAlignment: Qt.AlignHCenter
    tabOverlap: 0

    tabBar: Rectangle {
        color : backgroundColor
    }

    frame : Item {} // Transparent

    leftCorner: Item {
        implicitHeight: contentHeight
        implicitWidth : 22 + radius
        clip: true
        visible: control.count > 0
        Rectangle {
            antialiasing: true
            x: 22
            radius : style.radius
            border.width: 1
            border.color: tintColor
            anchors.verticalCenter: parent.verticalCenter
            height: 32
            width: 32
            color : "#00000000"

            Rectangle {
                id: leftRect
                color : tintColor
                border.width: 1
                radius : style.radius
                border.color: tintColor
                anchors.fill: parent
                opacity: 0

                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }

        }

        Binding {target : leftRect ; property : "opacity"; value : 1 ; when: control.currentIndex === 0}
        Binding {target : leftRect ; property : "opacity"; value : 0 ; when: control.currentIndex !== 0}

    }

    rightCorner: Item {
        implicitHeight: contentHeight
        implicitWidth : 22 + radius
        clip: true
        visible: control.count > 0

        Rectangle {
            antialiasing: true
            color : "#00000000"

            x: -width + radius
            radius : style.radius
            border.width: 1
            border.color: tintColor
            anchors.verticalCenter: parent.verticalCenter
            height: 32
            width: 32

            Rectangle {
                id: rightRect
                color : tintColor
                border.width: 1
                radius : style.radius
                border.color: tintColor
                anchors.fill: parent

                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }

        }

        Binding {target : rightRect ; property : "opacity"; value : 1 ; when: control.currentIndex === control.count - 1}
        Binding {target : rightRect ; property : "opacity"; value : 0 ; when: control.currentIndex !== control.count - 1}
    }

    tab: Item {
        implicitHeight: contentHeight
        implicitWidth: styleData.availableWidth /control.count

        Rectangle {
            id: topLine
            color : tintColor
            height : 1
            anchors.top: parent.top
            anchors.topMargin: 9
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Rectangle {
            color : tintColor
            id: bottomLine
            height : 1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 9
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Rectangle {
            id: divider
            width: 1
            color : tintColor
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 9
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 9
            visible: styleData.index !== 0
        }

        Rectangle {
            id : mask
            anchors.top: topLine.bottom
            anchors.bottom: bottomLine.top
            anchors.horizontalCenter: parent.horizontalCenter
            color : tintColor
            width: parent.width
            opacity: 0
            Behavior on opacity {

                NumberAnimation {
                    duration: 100
                }
            }
        }

        Text {
            id : text
            anchors.centerIn: parent
            text: styleData.title
            color : tintColor
            font.pixelSize: 17
        }

        Binding { target: mask; property : "opacity"; value : 1; when: styleData.selected }
        Binding { target: mask; property : "opacity";value : 0;when: !styleData.selected}

        Binding { target: text; property : "color"; value : "#ffffff"; when: styleData.selected  }
        Binding { target: text; property : "color"; value : tintColor; when: !styleData.selected  }

    }


}
