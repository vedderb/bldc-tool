// NavigationBarItem is a children of NavigationBar to display the navigation controls
// for the top view.
import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import "../def"

Rectangle {
  id: navigationBarItem

  // TRUE if the back button should be visible
  property bool backStage: false

  property alias barTintColor : navigationBarItem.color

  property alias title: navigationTitle.text
  property alias titleView : navigationTitle

  property VisualItemModel leftBarButtonItems : VisualItemModel {}

  property VisualItemModel rightBarButtonItems : VisualItemModel {}

  signal leftClicked()
  signal rightClicked()

  color: Constant.barTintColor

  property int _titleLeftMargin : leftBarArea.x + leftBarArea.childrenRect.width
  property int _titleRightMargin : rightBarArea.x
  property int _titleMargin : Math.max(_titleLeftMargin,_titleRightMargin)

  Text {
    id: navigationTitle
    font.family: "Helvetica Neue"
    renderType: Text.NativeRendering
    text: ""
    font.weight: Font.Bold
    width: parent.width - _titleMargin / 2
    height: parent.height - 1
    wrapMode: Text.NoWrap
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    anchors.horizontalCenter: parent.horizontalCenter

    color: "#000000"
    font.pixelSize: 16
    scale: paintedWidth > width ? (width / paintedWidth) : 1
  }

  Item {
      // The area reserved for right bar.
      id : leftBarArea
      anchors.left: parent.left
      anchors.leftMargin: backStage ? 22 + 16 : 0
      anchors.top: parent.top
      anchors.bottom: parent.bottom

      Row {
          anchors.verticalCenter: parent.verticalCenter
          Repeater {
              model : leftBarButtonItems
          }
      }
  }

  Item {
      // The area reserved for right bar.
      id : rightBarArea
      x: parent.width - rightBarRepeater.width
      anchors.top: parent.top
      anchors.bottom: parent.bottom

      Row {
          id: rightBarRepeater
          anchors.verticalCenter: parent.verticalCenter
          Repeater {
              model : rightBarButtonItems
          }
      }
  }

  Rectangle {
    x: 0
    y: parent.height - 1
    width: parent.width
    height: 1
    color: "#acacac"
  }

}
