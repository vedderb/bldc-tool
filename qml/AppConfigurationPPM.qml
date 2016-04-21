import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import bldc 1.0
BasicPage {
    id:rootAppConfigPPM
    showTopbar: true
    showBackButton: true
    pageTitle: "App Configuration(control)"
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
        TabView{
            id: tabveiew
            frameVisible: false
            anchors.fill: parent

            Tab{
                id: ppmTab
                title: "PPM"
                PpmTab{
                    anchors.fill: parent
                }

            }

            Tab{
                id: nunchukTab
                title: "nunchuk"
                NunchukTab{
                    anchors.fill: parent
                }
            }


        }
    }
}
