import QtQuick 2.4

BasicPage {
    id:rootMainMenu
    showBackButton: false
    pageTitle: "Dashboard"

    property string tab1Text: "Realtime Data"
    property string tab2Text: "App Configuration"
    property string tab3Text: "Motor Configuration(Limits)"
    property string tab4Text: "Motor Configuration(SensorLess)"
    property string tab5Text: "Firmware"

    Binding {
        target: rootMainMenu
        property: "pageComponent"
        value: mainViewComponent
        when:true
    }

    property Component mainViewComponent: Item {
        id:mainMenuItem
        Rectangle {
            id:rectMain
            anchors.fill: parent
            color: "#DCDCDC"

            Grid {
                anchors.centerIn: parent
                columns: 2
                spacing: 20

                Image {
                    height:mainWindow.height > mainWindow.width ? rectMain.height *0.16 : rectMain.height * 0.23
                    width: height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/images/6.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            destinationPage = "tab1"
                            nextPage()
                        }
                    }
                }

                Image {
                   height:mainWindow.height > mainWindow.width ? rectMain.height *0.16 : rectMain.height * 0.23
                    width: height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/images/2.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            destinationPage = "tab2"
                            nextPage()
                        }
                    }
                }

                Image {
                    height:mainWindow.height > mainWindow.width ? rectMain.height *0.16 : rectMain.height * 0.23
                    width: height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/images/5.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            destinationPage = "tab3"
                            nextPage()
                        }
                    }
                }

                Image {
                   height:mainWindow.height > mainWindow.width ? rectMain.height *0.16 : rectMain.height * 0.23
                    width: height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/images/4.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            destinationPage = "tab4"
                            nextPage()
                        }
                    }
                }

                Image {
                    height:mainWindow.height > mainWindow.width ? rectMain.height *0.16 : rectMain.height * 0.23
                    width: height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/images/3.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            destinationPage = "tab5"
                            nextPage()
                        }
                    }
                }

                Image {
                   height:mainWindow.height > mainWindow.width ? rectMain.height *0.16 : rectMain.height * 0.23
                    width: height
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/qml/images/1.png"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            destinationPage = "tab6"
                            nextPage()
                        }
                    }
                }
            }
        }
    }
}


