import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import bldc 1.0

BasicPage {
    id:rootAppConfig
    showTopbar: true
    showBackButton: true
    pageTitle: "App Configuration"

    property int headingTopMargin:4
    property int headingLeftMargin:6
    property int rowVerticalMargin:6
    property int rowLeftMargin:16
    property int rowContentSpacing: 10
    property int rectGap: 20

    Binding{
        target:rootAppConfig
        property: "pageComponent"
        value:mainComponent
        when:true
    }

    property Component mainComponent: Rectangle {
        id: name
        color: "#DCDCDC"

        Flickable{
            id:flicableMain
            height: parent.height
            width: parent.width
            contentWidth: width
            contentHeight:rectMain.height
            Rectangle{
                id:rectMain
                anchors.fill: parent
                color: "#DCDCDC"
                height: rectSettings.height + rectApp.height +rectTimeout.height + rectButtons.height +(4 * rectGap)

                Rectangle{
                    id:rectSettings
                    anchors.top: parent.top
                    width: parent.width
                    height: textSettings.height+headingTopMargin+rowControllerId.height+(3*rowVerticalMargin)+rbStatus.height+rowRate.height
                    // height: 100
                    color: parent.color

                    Text{
                        id:textSettings
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left:parent.left
                        anchors.leftMargin: headingLeftMargin
                        text: "Settings"
                        font.bold: true
                        font.pointSize: 16
                    }
                    Rectangle{
                        id:rowControllerId
                        anchors.top :textSettings.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        // anchors.leftMargin: rowLeftMargin
                        // spacing:rowContentSpacing
                        color: parent.color
                        width: parent.width
                        height: textfieldControllerId.height
                        Text{
                            id:textConID
                            text:"Controller ID"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            anchors.left: parent.left
                            anchors.leftMargin: rowLeftMargin
                        }
                        TextField{
                            id:textfieldControllerId
                            anchors.right: parent.right
                            anchors.rightMargin: rowLeftMargin
                            width: rectMain.width*0.2
                            anchors.verticalCenter:textConID.verticalCenter
                            text: appconf.controller_id
                        }
                    }
                    CheckBox{
                        id:rbStatus
                        checked: appconf.send_can_status
                        text: "Send status over CAN"
                        anchors.top: rowControllerId.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        style: CheckBoxStyle{
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
                    Rectangle{
                        id:rowRate
                        anchors.top :rbStatus.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        // anchors.leftMargin: rowLeftMargin
                        // spacing:rowContentSpacing
                        width: parent.width
                        height: textFieldRate.height
                        color: parent.color
                        Text{
                            id:textHZ
                            text:"Rate(Hz)"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            color:rbStatus.checked?"black":"#b5bbb7"
                            anchors.left: parent.left
                            anchors.leftMargin:rowLeftMargin
                        }
                        TextField{
                            id:textFieldRate
                            text: appconf.send_can_status_rate_hz
                            enabled: rbStatus.checked
                            anchors.right: parent.right
                            anchors.rightMargin: rowLeftMargin
                            width: parent.width*0.2
                            anchors.verticalCenter: textHZ.verticalCenter
                        }
                    }


                }

                Rectangle{
                    id:rectApp
                    anchors.top: rectSettings.bottom
                    anchors.topMargin: rectGap
                    anchors.left: parent.left
                    width: parent.width
                    color: parent.color
                    // height: 100
                    //  height:textAppHeading.height+headingTopMargin+(3*rowVerticalMargin)+rowOptions1.height+rowOptions2.height+rowOptions3.height+grid.height

                    height: textAppHeading.height+headingTopMargin + rectGrid.height +rowVerticalMargin
                    ExclusiveGroup{
                        id:groupOptions
                    }

                    Text{
                        id:textAppHeading
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left:parent.left
                        anchors.leftMargin: headingLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: headingLeftMargin
                        width: parent.width
                        maximumLineCount:  2
                        wrapMode: Text.WordWrap
                        text: "App to use(Changing this requires a reboot)"
                        font.bold: true
                        font.pointSize: 16
                    }

                    Rectangle{
                        id:rectGrid
                        anchors.top: textAppHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: rowLeftMargin
                        width: parent.width-(2*rowLeftMargin)

                        height: 4*rbNoApp.height+3*rowVerticalMargin
                        color: parent.color

                        Grid{
                            id:grid
                            columns: 2
                            columnSpacing: 2
                            rowSpacing: rowVerticalMargin
                            width: parent.width
                            //  anchors.centerIn: parent
                            RadioButton{
                                id:rbNoApp
                                checked: appconf.app_to_use === AppConf.APP_NONE

                                text: "No application"

                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
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
                            RadioButton{
                                id:rbPPMUART
                                checked: appconf.app_to_use === AppConf.APP_PPM
                                text: "PPM and UART"
                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
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
                            RadioButton{
                                id:rbCustomApp
                                checked: appconf.app_to_use === AppConf.APP_CUSTOM

                                text: "Custom user application"
                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        //verticalAlignment: Text.AlignVCenter
                                        // horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        text: control.text
                                        //width: rowOptions2.width * 0.40
                                        maximumLineCount: 2
                                        wrapMode: Text.WordWrap
                                    }
                                }
                            }
                            RadioButton{
                                id:rbPPM
                                checked: appconf.app_to_use === AppConf.APP_PPM
                                text: "PPM"

                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
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
                            RadioButton{
                                id:rbNunChuk
                                checked: appconf.app_to_use === AppConf.APP_NUNCHUK
                                text: "NunChuk"
                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
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
                            RadioButton{
                                id:rbUART
                                checked: appconf.app_to_use === AppConf.APP_UART
                                text: "UART"
                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
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
                            RadioButton{
                                id:rbNRF
                                checked: appconf.app_to_use === AppConf.APP_NRF
                                text: "NRF"
                                exclusiveGroup :groupOptions
                                style: RadioButtonStyle{
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

                Rectangle{
                    id:rectTimeout
                    anchors.top: rectApp.bottom
                    anchors.topMargin: rectGap
                    anchors.left: parent.left
                    width: parent.width
                    color: parent.color
                  //  height: 100
                     height: textTimeOutHeading.height+headingTopMargin+rowTimeout.height+(2*rowVerticalMargin)+rowBrakeCurrent.height

                    Text{
                        id:textTimeOutHeading
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        anchors.left:parent.left
                        anchors.leftMargin: headingLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: headingLeftMargin
                        maximumLineCount: 2
                        wrapMode: Text.WordWrap
                        text: "Timeout(When no control signal is received)"
                        font.bold: true
                        font.pointSize: 16
                    }

                    Rectangle{
                        id:rowTimeout
                        anchors.top :textTimeOutHeading.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        // anchors.leftMargin: rowLeftMargin
                        //spacing:rowContentSpacing
                        color: parent.color
                        width: rectMain.width
                        height: textFieldTimeout.height
                        Text{
                            id:textTimeout
                            text:"Timeout(in milliseconds)"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            anchors.left: parent.left
                            anchors.leftMargin: rowLeftMargin
                        }
                        TextField{
                            id:textFieldTimeout
                            text: appconf.timeout_msec
                            width: rectMain.width*0.2
                            anchors.right: parent.right
                            anchors.rightMargin: rowLeftMargin
                            anchors.verticalCenter: textTimeout.verticalCenter
                        }
                    }

                    Rectangle{
                        id:rowBrakeCurrent
                        anchors.top :rowTimeout.bottom
                        anchors.topMargin: rowVerticalMargin
                        anchors.left: parent.left
                        //anchors.leftMargin: rowLeftMargin
                        width: rectMain.width
                        height: textFieldBrakeCurrent.height
                        color: parent.color
                        Text{
                            id:textBrake
                            text:"Brake Current to use when a timeout occurs"
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            width: parent.width-(3*rowLeftMargin)-textFieldBrakeCurrent.width
                            anchors.left: parent.left
                            anchors.leftMargin: rowLeftMargin
                            anchors.right: textFieldBrakeCurrent.left
                            anchors.rightMargin: rowLeftMargin
                            maximumLineCount: 2
                            wrapMode: Text.WordWrap
                        }
                        TextField{
                            id:textFieldBrakeCurrent
                            text: appconf.timeout_brake_current
                            width: rectMain.width*0.2
                            anchors.right: parent.right
                            anchors.rightMargin: rowLeftMargin
                            anchors.verticalCenter: textBrake.verticalCenter
                        }
                    }




                }

                Rectangle{
                    id:rectButtons
                    width: rectMain.width - (2 * headingLeftMargin)
                    anchors.top: rectTimeout.bottom
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
                            height: textfieldPath.height
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
                            height: textfieldPath.height
                            text:"Write Config"
                            width: rectMain.width * 0.31
                            onClicked: {
                                appconf.controller_id = textfieldControllerId.text
                                appconf.send_can_status = rbStatus.checked
                                appconf.send_can_status_rate_hz = textFieldRate.text
                                if(rbNoApp.checked)         appconf.app_to_use = AppConf.APP_NONE
                                else if(rbPPMUART.checked)  appconf.app_to_use = AppConf.APP_PPM
                                else if(rbCustomApp.checked)appconf.app_to_use = AppConf.APP_CUSTOM
                                else if(rbPPM.checked)      appconf.app_to_use = AppConf.APP_PPM
                                else if(rbNunChuk.checked)  appconf.app_to_use = AppConf.APP_NUNCHUK
                                else if(rbUART.checked)     appconf.app_to_use = AppConf.APP_UART
                                else if(rbNRF.checked)      appconf.app_to_use = AppConf.APP_NRF
                                appconf.timeout_msec = textFieldTimeout.text
                                appconf.timeout_brake_current = textFieldBrakeCurrent.text

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
                            height: textfieldPath.height
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
}

