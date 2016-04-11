/*
 NavigationController provides views management like UINavigationController in iOS

  Author: Ben Lau (benlau)
  License: Apache License
  Project: https://github.com/hilarycheng/quickios

 */

import QtQuick 2.2
import "./priv"

ViewController {
    id : navigationController

    /// The instance of NavigationBar
    property alias navigationBar : navBar

    /* The first view that should be shown when the NavigationController is created.
       It should be an object. Component and string source is not allowed. It is
       just a convenience for writing Component.onCompleted: push()

       Moreover, don't change the value after created or your have pushed any view already.
     */
    property var initialViewController

    property var topViewController : null

    property var viewControllers : new Array

    signal pushed(ViewController viewController)
    signal popped(ViewController viewController)

    // Create ViewController from source file or Component then push it into the stack.
    function push(source,options) {
        // Just like the present() and presentViewController() in ViewController. QuickIOS offer a solution
        // that will create the component for you. Those functions will not has suffix of "viewController"
        return stack.push(source,options);
    }

    function pop() {
        stack.pop();
    }

    NavigationBar {
        id : navBar
        views: stack.views
        tintColor: navigationController.tintColor
        onBackClicked: stack.pop(true);
        z: stack.z + 1
    }

    NavigationStack {
        id : stack
        anchors.top: navBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        tintColor : navigationController.tintColor

        onPushed: {
            // Attach navigationController to a newly created view
            if (viewController.hasOwnProperty("navigationController"))
                viewController.navigationController = navigationController;

            topViewController = viewController;
            viewControllers.push(viewController);
            navigationController.viewControllersChanged();
            navigationController.pushed(viewController);
        }

        onPopped: {
            var topView = null;
            if (views.count > 0) {
                topView = views.get(views.count - 1).object;
            }
            viewControllers.splice(viewControllers.length - 1 , 1);
            navigationController.viewControllersChanged();
            topViewController = topView;

            navigationController.popped(viewController);
        }
    }

    Component.onCompleted: {        
        if (initialViewController) {
            // Push after the component is completed so that everything has been initialized.
            stack.push(initialViewController);
        }
    }

}
