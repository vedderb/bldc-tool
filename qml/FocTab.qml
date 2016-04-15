import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import bldc 1.0
Item{
    anchors.fill: parent
    id: mainItem
    TabView{
        anchors.top: parent.top
        anchors.topMargin: headingTopMargin
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: rowVerticalMargin
        style: TabViewStyle {

                tab: Rectangle {
                    color: styleData.selected ? "#E5E5E5" :"#DFDFDF"
                    border.color:  "#CDCDCD"
                    implicitWidth: Math.max(text.width + 4, 80)
                    implicitHeight: 30
                    radius: 2
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        font.pointSize: 12
                        font.bold: true
                    }
                }
            }
        Tab{
            title: "Sensorless"
            width: mainItem.width * 0.2

            Flickable{
                anchors.fill: parent
                contentHeight: sensorlessCol.height + rectGap
                contentWidth: mainItem.width
                clip: true

                Column{
                    id: sensorlessCol
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    anchors.top: parent.top
                    anchors.topMargin: headingTopMargin
                    anchors.right: parent.right
                    anchors.rightMargin: headingLeftMargin
                    spacing: 20
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
                                id:rbEncoder
                                checked: mcconf.foc_sensor_mode === FOC_SENSOR_MODE_ENCODER
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
                                checked: mcconf.foc_sensor_mode === FOC_SENSOR_MODE_HALL
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
                                id:rbSensorless
                                checked: mcconf.foc_sensor_mode === FOC_SENSOR_MODE_SENSORLESS
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


                    Row{
                        spacing: parent.width * 0.04
                        Text {
                            text: qsTr("Current Control")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 14
                            width:  mainItem.width * 0.3
                        }
                        TextField{
                            id:textFieldSensCurKp
                            text: mcconf.foc_current_kp
                            width:  mainItem.width * 0.29
                        }
                        TextField{
                            id:textFieldSensCurKi
                            width:  mainItem.width * 0.29
                            text: mcconf.foc_current_ki
                        }
                    }

                    Row{
                        spacing: parent.width * 0.04
                        Text {
                            text: qsTr("Encoder")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 14
                            width: mainItem.width * 0.3
                        }
                        TextField{
                            id:textFieldSensEncOfs
                            width:  mainItem.width * 0.29
                            text: mcconf.foc_encoder_offset
                        }
                        TextField{
                            id:textFieldSensEncRat
                            width:  mainItem.width * 0.29
                            text: mcconf.foc_encoder_ratio
                        }
                    }
                    CheckBox{
                        text: "Invert Encoder"
                        anchors.right: parent.right
                        checked: mcconf.foc_encoder_inverted
                    }

                    GroupBox{
                        width: parent.width
                        title: "Detect and Calculate Parameters"
                        Column{
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            spacing: 10
                            Row{
                                id: row1

                                spacing: parent.width * 0.02
                                Button{
                                    Layout.preferredWidth: mainItem.width*0.2
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
                                    width: mainItem.width*0.18
                                    text: mcconf.foc_motor_r
                                }
                                TextField{
                                    id:textFieldSensDetL
                                    width: mainItem.width*0.18
                                    text: mcconf.foc_motor_l
                                }
                                TextField{
                                    id:textFieldSensDetλ
                                    width: mainItem.width*0.18
                                    text: mcconf.foc_motor_flux_linkage
                                }
                            }

                            Row{
                                spacing: parent.width * 0.02
                                Button{
                                    Layout.preferredWidth: mainItem.width*0.2
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
                                    width: mainItem.width*0.18
                                    text: "6.0"
                                }
                                TextField{
                                    id:textFieldSensDetDuty
                                    width: mainItem.width*0.18
                                    text: "0.5"
                                }
                                TextField{
                                    id:textFieldSensDetRPM
                                    width: mainItem.width*0.18
                                    text: "700"
                                }
                            }

                            Row{
                                spacing: parent.width * 0.02
                                Button{
                                    Layout.preferredWidth: mainItem.width*0.2
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
                                    width: mainItem.width*0.18
                                    text: "1000.0"
                                }
                                TextField{
                                    id:textFieldSensDetKp
                                    width: mainItem.width*0.18
                                    text: mcconf.foc_current_kp
                                }
                                TextField{
                                    id:textFieldSensDetKi
                                    width: mainItem.width*0.18
                                    text: mcconf.foc_current_ki
                                }
                            }
                        }
                    }
                    GroupBox{
                        width: parent.width
                        title: "Motor Parameters"
                        clip: true

                        Column{
                            spacing: 20
                            Row{
                                spacing: mainItem.width * 0.02
                                TextField{
                                    id:textFieldSensMotR
                                    width: mainItem.width <  mainItem.height ? mainItem.width*0.4 : mainItem.width * 0.3
                                    text: mcconf.foc_motor_r
                                }
                                TextField{
                                    id:textFieldSensMotL
                                    width:  mainItem.width <  mainItem.height ? mainItem.width*0.23: mainItem.width * 0.3
                                    text: mcconf.foc_motor_l
                                }
                                TextField{
                                    id:textFieldSensMotλ
                                    width:  mainItem.width <  mainItem.height ? mainItem.width*0.25: mainItem.width * 0.3
                                    text: mcconf.foc_motor_flux_linkage
                                }

                            }

                            Row{
                                spacing: mainItem.width * 0.02
                                Text {
                                    text: qsTr("Observer Gain (x1M)")
                                    verticalAlignment: Text.AlignVCenter
                                    font.pointSize: 14
                                    width: mainItem.width <  mainItem.height ? mainItem.width*0.4 : mainItem.width * 0.3

                                }
                                TextField{
                                    id:textFieldSensGain
                                    width:mainItem.width <  mainItem.height ? mainItem.width*0.23: mainItem.width * 0.3
                                    text: mcconf.foc_observer_gain
                                }

                                Button{
                                    width:  mainItem.width <  mainItem.height ? mainItem.width*0.25: mainItem.width * 0.3
                                    id:buttonSensCalGain
                                    text:"Cal (Req: L)"
                                    // Layout.preferredWidth: parent.width * 0.2
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
                    }
                }
            }
        }
        Tab{
            title: "Hall"
            Flickable{
                anchors.fill: parent
                clip: true
                contentHeight: fochallCol.height + rectGap

                Column{
                    id: fochallCol
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    anchors.top: parent.top
                    anchors.topMargin: headingTopMargin
                    anchors.right: parent.right
                    anchors.rightMargin: headingLeftMargin
                    spacing: rectGap
                    GroupBox{
                        width: parent.width
                        title: "Hall Sensors"

                        Column{
                            spacing: 20
                            Row{
                                spacing: 2
                                TextField{
                                    id:textFieldFocTable1
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table1
                                }
                                TextField{
                                    id:textFieldFocTable2
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table2
                                }
                                TextField{
                                    id:textFieldFocTable3
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table3
                                }
                                TextField{
                                    id:textFieldFocTable4
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table4
                                }
                                TextField{
                                    id:textFieldFocTable5
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table5
                                }
                                TextField{
                                    id:textFieldFocTable6
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table6
                                }
                                TextField{
                                    id:textFieldFocTable7
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table7
                                }
                                TextField{
                                    id:textFieldFocTable8
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table8
                                }


                            }

                            Row{
                                spacing: rowVerticalMargin
                                Text {
                                    text: qsTr("Sensorless ERPM")
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pointSize: 14

                                }
                                TextField{
                                    id:textFieldSensHallErpm
                                    width: mainItem.width*0.15
                                    text: mcconf.foc_hall_sl_erpm
                                }
                            }
                        }
                    }
                    GroupBox{
                        width: parent.width
                        title: "Detect Hall Sensors"

                        Column{
                            spacing: 10

                            Row{
                                spacing: mainItem.width * 0.05
                                Button{
                                    text:"Measure"
                                    width: mainItem.width * 0.27
                                    style: buttonStyle
                                }

                                TextField{
                                    id:textFieldDetHallI
                                    width: mainItem.width*0.27
                                    text: "15.0"
                                }
                            }

                            Row{
                                id: detectHallSensorRow
                                spacing: mainItem.width * 0.015
                                TextField{
                                    id:textFieldDetTable1
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table1
                                }
                                TextField{
                                    id:textFieldDetTable2
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table2
                                }
                                TextField{
                                    id:textFieldDetTable3
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table3
                                }
                                TextField{
                                    id:textFieldDetTable4
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table4
                                }
                                TextField{
                                    id:textFieldDetTable5
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table5
                                }
                                TextField{
                                    id:textFieldDetTable6
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table6
                                }
                                TextField{
                                    id:textFieldDetTable7
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table7
                                }
                                TextField{
                                    id:textFieldDetTable8
                                    width: mainItem.width*0.11
                                    text: mcconf.foc_hall_table8
                                }
                            }

                            Button{
                                text:"Apply"
                                width: mainItem.width * 0.28
                                style: buttonStyle
                                anchors.right: parent.right
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

                        Column{
                            spacing: 10
                            Row{

                                spacing: mainItem.width * 0.05
                                Button{
                                    Layout.preferredWidth: parent.width*0.20
                                    text:"Measure"
                                    width: mainItem.width * 0.27
                                    style: buttonStyle
                                }
                                TextField{
                                    id:textFieldEncOfs
                                    width: mainItem.width*0.27
                                    text: mcconf.foc_encoder_offset
                                }
                                TextField{
                                    id:textFieldEncRat
                                    width: mainItem.width*0.27
                                    text: mcconf.foc_encoder_ratio
                                }
                            }

                            Row{
                                spacing: mainItem.width * 0.05
                                TextField{
                                    id:textFieldEncI
                                    width: mainItem.width*0.27
                                    text: "15.0"
                                }
                                CheckBox{
                                    text: "Invert Encoder"
                                }

                            }

                            Button{
                                text:"Apply"
                                width: mainItem.width * 0.27
                                style: buttonStyle
                                anchors.right: parent.right
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
                    spacing: 10
                    Row{
                        spacing: mainItem.width * 0.03
                        Text {
                            text: qsTr("F_SW and DTc")
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 14
                            width: mainItem.width*0.35

                        }

                        TextField{
                            id:textFieldAdvFSW
                            width: mainItem.width*0.25
                            text: mcconf.foc_f_sw
                        }
                        TextField{
                            id:textFieldAdvDTc
                            width: mainItem.width*0.25
                            text: mcconf.foc_dt_us
                        }
                    }

                    Row{
                        spacing: mainItem.width * 0.03
                        Text {
                            text: qsTr("Speed Tracker")
                            width: mainItem.width*0.35
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldAdvSpeKp
                            width: mainItem.width*0.25
                            text: mcconf.foc_pll_kp
                        }
                        TextField{
                            id:textFieldAdvSpeKi
                            width: mainItem.width*0.25
                            text: mcconf.foc_pll_ki
                        }
                    }

                    Row{
                        spacing: mainItem.width * 0.03
                        Text {
                            text: qsTr("Duty Downramp")
                            width: mainItem.width*0.35
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldAdvDutKp
                            width: mainItem.width*0.25
                            text: mcconf.foc_duty_dowmramp_kp
                        }
                        TextField{
                            id:textFieldAdvDutKi
                            width: mainItem.width*0.25
                            text: mcconf.foc_duty_dowmramp_ki
                        }
                    }

                    Row{
                        spacing: mainItem.width * 0.03
                        Text {
                            text: qsTr("Openloop RPM")
                            width: mainItem.width*0.35
                            anchors.verticalCenter: parent.verticalCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldAdvRPM
                            width: mainItem.width*0.25
                            text: mcconf.foc_openloop_rpm
                        }
                    }
                    GroupBox{
                        width: parent.width
                        title: "Sensorless Startup and Low Speed"

                        Column{
                            spacing: 10
                            Row{
                                spacing: mainItem.width * 0.03
                                Text {
                                    text: qsTr("Open Loop")
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pointSize: 14
                                    width: mainItem.width*0.35

                                }

                                TextField{
                                    id:textFieldAdvSensHyst
                                    width: mainItem.width*0.25
                                    text: mcconf.foc_sl_openloop_hyst
                                }
                                TextField{
                                    id:textFieldAdvSensTime
                                    width: mainItem.width*0.25
                                    text: mcconf.foc_sl_openloop_time
                                }

                            }

                            Row{
                                spacing: mainItem.width * 0.03
                                Text {
                                    text: qsTr("D Current Injection")
                                    anchors.verticalCenter: parent.verticalCenter
                                    font.pointSize: 14
                                    width: mainItem.width*0.35
                                }

                                TextField{
                                    id:textFieldAdvDuty
                                    width: mainItem.width*0.25
                                    text: mcconf.foc_sl_d_current_duty
                                }
                                TextField{
                                    id:textFieldAdvFact
                                    width: mainItem.width*0.25
                                    text: mcconf.foc_sl_d_current_factor
                                }

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
