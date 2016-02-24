import QtQuick 2.0

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

    ListModel{
        id:listModel1
        ListElement{
            lValue:"Power"
            rValue:"0.0 W"
        }
        ListElement{
            lValue:"Duty Cycle"
            rValue:"0.20 %"
        }
        ListElement{
            lValue:"Electrical speed"
            rValue:"4.0 rpm"
        }
        ListElement{
            lValue:"Battery current"
            rValue:"0.00 A"
        }
        ListElement{
            lValue:"Motor current"
            rValue:"-0.20 A"
        }
        ListElement{
            lValue:"MOFSET Temp"
            rValue:"34.40 Deg C"
        }
        ListElement{
            lValue:"Fault code"
            rValue:"NONE"
        }
    }

    ListModel{
        id:listModel2
        ListElement{
            lValue:"Drawn cap"
            rValue:"32.4 mAh"
        }
        ListElement{
            lValue:"Charged cap"
            rValue:"0.8 mAh"
        }
        ListElement{
            lValue:"Drawn energy"
            rValue:"0.67 Wh"
        }
        ListElement{
            lValue:"Charged energy"
            rValue:"0.02 Wh"
        }
        ListElement{
            lValue:"Tacho"
            rValue:"17330 counts"
        }
        ListElement{
            lValue:"Tacho ABS"
            rValue:"56406 counts"
        }
        ListElement{
            lValue:"Battery voltage"
            rValue:"21.1 V"
        }
    }

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
                        height: rect1.height/listModel1.count
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
                        height: rect2.height/listModel2.count
                    }
                }
            }
        }
    }
}

