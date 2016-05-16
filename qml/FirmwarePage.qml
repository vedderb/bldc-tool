import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.0

import bldc 1.0

import "components"

BasicPage {
    id:rootFirmware
    showTopbar: true
    showBackButton: true
    pageTitle: "Firmware"

    property int headingTopMargin:4
    property int headingLeftMargin:6
    property int rowVerticalMargin:6
    property int rowLeftMargin:16
    property int rowContentSpacing: 10
    property int rectGap: 20

    Binding{
        target:rootFirmware
        property: "pageComponent"
        value:mainComponent
        when:true
    }

    property Component mainComponent: Rectangle {
        id: rectMain
        color: "#DCDCDC"
        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            selectMultiple: false
            onAccepted: {
                textfieldPath.text = localFilePath(fileDialog.fileUrl)
            }

        }
        Rectangle{
            id:  rectUploadFirmware
            width: parent.height
            height: textHeading.height + headingTopMargin + grid.height +rowVerticalMargin
            color: parent.color

            Text{
                id:textHeading
                anchors.top: parent.top
                anchors.topMargin:headingTopMargin
                anchors.left: parent.left
                anchors.leftMargin: headingLeftMargin
                text: "Upload New Firmware"
                font.bold: true

            }
            Grid{
                id:grid
                columns: 3
                columnSpacing: 4
                width: parent.width - (2* rowLeftMargin)
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                anchors.top: textHeading.bottom
                anchors.topMargin: rowVerticalMargin
                Text{
                    id:textPath
                    text : "ONLINE UPDATE"
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Rectangle{
            id:rectProgress
            anchors.top: rectUploadFirmware.bottom
            anchors.topMargin: rectGap

            Grid {
                id:gridRadioButtons
                columns: 3
                columnSpacing: 4
                width: parent.width - (2* rowLeftMargin)
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                anchors.top: rectProgress.bottom
                anchors.topMargin: rowVerticalMargin


                ExclusiveGroup{
                    id: groupFirmwareUrl
                }

                BldcFirmwareRadioButton {
                    id: firmwareRadioButton1
                    firmwareSource: FirmwareSource.Source1
                    text: "4.10-4.12"
                    exclusiveGroup: groupFirmwareUrl
                }

                BldcFirmwareRadioButton {
                    id: firmwareRadioButton2
                    firmwareSource: FirmwareSource.Source2
                    text: "4.8"
                    exclusiveGroup: groupFirmwareUrl
                }

                BldcFirmwareRadioButton {
                    id: firmwareRadioButton3
                    firmwareSource: FirmwareSource.Source3
                    text: "4.6-4.7"
                    exclusiveGroup: groupFirmwareUrl
                }
            }

            Grid{
                id:gridProgress
                columns: 2
                columnSpacing: 4
                width: parent.width - (2* rowLeftMargin)
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                anchors.top: gridRadioButtons.bottom
                anchors.topMargin: rowVerticalMargin

                ProgressBar{
                    id:progressBar
                    width:rectMain.width - onlineUpdateBtn.width - gridProgress.columnSpacing - (2* rowLeftMargin)
                    minimumValue: 0
                    maximumValue: 100
                    value: firmwareProgress

                }

                Button {
                    id: onlineUpdateBtn
                    text:"Update"
                    width: rectMain.width * 0.17
                    onClicked: {
                        onlineUpdateFirmware()
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


            Grid{
                id:gridwarning
                columns: 1
                columnSpacing: 4
                width: parent.width - (2* rowLeftMargin)
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                anchors.top: gridProgress.bottom
                anchors.topMargin: rowVerticalMargin
                Text{
                    id:textwarning
                    text : {
                        "Updating your firmware through \n"+
                        "this app only supports \n"+
                        "The latest Default hardware version's \n"+
                        "4.10-4.12. \n"+
                        "Please check your hardware version \n"+
                        "before using this feature \n"+
                        "As incorrect firmware will \n"+
                        "damage your hardware \n"+
                        "USE AT YOUR OWN RISK!!!!!!"
                    }
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
