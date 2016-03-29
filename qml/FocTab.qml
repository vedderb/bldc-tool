import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import bldc 1.0
Item{
    anchors.fill: parent
    TabView{
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: rectGap
        Tab{
            title: "Sensorless"
            Flickable{
                anchors.fill: parent
                contentHeight: mainCol.height
                clip: true

                Column{
                    id: mainCol
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    anchors.top: parent.top
                    anchors.topMargin: headingTopMargin
                    anchors.right: parent.right
                    anchors.rightMargin: headingLeftMargin
                    spacing: rectGap
                    Column{
                        width: parent.width
                        spacing: rowVerticalMargin
                        Text{
                            text: "Sensor Mode"
                            font.pointSize: 14
                        }
                        Row{
                            id: sensorModeRow
                            anchors.left: parent.left
                            anchors.leftMargin: rowLeftMargin
                            spacing: rowContentSpacing

                            ExclusiveGroup{
                                id:groupOptionsSensorMode
                            }


                            RadioButton{
                                id:rbSensorless
                                text: "Encoder"
                                exclusiveGroup :groupOptionsSensorMode
                                style: RadioButtonStyle{
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 14
                                        text: control.text
                                    }
                                }
                            }
                            RadioButton{
                                id:rbSensored
                                text: "Hall"
                                exclusiveGroup :groupOptionsSensorMode
                                style: RadioButtonStyle{
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 14
                                        text: control.text
                                    }
                                }
                            }
                            RadioButton{
                                id:rbHybrid
                                text: "Sensorless"
                                exclusiveGroup :groupOptionsSensorMode
                                style: RadioButtonStyle{
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 14
                                        text: control.text
                                    }
                                }
                            }
                        }

                    }

                    GridLayout{
                        columns: 3
                        width: parent.width
                        rowSpacing: rowVerticalMargin
                        columnSpacing: rowContentSpacing

                        Text {
                            text: qsTr("Current Control")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }

                        SpinBox{
                            Layout.preferredWidth: parent.width*0.15
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Kp: "
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.15
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Ki: "
                        }
                        Text {
                            text: qsTr("Encoder")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.15
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Ofs: "
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.15
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Rat: "
                        }
                        CheckBox{
                            text: "Invert Encoder"
                            Layout.row: 2
                            Layout.column: 2
                            Layout.topMargin: rowVerticalMargin
                        }
                    }
                    GroupBox{
                        width: parent.width
                        title: "Detect and Calculate Parameters"
                        GridLayout{
                            id: detectGrid
                            width: parent.width
                            rowSpacing: rowVerticalMargin
                            columnSpacing: rowContentSpacing
                            columns: 4
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                text: "Measure R and L"
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "R: "
                                suffix: "Ω"
                                decimals: 5
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "L: "
                                suffix: "µH"
                                decimals: 2
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "λ: "
                                decimals:8
                            }
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                text: "Measure λ (Req: R)"
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "I: "
                                suffix: "A"
                                decimals: 2
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Duty: "
                                decimals: 2
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "RPM: "
                                decimals:1
                            }
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                text: "Calc CC (Req: R and L)"
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "TC: "
                                suffix: "µS"
                                decimals: 1
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Kp: "
                                decimals:4
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Ki: "
                                decimals:2
                            }
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                text: "Apply"
                                Layout.row: 3
                                Layout.column: 3
                                Layout.topMargin: rowVerticalMargin
                            }
                        }
                    }
                }
            }
        }
        Tab{
            title: "Hall"
            Flickable{
                anchors.fill: parent
                clip: true
            }
        }
        Tab{
            title: "Encoder"
            Flickable{
                anchors.fill: parent
                clip: true
            }
        }
        Tab{
            title: "Advanced"
            Flickable{
                anchors.fill: parent
                clip: true
            }
        }
    }
    Row{
        id: bottomRow
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: headingTopMargin
        anchors.leftMargin: rowLeftMargin
        spacing: rowContentSpacing



        Button{
            id:buttonReadConf
            text:"Read Config"
            width: parent.width * 0.31
            style: buttonStyle
        }

        Button{
            id:buttonWriteConf
            text:"Write Config"
            width: parent.width * 0.31
            style: buttonStyle
        }

        Button{
            id:buttonReboot
            text:"Reboot"
            width: parent.width * 0.31
            style: buttonStyle
        }
    }
    Component{
        id: buttonStyle
        ButtonStyle {
            label: Text {
                renderType: Text.NativeRendering
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.family: "Helvetica"
                font.pointSize: 12
                text: control.text
            }
        }
    }
}

