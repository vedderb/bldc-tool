import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

BasicPage {
    id:rootAppConfigPPM
    showTopbar: true
    showBackButton: true
    pageTitle: "App Configuration(PPM)"

    property int headingTopMargin:4
    property int headingLeftMargin:6
    property int rowVerticalMargin:6
    property int rowLeftMargin:16
    property int rowContentSpacing: 10
    property int rectGap: 20

    Binding{
        target:rootAppConfigPPM
        property: "pageComponent"
        value:mainComponent
        when:true
    }

    property Component mainComponent: Rectangle{
        color: "#DCDCDC"
        Flickable{
            id:flicKableMain
            height: parent.height
            width: parent.width
            contentWidth: width
            contentHeight:rectMain.height
            Rectangle{
                id:rectMain
                anchors.fill: parent
                color: "#DCDCDC"
                height: rectControlMode.height + rectSettings.height +rectRPMLimit.height +rectDisplay.height + rectMultESC.height  + rectButtons.height  +( mainWindow.height > mainWindow.width ?  (7 * rectGap) : (6 *rectGap))

                Rectangle{
                    id:rectControlMode
                    anchors.top: parent.top
                    width: parent.width
                    height: textHeadingControl.height +headingTopMargin +rectGridControl.height +rowVerticalMargin
                    color: "#DCDCDC"

                    ExclusiveGroup{
                        id:groupOptions
                    }

                    Text {
                        id: textHeadingControl
                        text: "Control Mode"
                        font.bold: true
                        anchors.top:parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left: parent.left
                        anchors.leftMargin: headingLeftMargin
                    }
                    Rectangle{
                        id:rectGridControl
                        width:parent.width - (2 * rowLeftMargin)
                        anchors.left:parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: rowLeftMargin
                        anchors.top: textHeadingControl.bottom
                        anchors.topMargin: rowVerticalMargin
                        height: childrenRect.height
                        color: parent.color

                        Grid{
                            id:gridControl
                            width: parent.width
                            columns: 2
                            columnSpacing: 2
                            rowSpacing: rowVerticalMargin*0.6

                            RadioButton{
                                id:rbDisabled
                                checked: false
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
                                checked: false
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
                                checked: false
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
                                checked: false
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
                                checked: false
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
                                checked: false
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
                                checked: false
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
                                checked: false
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
                }
                Rectangle{
                    id:rectSettings
                    anchors.top: rectControlMode.bottom
                    anchors.topMargin: rectGap
                    width: parent.width
                    color: parent.color
                    height: textHeadingSettings.height + headingTopMargin +rectGridSettings.height + rowVerticalMargin
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
                        color: parent.color

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
                            }
                        }
                    }

                }
                Rectangle{
                    id:rectRPMLimit
                    anchors.top: rectSettings.bottom
                    anchors.topMargin: rectGap
                    width: parent.width
                    height: cbRPMLimit.height +headingTopMargin +rectGridRPMLimit.height +rowVerticalMargin
                    color: parent.color

                    CheckBox{
                        id:cbRPMLimit
                        checked: false
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
                        color: parent.color

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
                            }
                        }
                    }
                }

                Rectangle{
                    id:rectMultESC
                    anchors.top: rectRPMLimit.bottom
                    anchors.topMargin: rectGap
                    width: parent.width
                    height: cbMultESC.height +headingTopMargin +rectGridMultESC.height +rowVerticalMargin
                    color: parent.color

                    CheckBox{
                        id:cbMultESC
                        checked: false
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
                        color: parent.color

                        CheckBox{
                            id:cbEnableTraction
                            anchors.left: parent.left
                            width: parent.width * 0.40
                            checked: false
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
                        }
                    }
                }

                Rectangle{
                    id:rectDisplay
                    anchors.top: rectMultESC.bottom
                    anchors.topMargin: rectGap
                    width: parent.width
                    height: textHeadingDisplay.height +headingTopMargin + progressBar.height + 2 *rowVerticalMargin + cbDecPPM.height
                    color: parent.color

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
                        maximumValue: 100

                    }



                }

                Rectangle{
                    id:rectButtons
                    width: rectMain.width - (2 * headingLeftMargin)
                    anchors.top: rectDisplay.bottom
                    anchors.topMargin: rectGap
                    anchors.left: parent.left
                    anchors.leftMargin: headingLeftMargin
                    color: parent.color
                    height: gridButtons.height
                    Grid{
                        id:gridButtons
                        columns: 3
                        columnSpacing: headingLeftMargin/1.5

                        Button{
                            id:buttonReadConf
                            height: textfieldPath.height
                            text:"Read Config"
                            width: rectButtons.width * 0.31
                            onClicked: {

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
                            id:buttonWriteConf
                            height: textfieldPath.height
                            text:"Write Config"
                            width: rectMain.width * 0.31
                            onClicked: {

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
                            height: textfieldPath.height
                            text:"Reboot"
                            width: rectMain.width * 0.31
                            onClicked: {

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

                    }
                }


            }



        }
    }
}

