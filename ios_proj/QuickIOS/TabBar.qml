import QtQuick 2.2
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0

Rectangle {
  id: tabBar

  property int tabsHeight: 48
  property int tabIndex: 0

  property VisualItemModel tabsModel

  signal tabItemClicked(int index)

  width: parent.width
  height: 49

  color: "#f8f8f8"

  anchors.bottom: parent.bottom
  anchors.bottomMargin: 0
  anchors.right: parent.right
  anchors.rightMargin: 0
  anchors.left: parent.left
  anchors.leftMargin: 0

  Rectangle {
    x: 0
    y: 0
    width: parent.width
    height: 1
    color: "#acacac"
  }

  Rectangle {
    id: tabContainer

    x: 0
    y: 1
    width: parent.width
    height: parent.height - 1

    Repeater {
      model: tabsModel
    }
  }

  Component {
    id: tabBarItem

    Rectangle {
      height: tabsHeight
      width: tabs.width / tabsModel.count
      color: "transparent"

      Image {
        id: tabImage
        source: tabsModel.children[index].icon
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 5
      }

      ColorOverlay {
        anchors.fill: tabImage
        source: tabImage
        color: tabsModel.children[index].selected ? "#007aff" : "#9c9c9c"
      }

      Text {
        font.family: "Helvetica Neue"
        renderType: Text.NativeRendering
        font.pixelSize: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        color: tabsModel.children[index].selected ? "#007aff" : "#929292"
        text: tabsModel.children[index].name
      }

      MouseArea {
        anchors.fill: parent
        onClicked: {
          for (var i = 0; i < tabsModel.count; i++) tabsModel.children[i].selected = false;
          tabsModel.children[index].selected = true;
          tabBar.tabItemClicked(index);
        }
      }
    }
  }

  Rectangle {
    id: tabBarRect
    width: parent.width
    height: parent.height - 1
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    Row {
      anchors.fill: parent
      id: tabs
      Repeater {
        model: tabsModel.count
        delegate: tabBarItem
      }
    }
  }

  Component.onCompleted: {
    tabsModel.children[0].selected = true
  }

}
