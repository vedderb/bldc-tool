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

                        TextField{
                            id:textFieldSensCurKp
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        TextField{
                            id:textFieldSensCurKi
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        Text {
                            text: qsTr("Encoder")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldSensCurOfs
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        TextField{
                            id:textFieldSensCurKi2
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
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
                                id:buttonSensCalRL
                                text: "R & L"
                                onClicked: {
                                    console.log("Measuring R and L")
                                    packetInterface.detectMotorParam(textFieldCurrent.text,
                                                                     textFieldMinRPM.text,
                                                                     textFieldLowDuty.text)
                                }
                            }
                            TextField{
                                id:textFieldSensDecR
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensDetL
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensDetλ
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                id:buttonSensCalλ
                                text: "Measure λ"
                                onClicked: {
                                    console.log("Measuring λ")
                                    packetInterface.detectMotorParam(textFieldCurrent.text,
                                                                     textFieldMinRPM.text,
                                                                     textFieldLowDuty.text)
                                }
                            }
                            TextField{
                                id:textFieldSensDetI
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensDetDuty
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensDetRPM
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                id:buttonSensCalCC
                                text: "Calc CC"
                                onClicked: {
                                    console.log("Calculating CC")
                                    packetInterface.detectMotorParam(textFieldCurrent.text,
                                                                     textFieldMinRPM.text,
                                                                     textFieldLowDuty.text)
                                }
                            }
                            TextField{
                                id:textFieldSensDetTC
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensDetKp
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensDetKi
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            Button{
                                Layout.preferredWidth: parent.width*0.2
                                id:buttonSensDetApply
                                text: "Apply"
                                Layout.row: 3
                                Layout.column: 3
                                Layout.topMargin: rowVerticalMargin
                                onClicked: {
                                    console.log("Applying values")
                                    packetInterface.detectMotorParam(textFieldCurrent.text,
                                                                     textFieldMinRPM.text,
                                                                     textFieldLowDuty.text)
                                }
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

                            TextField{
                                id:textFieldSensMotR
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensMotL
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldSensMotλ
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            Text {
                                text: qsTr("Observer Gain (x1M)")
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14

                            }
                            TextField{
                                id:textFieldSensGain
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }

                            Button{
                                Layout.preferredWidth: parent.width*0.20
                                id:buttonSensCalGain
                                text:"Cal (Req: L)"
                                width: parent.width * 0.31
                                style: buttonStyle
                                onClicked: {
                                    console.log("Calculating Observer Gain")
                                    packetInterface.detectMotorParam(textFieldCurrent.text,
                                                                     textFieldMinRPM.text,
                                                                     textFieldLowDuty.text)
                                }
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

                                TextField{
                                    id:textFieldTable1
                                    width: parent.width*0.09
                                    text: mcconf.hall_table1
                                }
                                TextField{
                                    id:textFieldTable2
                                    width: parent.width*0.09
                                    text: mcconf.hall_table2
                                }
                                TextField{
                                    id:textFieldTable3
                                    width: parent.width*0.09
                                    text: mcconf.hall_table3
                                }
                                TextField{
                                    id:textFieldTable4
                                    width: parent.width*0.09
                                    text: mcconf.hall_table4
                                }
                                TextField{
                                    id:textFieldTable5
                                    width: parent.width*0.09
                                    text: mcconf.hall_table5
                                }
                                TextField{
                                    id:textFieldTable6
                                    width: parent.width*0.09
                                    text: mcconf.hall_table6
                                }
                                TextField{
                                    id:textFieldTable7
                                    width: parent.width*0.09
                                    text: mcconf.hall_table7
                                }
                                TextField{
                                    id:textFieldTable8
                                    width: parent.width*0.09
                                    text: mcconf.hall_table8
                                }

                            Row{
                                spacing: rowVerticalMargin
                                Text {
                                    text: qsTr("Sensorless ERPM")
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14

                                }
                                TextField{
                                    id:textFieldBRERPM
                                    width: parent.width*0.2
                                    text: mcconf.sl_cycle_int_rpm_br
                                }
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
                            TextField{
                                id:textFieldEncI
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldEncOfs
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldEncRat
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
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

                                TextField{
                                    id:textFieldDetHallI
                                    width: parent.width*0.2
                                    text: mcconf.sl_cycle_int_rpm_br
                                }
                            }
                            Row{
                                id: detectHallSensorRow
                                spacing: rowVerticalMargin
                                TextField{
                                    id:textFieldDetTable1
                                    width: parent.width*0.09
                                    text: mcconf.hall_table1
                                }
                                TextField{
                                    id:textFieldDetTable2
                                    width: parent.width*0.09
                                    text: mcconf.hall_table2
                                }
                                TextField{
                                    id:textFieldDetTable3
                                    width: parent.width*0.09
                                    text: mcconf.hall_table3
                                }
                                TextField{
                                    id:textFieldDetTable4
                                    width: parent.width*0.09
                                    text: mcconf.hall_table4
                                }
                                TextField{
                                    id:textFieldDetTable5
                                    width: parent.width*0.09
                                    text: mcconf.hall_table5
                                }
                                TextField{
                                    id:textFieldDetTable6
                                    width: parent.width*0.09
                                    text: mcconf.hall_table6
                                }
                                TextField{
                                    id:textFieldDetTable7
                                    width: parent.width*0.09
                                    text: mcconf.hall_table7
                                }
                                TextField{
                                    id:textFieldDetTable8
                                    width: parent.width*0.09
                                    text: mcconf.hall_table8
                                }
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

                        TextField{
                            id:textFieldAdvFSW
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        TextField{
                            id:textFieldAdvDTc
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        Text {
                            text: qsTr("Speed Tracker")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldAdvSpeKp
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        TextField{
                            id:textFieldAdvSpeKi
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        Text {
                            text: qsTr("Duty Downramp")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldAdvDutKp
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        TextField{
                            id:textFieldAdvDutKi
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
                        }
                        Text {
                            text: qsTr("Openloop RPM")
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldAdvRPM
                            width: parent.width*0.2
                            text: mcconf.sl_cycle_int_rpm_br
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

                            TextField{
                                id:textFieldAdvSensHyst
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldAdvSensTime
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            Text {
                                text: qsTr("D Current Injection")
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14

                            }

                            TextField{
                                id:textFieldAdvDuty
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
                            }
                            TextField{
                                id:textFieldAdvFact
                                width: parent.width*0.2
                                text: mcconf.sl_cycle_int_rpm_br
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
        height: childrenRect.height + rectGap

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
