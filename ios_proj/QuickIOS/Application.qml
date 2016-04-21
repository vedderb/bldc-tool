import QtQuick 2.2
import QtQuick.Controls 1.2

Rectangle {
  id: application

  property alias delegate: stack.delegate

  color: "#ffffff"

  Loader {
    id: loader
    anchors.fill: parent
    z: 0
  }

  StackView {
    id: stack
    anchors.fill: parent
    z: 1
  }

  Component.onCompleted: {
//    AppDelegate.application = application;
  }

  function rootView(rootView) {
    loader.source = rootView;
  }

  function presentView(view) {
    stack.push(view);
  }

  function dismissView() {
    // If Stack Got 1 Item only, stack.pop will not work, use clear instead
    if (stack.depth == 1)
      stack.clear();
    else
      stack.pop(null);
  }

}
