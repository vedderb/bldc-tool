import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import bldc 1.0

BasicPage {
    id:rootMotorConfigSensonLess
    showTopbar: true
    showBackButton: true
    pageTitle: "Motor Configuration(SensorLess)"

    property int headingTopMargin:4
    property int headingLeftMargin:6
    property int rowVerticalMargin:6
    property int rowLeftMargin:16
    property int rowContentSpacing: 10
    property int rectGap: 20
    property int groupMargin: 5

    Binding{
        target:rootMotorConfigSensonLess
        property: "pageComponent"
        value:mainComponent
        when:true
    }

    property Component mainComponent: Rectangle {
        id: name
        color: "#DCDCDC"
        TabView{
            anchors.fill: parent
            Tab{
                id: bldcTab
                title: "BLDC"
                source: "BldcTab.qml"
            }
            Tab{
                id: focTab
                title: "FOC"
                source: "FocTab.qml"
            }
        }
    }
}
