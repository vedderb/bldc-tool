import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import bldc 1.0

Flickable{
    id:flicKableMain
    height: parent.height
    width: parent.width
    contentWidth: width
    contentHeight:rectControlMode.height + rectSettings.height
                  +rectRPMLimit.height +rectDisplay.height
                  + rectMultESC.height + rectButtons.height
                  +( mainWindow.height > mainWindow.width ?  (7 * rectGap) : (6 *rectGap))
    clip: true
    Rectangle{
        id:background
        color: "#DCDCDC"
        anchors.fill: parent
        height: rectControlMode.height + rectSettings.height
                +rectRPMLimit.height +rectDisplay.height
                + rectMultESC.height + rectButtons.height
                +( mainWindow.height > mainWindow.width ?  (7 * rectGap) : (6 *rectGap))
    }
    Rectangle{
        id:rectControlMode
        anchors.top: parent.top
        anchors.topMargin: rowVerticalMargin
        anchors.leftMargin: rowLeftMargin
        width: parent.width
        height: childrenRect.height//childrenRect.height
        color:  "#DCDCDC"
        Text {
            id: textHeadingControl
            text: "Control Mode"
            font.bold: true
            anchors.top:parent.top
            anchors.topMargin: headingTopMargin
            anchors.left: parent.left
            anchors.leftMargin: headingLeftMargin
        }
        ExclusiveGroup{
            id:groupOptions
        }
        Grid{
            anchors.top: textHeadingControl.bottom
            anchors.left: parent.left
            anchors.leftMargin: headingLeftMargin
            id:gridControl
            width: parent.width
            columns: 2
            columnSpacing: rowLeftMargin
            rowSpacing: rowVerticalMargin*0.6
            RadioButton{
                id:rbDisabled
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_NONE
                text: "Disabled"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
            RadioButton{
                id:rbDutyCycle
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_DUTY
                text: "Duty Cycle"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                    }
                }
            }
            RadioButton{
                id:rbCurrent
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_CURRENT
                text: "Current"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
            RadioButton{
                id:rbDutyCycleNoRev
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_DUTY_NOREV
                text: "Duty Cycle no reverse"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
            RadioButton{
                id:rbCurrentNoRev
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_CURRENT_NOREV
                text: "Current no reverse"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
            RadioButton{
                id:rbPIDSC
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_PID
                text: "PID speed control"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
            RadioButton{
                id:rbCurrNoRevBrake
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE
                text: "Current no reverse with brake"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
            RadioButton{
                id:rbPIDSCNoRev
                checked: appconf.ppm_ctrl_type === AppConf.PPM_CTRL_TYPE_PID_NOREV
                text: "PID speed control no reverse"
                exclusiveGroup :groupOptions
                style: RadioButtonStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 14
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: control.text
                        width: rectControlMode.width *0.38
                    }
                }
            }
        }

    }
    Rectangle{
        id:rectSettings
        anchors.top: rectControlMode.bottom
        anchors.topMargin: rectGap
        width: parent.width
        color:  background.color
        height: childrenRect.height//textHeadingSettings.height + headingTopMargin +rectGridSettings.height + rowVerticalMargin
        Text {
            id: textHeadingSettings
            text: "Settings"
            font.bold: true
            anchors.top:parent.top
            anchors.topMargin: headingTopMargin
            anchors.left: parent.left
            anchors.leftMargin: headingLeftMargin
        }
        Rectangle{
            id:rectGridSettings
            width:parent.width - (2 * rowLeftMargin)
            anchors.left:parent.left
            anchors.leftMargin: rowLeftMargin
            anchors.right: parent.right
            anchors.rightMargin: rowLeftMargin
            anchors.top: textHeadingSettings.bottom
            anchors.topMargin: rowVerticalMargin
            height: childrenRect.height
            color:  background.color
            Grid{
                id:gridSettings
                width: parent.width
                columns: 2
                columnSpacing: 2
                rowSpacing: rowVerticalMargin*0.6
                Text {
                    id: textPID
                    text: "PID max ERPM"
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                    width: rectGridSettings.width * 0.75
                    font.pointSize: 14
                }
                TextField{
                    id:textFieldPID
                    width: rectGridSettings.width * 0.2
                    text: appconf.ppm_pid_max_erpm
                }
                Text {
                    id: textDeadband
                    text: "Deadband"
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                    width: rectGridSettings.width * 0.75
                    font.pointSize: 14
                }
                TextField{
                    id:textFieldDeadband
                    width: rectGridSettings.width * 0.2
                    text: appconf.ppm_hyst
                }
                Text {
                    id: textMinPulse
                    text: "Minimum pulsewidth (milliseconds)"
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                    width: rectGridSettings.width * 0.75
                    font.pointSize: 14
                }
                TextField{
                    id:textFieldMinPulse
                    width: rectGridSettings.width * 0.2
                    text: appconf.ppm_pulse_start
                }
                Text {
                    id: textWidMinPulse
                    text: "Width after minimum pulse (milliseconds)"
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                    width: rectGridSettings.width * 0.75
                    font.pointSize: 14
                }
                TextField{
                    id:textFieldWidMinPulse
                    width: rectGridSettings.width * 0.2
                    text: appconf.ppm_pulse_end
                }
            }
        }
    }
    Rectangle{
        id:rectRPMLimit
        anchors.top: rectSettings.bottom
        anchors.topMargin: rectGap
        width: parent.width
        height: childrenRect.height//cbRPMLimit.height +headingTopMargin +rectGridRPMLimit.height +rowVerticalMargin
        color:  background.color
        CheckBox{
            id:cbRPMLimit
            checked: appconf.ppm_rpm_lim_end  >= 200000.0
            text: "Soft RPM limit(current mode only)"
            anchors.top: parent.top
            anchors.topMargin: headingTopMargin
            anchors.left: parent.left
            anchors.leftMargin: headingLeftMargin
            style: CheckBoxStyle{
                label: Text {
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Helvetica"
                    font.bold: true
                    text: control.text
                }
            }
        }
        Rectangle{
            id:rectGridRPMLimit
            width:parent.width - (2 * rowLeftMargin)
            anchors.left:parent.left
            anchors.leftMargin: rowLeftMargin
            anchors.right: parent.right
            anchors.rightMargin: rowLeftMargin
            anchors.top: cbRPMLimit.bottom
            anchors.topMargin: rowVerticalMargin
            height: childrenRect.height
            color:  background.color
            Grid{
                id:gridRPMLimit
                width: parent.width
                columns: 2
                columnSpacing: 2
                rowSpacing: rowVerticalMargin*0.6
                Text {
                    id: textRPMLimStart
                    text: "RPM limit start"
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                    width: rectGridSettings.width * 0.75
                    font.pointSize: 14
                    color:cbRPMLimit.checked?"black":"#b5bbb7"
                }
                TextField{
                    id:textFieldRPMLimStart
                    width: rectGridSettings.width * 0.2
                    enabled: cbRPMLimit.checked
                    text: appconf.ppm_rpm_lim_start
                }
                Text {
                    id: textRPMLimEnd
                    text: "RPM limit end"
                    maximumLineCount: 2
                    wrapMode: Text.WordWrap
                    width: rectGridSettings.width * 0.75
                    font.pointSize: 14
                    color:cbRPMLimit.checked?"black":"#b5bbb7"
                }
                TextField{
                    id:textFieldRPMLimEnd
                    width: rectGridSettings.width * 0.2
                    enabled: cbRPMLimit.checked
                    text: appconf.ppm_rpm_lim_end
                }
            }
        }
    }
    Rectangle{
        id:rectMultESC
        anchors.top: rectRPMLimit.bottom
        anchors.topMargin: rectGap
        width: parent.width
        height: childrenRect.height//cbMultESC.height +headingTopMargin +rectGridMultESC.height +rowVerticalMargin
        color:  background.color
        CheckBox{
            id:cbMultESC
            checked: appconf.ppm_multi_esc
            text: "Multiple ESCs over CAN"
            anchors.top: parent.top
            anchors.topMargin: headingTopMargin
            anchors.left: parent.left
            anchors.leftMargin: headingLeftMargin
            style: CheckBoxStyle{
                label: Text {
                    renderType: Text.NativeRendering
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: "Helvetica"
                    font.bold: true
                    text: control.text
                }
            }
        }
        Rectangle{
            id:rectGridMultESC
            width:parent.width - (2 * rowLeftMargin)
            anchors.left:parent.left
            anchors.leftMargin: rowLeftMargin
            anchors.right: parent.right
            anchors.rightMargin: rowLeftMargin
            anchors.top: cbMultESC.bottom
            anchors.topMargin: rowVerticalMargin
            height: cbEnableTraction.height
            color:  background.color
            CheckBox{
                id:cbEnableTraction
                anchors.left: parent.left
                width: parent.width * 0.40
                checked: appconf.ppm_tc
                text: "Enable Traction Control(current mode only)"
                style: CheckBoxStyle{
                    label: Text {
                        renderType: Text.NativeRendering
                        verticalAlignment: Text.AlignVCenter
                        font.family: "Helvetica"
                        font.pointSize: 13
                        text: control.text
                        maximumLineCount: 3
                        wrapMode: Text.WordWrap
                        width: rectGridMultESC.width * 0.38
                    }
                }
            }
            Text{
                id:textTractCont
                text:"Traction Control ERPM diff"
                horizontalAlignment: Text.AlignRight
                font.pointSize: 13
                maximumLineCount: 3
                wrapMode: Text.WordWrap
                width: parent.width * 0.38
                anchors.right: textFieldTractCont.left
                anchors.rightMargin: 4
            }
            TextField{
                id:textFieldTractCont
                width: parent.width * 0.20
                anchors.right: parent.right
                text: appconf.ppm_tc_max_diff
            }
        }
    }
    Rectangle{
        id:rectDisplay
        anchors.top: rectMultESC.bottom
        anchors.topMargin: rectGap
        width: parent.width
        height: childrenRect.height//textHeadingDisplay.height +headingTopMargin + progressBar.height + 2 *rowVerticalMargin + cbDecPPM.height
        color:  background.color
        Text {
            id: textHeadingDisplay
            text: "Display"
            font.bold: true
            anchors.top:parent.top
            anchors.topMargin: headingTopMargin
            anchors.left: parent.left
            anchors.leftMargin: headingLeftMargin
        }
        CheckBox{
            id:cbDecPPM
            checked: false
            text: "Decoded PPM value"
            anchors.top: textHeadingDisplay.bottom
            anchors.topMargin: rowVerticalMargin
            anchors.left: parent.left
            anchors.leftMargin: rowLeftMargin
            style: CheckBoxStyle{
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
        ProgressBar{
            id:progressBar
            anchors.top: cbDecPPM.bottom
            anchors.topMargin: rowVerticalMargin
            anchors.left: parent.left
            anchors.leftMargin: rowLeftMargin
            anchors.right: parent.right
            anchors.rightMargin: rowLeftMargin
            width:rectDisplay.width - (2* rowLeftMargin)
            minimumValue: 0
            maximumValue: 1000
            value: appconfDecodedPpm
        }
    }
    Rectangle{
        id:rectButtons
        width: flicKableMain.width - (2 * headingLeftMargin)
        anchors.top: rectDisplay.bottom
        anchors.topMargin: rectGap
        anchors.left: parent.left
        anchors.leftMargin: headingLeftMargin
        color: background.color
        height: childrenRect.height//gridButtons.height
        Grid{
            id:gridButtons
            columns: 3
            columnSpacing: headingLeftMargin/1.5
            height: buttonReadConf.height
            Button{
                id:buttonReadConf
                text:"Read Config"
                width: rectButtons.width * 0.31
                onClicked: packetInterface.getAppConf()
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
                width: flicKableMain.width * 0.31
                onClicked: {
                    if(rbDisabled.checked)              appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_NONE
                    else if(rbDutyCycle.checked)        appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_DUTY
                    else if(rbCurrent.checked)          appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_CURRENT
                    else if(rbDutyCycleNoRev.checked)   appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_DUTY_NOREV
                    else if(rbCurrentNoRev.checked)     appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_CURRENT_NOREV
                    else if(rbPIDSC.checked)            appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_PID
                    else if(rbCurrNoRevBrake.checked)   appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_CURRENT_NOREV_BRAKE
                    else if(rbPIDSCNoRev.checked)       appconf.ppm_ctrl_type = AppConf.PPM_CTRL_TYPE_PID_NOREV

                    appconf.ppm_pid_max_erpm = textFieldPID.text
                    appconf.ppm_hyst = textFieldDeadband.text
                    appconf.ppm_pulse_start = textFieldMinPulse.text
                    appconf.ppm_pulse_end = textFieldWidMinPulse.text

                    if(cbRPMLimit.checked){
                        appconf.ppm_rpm_lim_start = textFieldRPMLimStart.text
                        appconf.ppm_rpm_lim_end = textFieldRPMLimEnd.text
                    }
                    appconf.ppm_multi_esc = cbMultESC.checked
                    appconf.ppm_tc = cbEnableTraction.checked
                    appconfUpdatePpm = cbDecPPM.checked
                    appconf.ppm_tc_max_diff = textFieldTractCont.text

                    writeAppConf()
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
                width: flicKableMain.width * 0.31
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

