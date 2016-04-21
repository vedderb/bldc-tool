import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQml.Models 2.1
import QuickIOS 0.1
import "./priv"
import "./def"

Rectangle {
  id: navigationBar

  // The tint color to apply to the navigation bar background. It is equivalent to color. Follow the naming convenient of UIKit
  property string tintColor : Constant.tintColor

  property alias barTintColor : navigationBar.color

  // The view objects within NavigationView
  property ListModel views : ListModel{}

  // The top most navigation item in navigationItems
  property NavigationItem navigationItem : dummyNavigationItem

  property ListModel navigationItems : ListModel{}

  property NavigationBarTitleAttributes titleAttributes : NavigationBarTitleAttributes {}

  signal backClicked()

  // The current title
  property string currentTitle : stack.topBarItem.title

  // The current title item
  property var currentTitleItem : stack.topBarItem.titleView

  // The current list of left buttons
  property var currentLeftButtonItems : navigationItem.leftBarButtonItems

  // The current list of right buttons
  property var currentRightButtonItems : navigationItem.rightBarButtonItems

  width: parent.width
  height: QIDevice.screenFillStatusBar ? 44 + 20 : 44

  color : "#f8f8f8"

  anchors.top: parent.top
  anchors.topMargin: 0
  anchors.right: parent.right
  anchors.rightMargin: 0
  anchors.left: parent.left
  anchors.leftMargin: 0

  NavigationItem {
    id : dummyNavigationItem
  }

  NavigationBarItem {
      id : dummyNavigationBarItem
  }

  ObjectModel {
      id : dummyItemModel;
  }

  MouseArea {
      anchors.fill: parent // block all the mouse event
  }

  StackView {
      id : stack
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      height : 44
      delegate: NavigationBarTransition {}

      property NavigationBarItem topBarItem : dummyNavigationBarItem

      onDepthChanged: {
          var index = depth -1;
          if (index < 0) {
              currentTitle = "";
              topBarItem = dummyNavigationBarItem;
          } else {
              var barItem = get(index);
              topBarItem = barItem;
          }
      }
  }

  Item {
      anchors.left: parent.left
      anchors.bottom: parent.bottom

      width: 22 + 16;
      height: 44

      NavigationBackButton {
          id: backButton
          enabled: views.count > 1 && navigationItem.backButtonVisible
          tintColor: navigationBar.tintColor
          anchors.fill: parent

          onClicked: {
              navigationBar.backClicked();
          }
      }
  }

  Repeater {
      id: viewsListener
      model : navigationBar.views
      delegate: Item {
          id: item

          property ViewController source ; ViewController {}
          property var navigationItem : source.navigationItem
          property string title : source.title;

          Component {
              id: creator
              NavigationBarItem {
                  title: item.title
                  backStage: index > 0
                  leftBarButtonItems: navigationItem.leftBarButtonItems
                  rightBarButtonItems: navigationItem.rightBarButtonItems
                  barTintColor : navigationBar.barTintColor
                  titleView.color: titleAttributes.textColor
              }
          }

          function setup(model) {
              for (var i = 0 ; i < model.children.length; i++) {
                  var child = model.children[i];
                  if (child.hasOwnProperty("tintColor")) {
                      child.tintColor = Qt.binding(function() {return navigationBar.tintColor;});
                  }

                  (function(item) {
                      item.height = navigationBar.height;
                  })(child);
              }
          }

          onNavigationItemChanged:  {
              var data = { object: navigationItem}
              if (index >= navigationItems.count) {
                  navigationItems.append(data);
              } else {
                  navigationItems.set(index,data);
              }

              if (index === navigationItems.count - 1) // it is top most
                  navigationBar.navigationItem = navigationItem;

              if (!navigationItem) {
                  return;
              }

              setup(navigationItem.rightBarButtonItems);
              setup(navigationItem.leftBarButtonItems);
          }

          Component.onCompleted: {
              source = model.object;

              var object = creator.createObject(stack);

              stack.push({item : object , destroyOnPop : true});
          }

          Component.onDestruction: {
              stack.pop();
              navigationItems.remove(navigationItems.count - 1);
              if (navigationItems.count > 0)
                navigationBar.navigationItem = navigationItems.get(navigationItems.count - 1).object;
              else
                navigationBar.navigationItem = dummyNavigationItem;
          }
      }
  }

}
