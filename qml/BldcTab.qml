import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import bldc 1.0

Flickable{
    id:flicableMain
    anchors.fill: parent
    contentHeight:rectMain.height+rectGap
    clip: true

    Rectangle{
        id:rectMain
        height:rectSensorMode.height + rectSensorless.height+rectHallSensors.height+rectDetectParams.height + rectButtons.height +(5 * rectGap)
        width: parent.width
        color: "#DCDCDC"

        Rectangle{
            id:rectSensorMode
            anchors.top: parent.top
            width: parent.width
            color: parent.color
            height: textHeadingSensorMode.height + headingTopMargin +rowVerticalMargin + rectGridSensorMode.height

            Text{
                id:textHeadingSensorMode
                anchors.top: parent.top
                anchors.topMargin: headingTopMargin
                anchors.left:parent.left
                anchors.leftMargin: headingLeftMargin
                text: "Sensor Mode"
                font.bold: true
                font.pointSize: 14
            }

            Rectangle{
                id:rectGridSensorMode
                anchors.top: textHeadingSensorMode.bottom
                anchors.topMargin: rowVerticalMargin
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                width: parent.width-(2*rowLeftMargin)
                color: parent.color
                height: gridSensorMode.height

                Grid{
                    id: gridSensorMode
                    columnSpacing: 6
                    columns: 3
                    width: parent.width

                    ExclusiveGroup{
                        id:groupOptionsSensorMode
                    }

                    RadioButton{
                        id:rbSensorless
                        checked: mcconf.sensor_mode === McConf.SENSOR_MODE_SENSORLESS
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
                    RadioButton{
                        id:rbSensored
                        checked: mcconf.sensor_mode === McConf.SENSOR_MODE_SENSORED
                        text: "Sensored"
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
                        checked: mcconf.sensor_mode === McConf.SENSOR_MODE_HYBRID
                        text: "Hybrid"
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
        }

        Rectangle{
            id:rectSensorless
            anchors.top: rectSensorMode.bottom
            anchors.topMargin: rectGap
            width: parent.width
            height: cbSensorless.height+headingTopMargin+textCommMode.height + (rowVerticalMargin * 2.60) +rectCommModeOptions.height +rectGrid.height
            color: parent.color

            Text{
                id:cbSensorless
                text: "Sensorless"
                font.bold: true
                anchors.top: parent.top
                anchors.topMargin: headingTopMargin
                anchors.left: parent.left
                anchors.leftMargin: headingLeftMargin

            }

            Text{
                id:textCommMode
                anchors.top: cbSensorless.bottom
                anchors.topMargin: rowVerticalMargin
                anchors.left:parent.left
                anchors.leftMargin: rowLeftMargin
                text: "Communication mode"
                font.pointSize: 14
                //color:cbSensorless.checked?"black":"#b5bbb7"
            }
            Rectangle{
                id:rectCommModeOptions
                anchors.top :textCommMode.bottom
                anchors.topMargin: rowVerticalMargin*0.60
                anchors.left: parent.left
                //anchors.leftMargin: rowLeftMargin
                // spacing:rowContentSpacing
                width: parent.width
                height: rbIntegrate.height
                color: parent.color

                ExclusiveGroup{
                    id:groupOptions
                }

                RadioButton{
                    id:rbIntegrate
                    checked: mcconf.comm_mode === McConf.COMM_MODE_INTEGRATE
                    // enabled: cbSensorless.checked
                    anchors.left: parent.left
                    anchors.leftMargin: rowLeftMargin
                    text: "Integrate"
                    width: rectMain.width *0.45
                    exclusiveGroup :groupOptions
                    style: RadioButtonStyle{
                        label: Text {
                            renderType: Text.NativeRendering
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "Helvetica"
                            font.pointSize: 14
                            text: control.text
                            // color:cbSensorless.checked?"black":"#b5bbb7"
                        }
                    }
                }
                RadioButton{
                    id:rbDelay
                    checked: mcconf.comm_mode === McConf.COMM_MODE_DELAY
                    text: "Delay"
                    // enabled: cbSensorless.checked
                    anchors.left: rbIntegrate.right
                    anchors.leftMargin: rowLeftMargin
                    exclusiveGroup :groupOptions
                    style: RadioButtonStyle{
                        label: Text {
                            renderType: Text.NativeRendering
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.family: "Helvetica"
                            font.pointSize: 14
                            text: control.text
                            //   color:cbSensorless.checked?"black":"#b5bbb7"
                        }
                    }
                }
            }

            Rectangle{
                id:rectGrid
                anchors.top: rectCommModeOptions.bottom
                anchors.topMargin: rowVerticalMargin
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                width: parent.width-(2*rowLeftMargin)
                color: parent.color
                // height:textMinERPM.height+textMinERPMLimit.height+textBEMF.height+textBRERPM.height+textIntLimit.height+textMaxBrake.height+textphase.height+rowVerticalMargin*4.2

                height: grid.height
                Grid{
                    id:grid
                    columns: 2
                    columnSpacing: 2
                    rowSpacing: rowVerticalMargin*0.6
                    width: parent.width

                    Text{
                        id:textMinERPM
                        text: "Minimum ERPM"
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 14
                        // color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldMinERPM
                        // enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_min_erpm
                    }

                    Text{
                        id:textMinERPMLimit
                        text: "Minimum ERPM for integrator limit"
                        verticalAlignment: Text.AlignVCenter
                        width: rectGrid.width*0.75
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        font.pointSize: 14
                        // color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldMinERPMLimit
                        //  enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_min_erpm_cycle_int_limit
                    }

                    Text{
                        id:textMaxBrake
                        text: "Maximum full brake current during direction change"
                        verticalAlignment: Text.AlignVCenter
                        width: rectGrid.width*0.75
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        font.pointSize: 14
                        //  color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldMaxBrake
                        //  enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_max_fullbreak_current_dir_change
                    }
                    Text{
                        id:textIntLimit
                        text: "Integrator limit"
                        verticalAlignment: Text.AlignVCenter
                        width: rectGrid.width*0.75
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        font.pointSize: 14
                        // color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldIntLimit
                        //   enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_cycle_int_limit
                    }
                    Text{
                        id:textphase
                        text: "Phase advance at BR ERPM"
                        verticalAlignment: Text.AlignVCenter
                        width: rectGrid.width*0.75
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        font.pointSize: 14
                        //  color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldPhase
                        //   enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_phase_advance_at_br
                    }
                    Text{
                        id:textBRERPM
                        text: "BR ERPM"
                        verticalAlignment: Text.AlignVCenter
                        width: rectGrid.width*0.75
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        font.pointSize: 14
                        //  color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldBRERPM
                        //  enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_cycle_int_rpm_br
                    }
                    Text{
                        id:textBEMF
                        text: "BEMF Coupling"
                        verticalAlignment: Text.AlignVCenter
                        width: rectGrid.width*0.75
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        font.pointSize: 14
                        // color:cbSensorless.checked?"black":"#b5bbb7"
                    }
                    TextField{
                        id:textFieldBEMF
                        // enabled: cbSensorless.checked
                        width: parent.width*0.2
                        text: mcconf.sl_bemf_coupling_k
                    }
                }
            }
        }

        Rectangle{
            id:rectHallSensors
            width: parent.width
            anchors.top: rectSensorless.bottom
            anchors.topMargin: rectGap
            color: parent.color
            height: textHeadingHall.height+rectGridHallTable.height +(rowVerticalMargin * 2) + rectGridSensorlessERPM.height

            Text{
                id:textHeadingHall
                anchors.top: parent.top
                anchors.left:parent.left
                anchors.leftMargin: headingLeftMargin
                text: "Hall sensors"
                font.bold: true
                font.pointSize: 14
            }
            Rectangle{
                id:rectGridHallTable
                anchors.top: textHeadingHall.bottom
                anchors.topMargin: rowVerticalMargin
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                width: parent.width-(2*rowLeftMargin)
                color: parent.color
                height: gridHallTable.height
                Grid{
                    id:gridHallTable
                    columns: 9
                    columnSpacing:parent.width *0.02
                    width: parent.width
                    Text{
                        id:textDirection
                        text: "Table"
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

                }
            }

            Rectangle{
                id:rectGridSensorlessERPM
                anchors.top: rectGridHallTable.bottom
                anchors.topMargin: rowVerticalMargin
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                width: parent.width-(2*rowLeftMargin)
                color: parent.color
                height: gridHallSensorless.height
                Grid{
                    id:gridHallSensorless
                    columns: 2
                    columnSpacing:parent.width *0.02
                    width: parent.width
                    Text{
                        id:textSensorlessERPM
                        text:"Sensorless ERPM (hybrid mode)"
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 14
                        width: rectGrid.width*0.75


                    }
                    TextField{
                        id:textFieldHybridMode
                        width: parent.width * 0.2
                        text: mcconf.hall_sl_erpm
                    }

                }
            }

            Rectangle{
                id:rectDetectParams
                width: parent.width
                anchors.top: rectHallSensors.bottom
                anchors.topMargin: rectGap
                color: parent.color
                height: textHeadingDetect.height + buttonStartDet.height+rectGridDetect.height+rowVerticalMargin
                // height: 100

                Text{
                    id:textHeadingDetect
                    anchors.top: parent.top
                    anchors.left:parent.left
                    anchors.leftMargin: headingLeftMargin
                    text: "Detect parameters"
                    font.bold: true
                    font.pointSize: 14
                }
                Button{
                    id:buttonStartDet
                    anchors.left: parent.left
                    anchors.leftMargin:rowLeftMargin
                    anchors.top: textHeadingDetect.bottom
                    anchors.topMargin:rowVerticalMargin
                    width: parent.width *0.4
                    text:"Start detection"
                    onClicked: {
                        console.log("Start Detection tapped")
                        packetInterface.detectMotorParam(textFieldCurrent.text,
                                                         textFieldMinRPM.text,
                                                         textFieldLowDuty.text)
                    }
                    style: ButtonStyle {
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

                Rectangle{
                    id:rectGridDetect
                    anchors.top: buttonStartDet.bottom
                    anchors.topMargin: rowVerticalMargin
                    anchors.left: parent.left
                    anchors.leftMargin: rowLeftMargin
                    anchors.right: parent.right
                    // anchors.rightMargin: rowLeftMargin
                    width: buttonStartDet.width
                    color: parent.color
                    height: gridDetect.height
                    Grid{
                        id:gridDetect
                        columns: 2
                        columnSpacing: 6
                        rowSpacing: rowVerticalMargin*0.6
                        width: parent.width

                        Text{
                            id:textCurrent
                            text: "Current"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldCurrent
                            width: parent.width*0.2
                            text: "6.0"
                        }

                        Text{
                            id:textMinRPM
                            text: "Min ERPM"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldMinRPM
                            width: parent.width*0.2
                            text: "600"
                        }

                        Text{
                            id:textLowDuty
                            text: "Low Duty"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                        }
                        TextField{
                            id:textFieldLowDuty
                            width: parent.width*0.2
                            text: "0.05"
                        }
                    }
                }

                TextArea{
                    id:textArea
                    anchors.top: buttonStartDet.top
                    height: buttonStartDet.height+rectGridDetect.height+rowVerticalMargin
                    width: parent.width- buttonStartDet.width-(2.5 * rowLeftMargin)
                    anchors.left: buttonStartDet.right
                    anchors.leftMargin: rowLeftMargin/2
                    anchors.right: parent.right
                    anchors.rightMargin: rowLeftMargin
                    text: mcconfDetectResult

                }
            }

            Rectangle{
                id:rectButtons
                width: rectMain.width - (2 * headingLeftMargin)
                anchors.top: rectDetectParams.bottom
                anchors.topMargin: rectGap
                anchors.left: parent.left
                anchors.leftMargin: headingLeftMargin
                color: parent.color
                height: childrenRect.height
                Grid{
                    id:gridButtons
                    columns: 3
                    columnSpacing: headingLeftMargin/1.5

                    Button{
                        id:buttonReadConf
                        text:"Read Config"
                        width: rectButtons.width * 0.31
                        onClicked: packetInterface.getMcconf()
                        style: ButtonStyle {
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

                    Button{
                        id:buttonWriteConf
                        text:"Write Config"
                        width: rectMain.width * 0.31
                        onClicked: {
                            if(rbSensorless.checked)    mcconf.sensor_mode = McConf.SENSOR_MODE_SENSORLESS
                            else if(rbSensored.checked) mcconf.sensor_mode = McConf.SENSOR_MODE_SENSORED
                            else if(rbHybrid.checked)   mcconf.sensor_mode = McConf.SENSOR_MODE_HYBRID

                            if(rbIntegrate.checked)  mcconf.comm_mode = McConf.COMM_MODE_INTEGRATE
                            else if(rbDelay.checked) mcconf.comm_mode = McConf.COMM_MODE_DELAY

                            mcconf.sl_min_erpm = textFieldMinERPM.text
                            mcconf.sl_max_fullbreak_current_dir_change = textFieldMaxBrake.text
                            mcconf.sl_cycle_int_limit = textFieldIntLimit.text
                            mcconf.sl_phase_advance_at_br = textFieldPhase.text
                            mcconf.sl_cycle_int_rpm_br = textFieldBRERPM.text
                            mcconf.sl_bemf_coupling_k = textFieldBEMF.text
                            mcconf.hall_table1 = textFieldTable1.text
                            mcconf.hall_table2 = textFieldTable2.text
                            mcconf.hall_table3 = textFieldTable3.text
                            mcconf.hall_table4 = textFieldTable4.text
                            mcconf.hall_table5 = textFieldTable5.text
                            mcconf.hall_table6 = textFieldTable6.text
                            mcconf.hall_table7 = textFieldTable7.text
                            mcconf.hall_table8 = textFieldTable8.text
                            mcconf.hall_sl_erpm = textFieldHybridMode.text

                            writeMcconf()
                        }
                        style: ButtonStyle {
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

                    Button{
                        id:buttonReboot
                        text:"Reboot"
                        width: rectMain.width * 0.31
                        onClicked: packetInterface.reboot()
                        style: ButtonStyle {
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
            }
        }
    }
}

