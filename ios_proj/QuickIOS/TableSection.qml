/* TableSection is a section of UITableView in grouped style.

Author: Ben Lau (benlau)
License: Apache License
Project: https://github.com/hilarycheng/quickios
*/

import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: table

    property string headerTitle : " "
    property string footerTitle : " "
    property color separatorColor  : "#1F000000"
    property int separatorInsetLeft : 20
    property int separatorInsetRight : 0
    property int rowHeight: 44
    property alias model : repeater.model

    // The default header. User may simplify override it.
    property Component header : Item {
        width: parent.width
        height: Math.max(28,headerLabel.paintedHeight + 16)

        Text {
            id: headerLabel
            color : "#8a000000"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            font.pixelSize: 12
            text: headerTitle
        }
    }

    /// This holds the header item created from the header component.
    property alias headerItem : headerLoader.item

    // The default footer. User may simplify override it.
    property Component footer : Item {
        width: parent.width
        height: Math.max(28,footerLabel.paintedHeight + 16)

        Text {
            id : footerLabel
            color : "#8a000000"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            font.pixelSize: 12
            text: footerTitle
        }
    }

    /// This holds the footer item created from the footer component.
    property alias footerItem : footerLoader.item

    signal selected(int index,var item)

    height: headerLoader.height + footerLoader.height + rowHeight * repeater.count

    Column {

        Loader {
            id: headerLoader
            sourceComponent: header
            width: parent.width
        }

        Repeater {
            id : repeater

            // The default delegate for table cell. User may override it.
            delegate : Rectangle {
                id: cell
                color : "#FFFFFF"
                width: table.width
                height: rowHeight

                Rectangle {
                    id: mask
                    visible : false
                    color : "#1f000000"
                    anchors.fill: parent
                }

                MouseArea {
                    id : mouseArea
                    anchors.fill: parent
                    onClicked: {
                        selected(index,model);
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 20
                    anchors.rightMargin: 20

                    Image {
                        source: model.source ? model.source : ""
                        anchors.verticalCenter: parent.verticalCenter
                        Layout.fillWidth: false
                    }

                    Text {
                        font.pixelSize: 16
                        color : "#000000"
                        text: model.title
                    }

                }

                Rectangle {
                    id : separator
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: separatorInsetLeft
                    anchors.rightMargin: separatorInsetRight
                    color : separatorColor
                    height: 1
                    visible: index !== table.model.count -1
                }

                states : [
                    State {
                        when: mouseArea.pressed
                        PropertyChanges {
                            target: mask
                            visible: true
                        }
                    }

                ]
            }
        }

        Loader {
            id: footerLoader
            width: parent.width
            sourceComponent : footer
        }
    }

}

