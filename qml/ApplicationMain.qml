import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Rectangle {
    id:rootApplicationMain
    height: mainWindow.height
    width: mainWindow.width

    property string destinationPage: ""

    property Component settingsPage:SettingsPage{
     onPreviousPage: {
         pageView.pop()
     }
    }

    property Component realtimeDataPage: RealtimeDataPage{
        onPreviousPage: {
            pageView.pop()
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }
    property Component appConfigurationPage: AppConfigurationPage{
        onPreviousPage: {
            pageView.pop()
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }
    property Component motorConfLimitsPage: MCLimits{
        onPreviousPage: {
            pageView.pop()
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }
    property Component motorConfigSensorLess: MCSensorLess{
        onPreviousPage: {
            pageView.pop()
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }
    property Component firmwarePage: FirmwarePage{
        onPreviousPage: {
            pageView.pop()
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }
    property Component appConfigurationPPM:AppConfigurationPPM{
        onPreviousPage: {
            pageView.pop()
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }


    property Component mainMenusPage: MainMenu{
        onNextPage: {
            if(destinationPage === "tab1"){
                pageView.push(realtimeDataPage)
            }
            else if(destinationPage === "tab2"){
                pageView.push(appConfigurationPage)
            }
            else if(destinationPage === "tab3"){
                pageView.push(motorConfLimitsPage)
            }
            else if(destinationPage === "tab4"){
                pageView.push(motorConfigSensorLess)
            }
            else if(destinationPage === "tab5"){
                pageView.push(firmwarePage)
            }
            else if(destinationPage === "tab6"){
                pageView.push(appConfigurationPPM)
            }
        }
        onShowSettings: {
            pageView.push(settingsPage)
        }
    }

    StackView {
        id: pageView
        anchors.fill: parent
        initialItem: mainMenusPage
        delegate: StackViewDelegate {
            pushTransition: StackViewTransition {
                function transitionFinished(properties)
                {
                    properties.exitItem.opacity = 1
                }
                PropertyAnimation {
                    target: enterItem
                    property: "x"
                    from: target.width
                    to: 0
                    duration: 100
                    easing.type: Easing.OutSine
                }
                PropertyAnimation {
                    target: exitItem
                    property: "x"
                    from: 0
                    to: -target.width
                    duration: 100
                    easing.type: Easing.OutSine
                }
            }
            popTransition: StackViewTransition {
                function transitionFinished(properties) {
                    properties.exitItem.opacity = 1
                }
                PropertyAnimation {
                    target: enterItem
                    property: "x"
                    from: -target.width
                    to: 0
                    duration: 100
                    easing.type: Easing.OutSine
                }
                PropertyAnimation {
                    target: exitItem
                    property: "x"
                    from: 0
                    to: target.width
                    duration: 100
                    easing.type: Easing.OutSine

                }
            }
            property Component replaceTransition: pushTransition
        }
    }

}

