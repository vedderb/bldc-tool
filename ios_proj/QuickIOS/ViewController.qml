import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Layouts 1.1
import QuickIOS 0.1
import "./def"
import "./priv"
import "./utils/objectutils.js" as ObjectUtils

Rectangle {
  id: viewController

  property string title : ""

  default property alias content : viewport.data

  property var navigationController
  // It can't set type to NavigationController. It will create cyclic dependenices.

  property NavigationItem navigationItem : NavigationItem {}

  property color tintColor : parent && parent.tintColor ? parent.tintColor : Constant.tintColor

  /// The current toolBar instance
  property alias toolBarItem: toolBar

  /// The assigned item will be placed within the tool bar
  property alias toolBarItems : toolBar.content

  property alias backgroundColor : viewController.color

  /// Specifies whether the view controller prefers the status bar to be hidden or shown.
  // @remarks: Only implemented in presentViewController now
  property bool prefersStatusBarHidden : false

  property int preferredStatusBarUpdateAnimation : 1

  signal viewWillAppear(bool animated)
  signal viewDidAppear(bool animated)
  signal viewWillDisappear(bool animated)
  signal viewDidDisappear(bool animated)

  color: Constant.viewControllerBackgroundColor
  objectName: "ViewController"

  signal pageChanged(var dstItem)

  property Component modelTransitionStyle : ModelTransitionStyleCoverVertical { }

  // The transition component instance for presentViewController and dismissViewController
  property var _modelTransition;

  // present is a wrapper of presentViewController that accept multiple data type include string , Component etc.
  function present(source,options,animated) {
      var view = ObjectUtils.createObject(source,viewController,options);
      if (view)
          presentViewController(view,animated);
      return view;
  }

  function presentViewController(view,animated) {
      var rootController = viewController;

      if (navigationController) {
          rootController = navigationController;
      }

      var root = rootController.parent;

      if (animated === undefined)
          animated = true;

      // Only a kind of transition is supported now.
      var transition = modelTransitionStyle.createObject(view, { container : rootController ,
                                                        nextView : view ,
                                                        prevView : viewController });
      view._modelTransition = transition;

      var coordinator =  ObjectUtils.createObject(Qt.resolvedUrl("./priv/ViewTransitionCoordinator.qml") ,
                                                                    root,
                                                                    {nextView: view,
                                                                     prevView : viewController,
                                                                     container : rootController,
                                                                     transition : transition});

      coordinator.onDismissed.connect(function() {
          viewController.enabled = true;
          if (viewController.navigationController)
              viewController.navigationController.enabled = true;
      });

      coordinator.present(animated);
      viewController.enabled = false;
      if (viewController.navigationController)
          viewController.navigationController.enabled = false;
      return view;
  }

  function dismissViewController(animated) {
      if (animated === undefined)
          animated = true;

      if (animated) {
          viewController._modelTransition.dismissTransition.start();
      } else {
          viewWillDisappear(false);
          _modelTransition.dismissTransitionFinished();
          viewDidDisappear(false);
      }
  }

  // Indicates to the system that the view controller status bar attributes have changed.
  function setNeedsStatusBarAppearanceUpdate() {
      var hidden = prefersStatusBarHidden;
      var animation = preferredStatusBarUpdateAnimation;
      SystemMessenger.sendMessage("applicationSetStatusBarHidden", {
                               hidden: hidden,
                               animation : animation
                           });
  }


  ColumnLayout {
      anchors.fill: parent
      spacing : 0

      Item {
          id: viewport
          property color tintColor : viewController.tintColor
          Layout.fillHeight: true
          Layout.fillWidth: true
      }

      ToolBar {
          id: toolBar
          Layout.fillHeight: true
          Layout.fillWidth: true
          Layout.maximumHeight: height
          Layout.minimumHeight: height
          height: toolBar.content.length > 0 ? 44 :0

          onContentChanged: {
              if (content.length <= 0)
                  return;
              var child = content[0];
              child.anchors.fill = child.parent;
          }
      }
  }

  onParentChanged: {
      if (this.parent === undefined)
          return;
      var p = parent;
      while (p) {
          if (p.hasOwnProperty("navigationController")) {
              navigationController = Qt.binding(function() {
                  return p.navigationController;
              });
              break;
          }
          p = p.parent;
      }
  }

}
