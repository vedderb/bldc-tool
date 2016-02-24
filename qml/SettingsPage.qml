import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

BasicPage {

    id:rootSettings
    height: mainWindow.height
    width:mainWindow.width
    showBackButton: true
    showSettingsButton: false
    pageTitle: "Settings"

    property int headingTopMargin:4
    property int headingLeftMargin:6
    property int rowVerticalMargin:6
    property int rowLeftMargin:16
    property int rowContentSpacing: 10
    property int rectGap: 20

    Binding{
        target:rootSettings
        property: "pageComponent"
        value:mainComponent
        when:true
    }
    property Component mainComponent: Rectangle {
        color: "#DCDCDC"

        Flickable{
            id:flickableMain
            height: parent.height
            width: parent.width
            contentWidth: width
            contentHeight:rectContent.height
            Rectangle{
                id:rectContent
                width: parent.width
                height:rectConnection.height + rectControl.height +rectBEMF.height + rectPlotControl.height + (rectGap * 4)
                color:"#DCDCDC"

                Rectangle{
                    color: "#DCDCDC"
                    id:rectConnection
                    width: parent.width
                    height: textTitle.height + headingTopMargin +rectGridConnection.height + rowVerticalMargin
                    Text{
                        id:textTitle
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width * 0.02
                        anchors.top: parent.top
                        anchors.topMargin: headingTopMargin
                        text:"Connection"
                        font.bold: true
                        font.pointSize: 14

                    }

                    Rectangle{
                        id:rectGridConnection
                        width:parent.width - (2 * rowLeftMargin)
                        anchors.left:parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: rowLeftMargin
                        anchors.top: textTitle.bottom
                        anchors.topMargin: rowVerticalMargin
                        height: childrenRect.height
                        color: parent.color

                        Grid{
                            id:gridConnection
                            width: parent.width
                            columns: 2
                            columnSpacing: rowLeftMargin
                            rowSpacing: rowVerticalMargin*0.6

                            Text{
                                id:textPort
                                text:"Port"
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 14
                                width: rectConnection.width *0.4
                            }

                            TextField{
                                id:textFieldPort
                                width: rectConnection.width *0.4
                            }

                            Button{
                                id:buttonConnect
                                text:"Connect"
                                width: rectConnection.width *0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }

                            Button{
                                id:buttonDisconnect
                                width: rectConnection.width *0.4
                                text:"Disconnect"
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }



                        }
                    }

                }

                Rectangle{
                    color: "#DCDCDC"
                    id:rectControl
                    anchors.top:rectConnection.bottom
                    anchors.topMargin:rectGap
                    width: parent.width
                    height: textTitleControl.height + rectGridControl.height + 3 *rowVerticalMargin +kbControlChkRow.height +rectGridCurrent.height

                    Text{
                        id:textTitleControl
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width * 0.02
                        text:"Control"
                        font.bold: true
                        font.pointSize: 14
                    }

                    Rectangle{
                        id:rectGridControl
                        width:parent.width - (2 * rowLeftMargin)
                        anchors.left:parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: rowLeftMargin
                        anchors.top:textTitleControl.bottom
                        anchors.topMargin: rowVerticalMargin
                        height: childrenRect.height
                        color: parent.color

                        Grid{
                            id:gridControl
                            width: parent.width
                            columns: 2
                            columnSpacing: rowLeftMargin
                            rowSpacing: rowVerticalMargin*0.6

                            TextField {
                                id:textDutyCycle
                                text:"0.20"
                                //width: 70
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                                width: rectConnection.width *0.4
                            }
                            Button{
                                id:buttonDutyCycle
                                text:"Duty Cycle"
                                width: rectConnection.width *0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text

                                    }
                                }
                            }

                            TextField {
                                id:textRPM
                                text:"15000"
                                width: rectConnection.width *0.4
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                            }
                            Button{
                                id:buttonRPM
                                text:"RPM"
                                width: rectConnection.width *0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }

                            TextField {
                                id:textCurrent
                                text:"3,00"
                                width: rectConnection.width *0.4
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                            }
                            Button{
                                id:buttonCurrent
                                text:"Current"
                                width: rectConnection.width *0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }


                            TextField {
                                id:textBrakeCurrent
                                text:"3,00"
                                width: rectConnection.width *0.4
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                            }
                            Button{
                                id:buttonBrakeCurrent
                                width: rectConnection.width *0.4
                                text:"Brake Current"
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }







                        }}

                    Row {
                        id: kbControlChkRow
                        anchors.top:rectGridControl.bottom
                        anchors.topMargin:rowVerticalMargin
                        anchors.left: parent.left
                        anchors.leftMargin: rowLeftMargin
                        spacing: 8

                        CheckBox {
                            id: kbControlChkBox
                            text: qsTr("Keyboard Control")
                            checked: true
                            //                    anchors.topMargin: 8
                            //                    anchors.top: rectControl.bottom
                            //                    anchors.leftMargin: parent.width * 0.02
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
                    }

                    Rectangle{
                        id:rectGridCurrent
                        width:parent.width - (2 * rowLeftMargin)
                        anchors.left:parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: rowLeftMargin
                        anchors.top:kbControlChkRow.bottom
                        anchors.topMargin: rowVerticalMargin
                        height: childrenRect.height
                        color: parent.color

                        Grid{
                            id:gridCurrent
                            width: parent.width
                            columns: 2
                            columnSpacing: rowLeftMargin
                            rowSpacing: rowVerticalMargin*0.6

                            Text{
                                id:textKBCurrent
                                //anchors.left: parent.left
                                //anchors.leftMargin: parent.width * 0.02
                                text:"KB Current"
                                //font.bold: true
                                font.pointSize: 12
                                width: rectConnection.width *0.4

                            }
                            TextField {
                                id: textFieldKBCurrent
                                text:"3.00"
                                //width: 70
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                //anchors.left: textKBCurrent.right
                                font.pointSize: 12
                                width: rectConnection.width *0.4

                            }

                            Button{
                                id:buttonRelease
                                text:"Release"
                                width: rectConnection.width *0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }
                            Button{
                                id:buttonFullBrake
                                text:"Full Brake"
                                width: rectConnection.width *0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }

                        }}
                }

                Rectangle{
                    color: "#DCDCDC"
                    id:rectBEMF
                    anchors.top:rectControl.bottom
                    anchors.topMargin:rectGap
                    width: parent.width
                    height: textTitleBEMF.height + rowVerticalMargin +rectGridBEMF2.height

                    Text{
                        id:textTitleBEMF
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width * 0.02
                        text:"BEMF and Current Sampling"
                        font.bold: true
                        font.pointSize: 14
                    }

                    Rectangle{
                        id:rectGridBEMF2
                        width:parent.width - (2 * rowLeftMargin)
                        anchors.left:parent.left
                        anchors.leftMargin: rowLeftMargin
                        anchors.right: parent.right
                        anchors.rightMargin: rowLeftMargin
                        anchors.top: textTitleBEMF.bottom
                        anchors.topMargin: rowVerticalMargin
                        height: gridBEMF.height
                        color: parent.color


                        Grid{
                            id:gridBEMF
                            width: parent.width
                            columns: 2
                            columnSpacing: rowLeftMargin
                            rowSpacing: rowVerticalMargin*0.6

                            Button{
                                id:buttonNow
                                text:"Now"
                                width: rectConnection.width * 0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }
                            Button{
                                id:buttonAtStart
                                text:"At Start"
                                width: rectConnection.width * 0.4
                                onClicked: {

                                }
                                style: ButtonStyle {
                                    label: Text {
                                        renderType: Text.NativeRendering
                                        verticalAlignment: Text.AlignVCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: "Helvetica"
                                        font.pointSize: 12
                                        //color: "blue"
                                        text: control.text
                                    }
                                }
                            }

                            Text{
                                id:textSamples
                                text:"Samples"
                                font.pointSize: 12
                                width: rectConnection.width * 0.4
                            }
                            TextField {
                                id: textFieldKSamples
                                text:"1000"
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                                width: rectConnection.width * 0.4
                            }

                            Text{
                                id:decimation
                                text:"Decimation"
                                font.pointSize: 12
                                width: rectConnection.width * 0.4
                            }
                            TextField {
                                id: textFieldDecimation
                                text:"1"
                                width: rectConnection.width * 0.4
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                            }

                            Text{
                                id:fftText
                                text:"Fs for FFT"
                                font.pointSize: 12
                                width: rectConnection.width * 0.4
                            }
                            TextField {
                                id: textFieldFFT
                                text:"1"
                                width: rectConnection.width * 0.4
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                font.pointSize: 12
                            }



                        }

                    }

                }

                Rectangle{
                    color: "#DCDCDC"
                    id:rectPlotControl
                    anchors.top:rectBEMF.bottom
                    anchors.topMargin:12
                    width: parent.width
                    height: textTitlePlotControl.height + hzoomControlChkRow.height + vzoomControlChkRow.height +buttonsRowPlot.height + 20

                    Text{
                        id:textTitlePlotControl
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width * 0.02
                        text:"Plot Control"
                        font.bold: true
                        font.pointSize: 14
                    }

                    Row {
                        id: hzoomControlChkRow
                        anchors.top:textTitlePlotControl.bottom
                        anchors.topMargin:6
                        anchors.left: parent.left
                        anchors.leftMargin: 32
                        spacing: 8

                        CheckBox {
                            id: hZoomChkBox
                            text: qsTr("Horizontal Zoom")
                            checked: true
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
                    }

                    Row {
                        id: vzoomControlChkRow
                        anchors.top:hzoomControlChkRow.bottom
                        anchors.topMargin:6
                        anchors.left: parent.left
                        anchors.leftMargin: 32
                        spacing: 8

                        CheckBox {
                            id: vZoomChkBox
                            text: qsTr("Vertcial Zoom")
                            checked: true
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
                    }

                    Row{
                        id:buttonsRowPlot
                        anchors.top:vzoomControlChkRow.bottom
                        anchors.topMargin:8
                        anchors.left: parent.left
                        anchors.leftMargin: 32
                        spacing: 8

                        Button{
                            id:buttonRescale
                            text:"Rescale"
                            width: rectConnection.width * 0.4
                            onClicked: {

                            }
                            style: ButtonStyle {
                                label: Text {
                                    renderType: Text.NativeRendering
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.family: "Helvetica"
                                    font.pointSize: 12
                                    //color: "blue"
                                    text: control.text
                                }
                            }
                        }
                        Button{
                            id:buttonReplot
                            text:"Replot"
                            width: rectConnection.width * 0.4
                            onClicked: {

                            }
                            style: ButtonStyle {
                                label: Text {
                                    renderType: Text.NativeRendering
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.family: "Helvetica"
                                    font.pointSize: 12
                                    //color: "blue"
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

