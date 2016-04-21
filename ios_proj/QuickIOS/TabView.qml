import QtQuick 2.2
import QtQuick.Window 2.1

Item {
  id: tabView

  width: A.screenWidth
  anchors.bottom: parent.bottom
  anchors.bottomMargin: 0
  anchors.left: parent.left
  anchors.leftMargin: 0
  anchors.right: parent.right
  anchors.rightMargin: 0

  property alias tabsModel: tabBar.tabsModel

  Loader {
    id: tabLoader
    anchors.top: parent.top
    anchors.topMargin: 0
    anchors.bottom: tabBar.top
    anchors.bottomMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0
    anchors.right: parent.right
    anchors.rightMargin: 0
  }

  TabBar {
    id: tabBar
    onTabItemClicked: {
      tabLoader.source = tabBar.tabsModel.children[index].source;
    }
  }

  Component.onCompleted: {
    tabLoader.source = tabBar.tabsModel.children[0].source;
  }


}
