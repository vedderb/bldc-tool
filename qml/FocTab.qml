import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import bldc 1.0
Item{
    anchors.fill: parent
    TabView{
        anchors.top: parent.top
        anchors.topMargin: headingTopMargin
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: rowVerticalMargin
        Tab{
            title: "Sensorless"
            Flickable{
                anchors.fill: parent
                contentHeight: sensorlessCol.height + rectGap
                clip: true

                Column{
                    id: sensorlessCol
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
                contentHeight: hallCol.height + rectGap

                Column{
                    id: hallCol
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    anchors.top: parent.top
                    anchors.topMargin: headingTopMargin
                    anchors.right: parent.right
                    anchors.rightMargin: headingLeftMargin
                    spacing: rectGap
                    GroupBox{
                        width: parent.width
                        title: "Motor Parameters (for Sensorless and Hall Operation)"
                        GridLayout{
                            width: parent.width
                            rowSpacing: rowVerticalMargin
                            columnSpacing: rowContentSpacing
                            columns: 3

                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "R: "
                                suffix: "  Ω"
                                maximumValue: 1000.0
                                stepSize: 0.001000
                                decimals: 5
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "L: "
                                suffix: " µH"
                                decimals: 2
                                maximumValue: 1000000.0
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "λ: "
                                maximumValue: 1000.0
                                stepSize: 0.001000
                                decimals: 6
                            }
                            Text {
                                text: qsTr("Observer Gain (x1M)")
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14

                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                decimals: 2
                                maximumValue: 100000.0
                            }

                            Button{
                                Layout.preferredWidth: parent.width*0.20
                                text:"Cal (Req: L)"
                                width: parent.width * 0.31
                                style: buttonStyle
                            }


                        }
                    }

                    GroupBox{
                        width: parent.width
                        title: "Hall Sensors"

                        Column{
                            width: parent.width
                            spacing: rowContentSpacing

                            Row{
                                spacing: rowVerticalMargin
                                Text {
                                    text: qsTr("Table")
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14

                                }

                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                                SpinBox{width:hallCol.width * 0.1}
                            }

                            Row{
                                spacing: rowVerticalMargin
                                Text {
                                    text: qsTr("Sensorless ERPM")
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14

                                }
                                SpinBox{
                                    Layout.preferredWidth: parent.width*0.20
                                    horizontalAlignment: Qt.AlignLeft
                                    decimals: 2
                                    maximumValue: 200000.0
                                    stepSize: 10
                                }
                            }
                        }
                    }
                }
            }
        }
        Tab{
            title: "Encoder"
            Flickable{
                anchors.fill: parent
                clip: true
                contentHeight: encoderCol.height + rectGap

                Column{
                    id: encoderCol
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    anchors.top: parent.top
                    anchors.topMargin: headingTopMargin
                    anchors.right: parent.right
                    anchors.rightMargin: headingLeftMargin
                    spacing: rectGap
                    GroupBox{
                        width: parent.width
                        title: "Detect Encoder"
                        GridLayout{
                            width: parent.width
                            rowSpacing: rowVerticalMargin
                            columnSpacing: rowContentSpacing
                            columns: 3

                            Button{
                                Layout.preferredWidth: parent.width*0.20
                                text:"Measure"
                                width: parent.width * 0.2
                                style: buttonStyle
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "I: "
                                suffix: "  A"
                                maximumValue: 99.0
                                decimals: 2
                                value: 15
                                Layout.columnSpan: 1
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Ofs: "
                                decimals: 2
                                maximumValue: 360
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Rat: "
                                maximumValue: 500
                                decimals: 2
                            }
                            CheckBox{
                                text: "Invert Encoder"
                            }

                            Button{
                                Layout.preferredWidth: parent.width*0.20
                                text:"Apply"
                                width: parent.width * 0.2
                                style: buttonStyle
                                Layout.column: 2
                                Layout.row: 2
                            }
                        }
                    }

                    GroupBox{
                        width: parent.width
                        title: "Detect Hall Sensors"

                        Column{
                            width: parent.width
                            spacing: rowContentSpacing

                            Row{
                                spacing: rowVerticalMargin
                                Button{
                                    text:"Measure"
                                    width: encoderCol.width * 0.2
                                    style: buttonStyle
                                }

                                SpinBox{
                                    width: encoderCol.width*0.20
                                    horizontalAlignment: Qt.AlignLeft
                                    prefix: "Rat: "
                                    maximumValue: 500
                                    decimals: 2
                                }
                            }
                            Row{
                                id: detectHallSensorRow
                                spacing: rowVerticalMargin
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                                SpinBox{width:encoderCol.width * 0.1}
                            }
                            Button{
                                text:"Apply"
                                width: parent.width * 0.2
                                style: buttonStyle
                                anchors.right: detectHallSensorRow.right
                            }
                        }
                    }
                }

            }
        }
        Tab{
            title: "Advanced"
            Flickable{
                anchors.fill: parent
                contentHeight: asdvancedCol.height + rectGap
                clip: true

                Column{
                    id: asdvancedCol
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    anchors.top: parent.top
                    anchors.topMargin: headingTopMargin
                    anchors.right: parent.right
                    anchors.rightMargin: headingLeftMargin
                    spacing: rectGap

                    GridLayout{
                        columns: 3
                        width: parent.width
                        rowSpacing: rowVerticalMargin
                        columnSpacing: rowContentSpacing

                        Text {
                            text: qsTr("F_SW and DTc")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14

                        }

                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "F_SW: "
                            suffix: " Hz"
                            maximumValue: 100000
                            stepSize: 100
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "DTc: "
                            suffix: " µS"
                            decimals: 3
                            stepSize: 0.01
                            maximumValue: 10000.0
                        }
                        Text {
                            text: qsTr("Speed Tracker")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Kp: "
                            decimals: 2
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Ki: "
                            decimals: 2
                        }
                        Text {
                            text: qsTr("Duty Downramp")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Kp: "
                            decimals: 2
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "Ki: "
                            decimals: 2
                        }
                        Text {
                            text: qsTr("Openloop RPM")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        SpinBox{
                            Layout.preferredWidth: parent.width*0.20
                            horizontalAlignment: Qt.AlignLeft
                            prefix: "RPM: "
                            decimals: 2
                            maximumValue: 100000.0
                            stepSize: 10.0
                            Layout.rowSpan: 1
                        }
                    }
                    GroupBox{
                        width: parent.width
                        title: "Sensorless Startup and Low Speed"
                        GridLayout{
                            width: parent.width
                            rowSpacing: rowVerticalMargin
                            columnSpacing: rowContentSpacing
                            columns: 3

                            Text {
                                text: qsTr("Open Loop")
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14

                            }

                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Hyst: "
                                suffix: " s"
                                maximumValue: 200.0
                                stepSize: 0.1
                                decimals: 3
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Time: "
                                suffix: " s"
                                decimals: 3
                                stepSize: 0.01
                                maximumValue: 10000.0
                            }
                            Text {
                                text: qsTr("D Current Injection")
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14

                            }

                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Duty: "
                                maximumValue: 1.0
                                stepSize: 0.01
                            }
                            SpinBox{
                                Layout.preferredWidth: parent.width*0.20
                                horizontalAlignment: Qt.AlignLeft
                                prefix: "Factor: "
                                decimals: 3
                                stepSize: 0.1
                                maximumValue: 99.0
                            }
                        }
                    }
                }
            }
        }
    }
    Row{
        id: bottomRow
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottomMargin: rowVerticalMargin
        anchors.leftMargin: rowLeftMargin
        spacing: rowContentSpacing
        height: childrenRect.height

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

