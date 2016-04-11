// A ruler component that show a ruler visually and provide the value of the length
// Project: QuickIOS (https://github.com/hilarycheng/quickios)
// Author: benlau
// License: Apache

import QtQuick 2.0

Item {
    id : ruler

    // This property holds the orientation of the ruler. The value can be either Qt.Horizontal or Qt.Vertical. The default value is Qt.Horizontal.
    property int orientation : Qt.Horizontal

    // The length of the ruler
    property int length;

    // The width of the line
    property int lineWidth : 1 ;

    // The length of the line in arrow head.
    property int arrowHeadLength : 20;

    // The color of the ruler
    property string color : "#FF0000";

    // The pixel size of the label
    property int labelPixelSize : 17

    // The offset of the label
    property point labelOffset : Qt.point(defaults.offsetX,defaults.offsetY);


    Rectangle {
        id: head1
        color : ruler.color
    }

    Rectangle {
        id: head2
        color : ruler.color
    }

    Rectangle {
        id: body
        color : ruler.color
    }

    Text {
        id: label
        text: ruler.length
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: ruler.labelOffset.y
        anchors.horizontalCenterOffset: ruler.labelOffset.x
        color: ruler.color
        font.pixelSize: ruler.labelPixelSize
    }

    QtObject {
        id: defaults
        property int offsetX : 0
        property int offsetY : 0
    }

    width: 100
    height: 100

    states: [
        State {
            name: "Horizontal"
            when: orientation === Qt.Horizontal

            PropertyChanges {
                target: head1
                width: lineWidth;
                height: arrowHeadLength
            }

            AnchorChanges {
                target: head1
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
            }

            PropertyChanges {
                target: head2
                width: lineWidth;
                height: arrowHeadLength
            }

            AnchorChanges {
                target: head2
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
            }

            PropertyChanges {
                target: body
                width: parent.width
                height: lineWidth
            }

            AnchorChanges {
                target: body;
                anchors.verticalCenter: ruler.verticalCenter
            }

            PropertyChanges {
                target: ruler
                length: ruler.width
            }

            PropertyChanges {
                target: defaults
                offsetX: 0
                offsetY: - label.height / 2  - 8
            }

        },
        State {
            name: "Vertical"
            when: orientation === Qt.Vertical

            PropertyChanges {
                target: head1
                width: arrowHeadLength;
                height: lineWidth;
            }

            AnchorChanges {
                target: head1
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
            }

            PropertyChanges {
                target: head2
                width: arrowHeadLength;
                height: lineWidth;
            }

            AnchorChanges {
                target: head2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
            }

            PropertyChanges {
                target: body
                width: lineWidth
                height: parent.height
            }

            AnchorChanges {
                target: body;
                anchors.horizontalCenter: parent.horizontalCenter
            }

            PropertyChanges {
                target: ruler
                length: ruler.height
            }

            PropertyChanges {
                target: defaults
                offsetX: 8 + label.width / 2
                offsetY: 0
            }
        }
    ]




}
