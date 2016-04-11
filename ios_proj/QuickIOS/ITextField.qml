import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
  id: textFieldWrapper
  height: 30

  property alias placeHolder: placeHolderText.text
  property alias text: inner.text
  property bool passwordMode: false

  TextField {
    id: inner
    anchors.left: parent.left
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: parent.right
    width: parent.width
    height: parent.height
    echoMode: passwordMode ? TextInput.Password : TextInput.Normal

    style: TextFieldStyle {
      font.family: "Helvetica Neue"
      font.pixelSize: 14
      background: Rectangle {
        color: "transparent"
      }
    }

    Text {
      id: placeHolderText
      width: parent.width
      height: parent.height
      anchors.fill: parent
      anchors.leftMargin: 7
      anchors.topMargin: 6
      font: parent.font
      color: "#cccccc"
      renderType: Text.NativeRendering
      text: ""
      visible: parent.text.length > 0 ? false : true
    }

    Rectangle {
      width: parent.width
      height: parent.height
      anchors.fill: parent
      border.color: "#cccccc"
      border.width: 1
      radius: 5
      color: "transparent"
    }
  }

}
