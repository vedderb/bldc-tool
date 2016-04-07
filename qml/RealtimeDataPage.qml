import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import bldc 1.0

BasicPage {
    id:rootRealTimeData
    showTopbar: true
    showBackButton: true
    pageTitle: "Realtime Data"

    Binding{
        target:rootRealTimeData
        property: "pageComponent"
        value:mainComponent
        when:true
    }

    property list<QtObject> listModel1: [
        QtObject{
            property string lValue:"Power"
            property string rValue: mcValues.power + " W"
        },
        QtObject{
            property string lValue:"Duty Cycle"
            property string rValue: mcValues.dutyCycle + " %"
        },
        QtObject{
            property string lValue:"Electrical speed"
            property string rValue: mcValues.electricalSpeed + " rpm"
        },
        QtObject{
            property string lValue:"Battery current"
            property string rValue: mcValues.batteryCurrent + " A"
        },
        QtObject{
            property string lValue:"Motor current"
            property string rValue: mcValues.motorCurrent +" A"
        },
        QtObject{
            property string lValue:"MOFSET Temp"
            property string rValue: mcValues.MOSFET_Temp + " Deg C"
        },
        QtObject{
            property string lValue:"Fault code"
            property string rValue: mcValues.faultString
        }
    ]

    property list<QtObject> listModel2: [
        QtObject{
            property string lValue:"Drawn cap"
            property string rValue: mcValues.drawnCap + " mAh"
        },
        QtObject{
            property string lValue:"Charged cap"
            property string rValue: mcValues.chargedCap + " mAh"
        },
        QtObject{
            property string lValue:"Drawn energy"
            property string rValue: mcValues.drawnEnergy + " Wh"
        },
        QtObject{
            property string lValue:"Charged energy"
            property string rValue: mcValues.chargedEnergy + " Wh"
        },
        QtObject{
            property string lValue:"Tacho"
            property string rValue: mcValues.tacho + " counts"
        },
        QtObject{
            property string lValue:"Tacho ABS"
            property string rValue: mcValues.tachoABS + " counts"
        },
        QtObject{
            property string lValue:"Battery voltage"
            property string rValue: mcValues.batteryVoltage + " V"
        }
    ]

    property Component mainComponent: Item {
        id: mainItem


        Column{
            anchors.top: parent.top
            anchors.topMargin: parent.height*0.05
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: rootRealTimeData.height*0.004
            Rectangle{
                id:rect1
                color: "#A9A9A9"
                width: rootRealTimeData.width*0.96
                height:mainItem.height*0.30
                ListView{
                    //anchors.top: parent.top
                    width: parent.width
                    height: parent.height
                    interactive: false
                    model: listModel1
                    delegate:RealtimeListDelegate{
                        width: parent.width
                        height: rect1.height/ListView.view.count
                    }
                }
            }
            Rectangle{
                id:rect2
                color: "#A9A9A9"
                width: rootRealTimeData.width*0.96
                height:mainItem.height*0.30
                ListView{
                    anchors.top: parent.top
                    width: parent.width
                    height: parent.height
                    interactive: false
                    model: listModel2
                    delegate: RealtimeListDelegate{
                        width: parent.width
                        height: rect2.height/ListView.view.count
                    }
                }
            }
            CheckBox{
                checked: false
                onCheckedChanged: realtimeActivate = checked
                text: "Activate sampling"
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
            Item {width:1;height: parent.height*0.05}//spacer

            Slider{
                id: slider
                width: parent.width
                minimumValue: -128
                maximumValue: 127
                onPressedChanged: packetInterface.setSliderPressState(pressed)
                onValueChanged: packetInterface.setChukData(0,value,0,0,0,0,0)
            }

            Timer {
                id: sliderTimer
                interval: 100
                running: true
                repeat: true
                onTriggered: {
                    if (100 < packetInterface.getElapsedFromSliderChange()) {
                        slider.value = 0;
                    }
                }
            }
        }
    }
}

