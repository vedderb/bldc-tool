import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.0

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
                var path = fileDialog.fileUrl.toString()
                path = path.replace("file:///", "")
                textfieldPath.text = path
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
                    text : "Path"
                    font.pointSize: 14
                    verticalAlignment: Text.AlignVCenter
                }
                TextField{
                    id:textfieldPath
                    width: rectMain.width-textPath.width - buttonChoose.width - 2*rowLeftMargin -8
                    //anchors.verticalCenter:textPath.verticalCenter
                }
                Button{
                    id:buttonChoose
                    height: textfieldPath.height
                    text:"Choose"
                    width: rectMain.width * 0.17
                    onClicked: {
                        messageDialog.showWarrning("Warning", "WARNING: Uploading firmware for the wrong"+
                                                   " hardware version WILL damage the VESC for sure. Make "+
                                                   "sure that you choose the correct hardware version.")
                        // wait for message dialog and show file dialog
                        messageDialog.onAccepted.connect(openFileDialog)
                    }
                    function openFileDialog(){
                        fileDialog.open()
                        messageDialog.onAccepted.disconnect(openFileDialog)
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

        Rectangle{
            id:rectProgress
            anchors.top: rectUploadFirmware.bottom
            anchors.topMargin: rectGap
            Grid{
                id:gridProgress
                columns: 2
                columnSpacing: 4
                width: parent.width - (2* rowLeftMargin)
                anchors.left: parent.left
                anchors.leftMargin: rowLeftMargin
                anchors.right: parent.right
                anchors.rightMargin: rowLeftMargin
                anchors.top: textHeading.bottom
                anchors.topMargin: rowVerticalMargin

                ProgressBar{
                    id:progressBar
                    width:rectMain.width - buttonupload.width - gridProgress.columnSpacing - (2* rowLeftMargin)
                    minimumValue: 0
                    maximumValue: 100

                }

                Button{
                    id:buttonupload
                    height: textfieldPath.height
                    text:"upload"
                    width: rectMain.width * 0.17
                    onClicked: {
                        uploadFirmware(textfieldPath.text)
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
