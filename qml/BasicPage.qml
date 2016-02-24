import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Item {
    id:rootBasicPage

    signal nextPage
    signal previousPage
    signal showSettings
    height: mainWindow.height
    width:mainWindow.width

    property bool showTopbar: true
    property bool showBackButton: true
    property bool showSettingsButton: true
   // property bool showSettingsModal: false
    property string pageTitle: ""
    property Component pageComponent
//    onShowSettingsModalChanged: {
//        if(showSettingsModal === true){
//            game_translate_.y = _settingsControlBase.height+_topToolbar.height
//        }
//        else{
//            game_translate_.y = -( _settingsControlBase.height+_topToolbar.height)
//        }
//    }
    Item{
        id:itemBasicPage
        height: parent.height
        width: parent.width
        Rectangle {
            id: _topToolbar
            visible:showTopbar
            anchors.top: parent.top
            height:mainWindow.height *0.1
            z:2
            width: parent.width
            color: "#ECECEC"
            //            Image {
            //                id: backButtonImage
            //                visible: true
            //                source: "qrc:/qml/images/back_normal.svg"
            //                height :parent.height*0.5
            //                width: height
            //                anchors.left: parent.left
            //                anchors.leftMargin: width*0.7
            //                anchors.verticalCenter: parent.verticalCenter
            //                //fillMode: Image.PreserveAspectFit

            //                MouseArea{
            //                    anchors.fill: parent
            //                    onClicked:{
            //                        rootBasicPage.previousPage()
            //                    }
            //                }

            //            }
            Rectangle{
                id:rectback
                visible: showBackButton
                width: childrenRect.width
                height: childrenRect.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: width*0.3
                color: parent.color
                Text{
                    anchors.centerIn: parent
                    text:"Back"
                    font.pointSize: 14
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                        rootBasicPage.previousPage()
                    }
                }
            }
            Text{
                color: "#000000"
                //anchors.left: rectback.right
                //anchors.right: rectSettings.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: pageTitle
                elide: Text.ElideMiddle
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle{
                id:rectSettings
                width: childrenRect.width
                height: childrenRect.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: width*0.2
                color: parent.color
                visible: showSettingsButton
                Image {
                    anchors.centerIn: parent
                    height:_topToolbar.height * 0.45
                    width:height
                    source: "qrc:/qml/images/gear-two-7@3x.png"
                }

//                Text{
//                    anchors.centerIn: parent
//                    text:"Settings"
//                    font.pointSize: 14
//                }
                MouseArea{
                    anchors.fill: parent
                    onClicked:{
                       showSettings()
                    }
                }
            }
        }
//        Rectangle {
//            color: "#DCDCDC"
//            id: _settingsControlBase
//            z: _topToolbar.z+1
//            y: -height-_topToolbar.height
//            //x: parent.width/2 - width/2
//           x :0
//            height: mainWindow.height
//           // width: Math.min( mainWindow.width, mainWindow.height )
//            width :mainWindow.width
//            transform: Translate {
//                id: game_translate_
//                y: -height
//                Behavior on y { NumberAnimation { duration: 600; easing.type: Easing.OutQuad } }
//            }

//            SettingsPage
//            {
//                id: _settings
//                onBackButtonTapped: {
//                    showSettingsModal = false
//                }

//            }
//        }
        Loader {
            id: pageLoader
            sourceComponent: pageComponent
            anchors.top: (showTopbar===true) ? _topToolbar.bottom : parent.top
            anchors.bottom: parent.bottom
            width: parent.width

        }
    }

}

