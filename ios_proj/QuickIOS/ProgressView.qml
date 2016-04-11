import QtQuick 2.2
import QtQuick.Window 2.1

Rectangle {
  id: progressView
  width: 75
  height: 2
  color: "#9c9c9c"

  property int minimum: 0
  property int maximum: 100
  property int current: 50

  Rectangle {
    id: running
    anchors.left: parent.left
    width: 10
    height: 2
    color: "#007aff"
  }

  Component.onCompleted: {
    update();
  }

  function update() {
    var total = maximum - minimum;
    if (total > 0) {
      if (current == maximum) {
        running.width = progressView.width;
      } else {
        running.width = (current / total) * progressView.width;
      }
    }
  }


}
