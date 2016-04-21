/**
    NavigationStack provides a stackbased navigation model used within NavigationView

    Author: Ben Lau (benlau)
    License: Apache License
    Project: https://github.com/hilarycheng/quickios
 */

import QtQuick 2.0
import QtQuick.Controls 1.2
import "../utils/objectutils.js" as ObjectUtils

Item {
    id: navigationView

    property ListModel views : ListModel {}

    property var tintColor

    signal pushed(var viewController)
    signal popped(var viewController)

    function push(source,options) {        
        var container = containerFactory.createObject(navigationView);
        var view = ObjectUtils.createObject(source,container,options);
        if (view === undefined) {
            container.destroy();
            return;
        }

        view.anchors.fill = container;

        var topView = top();
        if (topView)
            topView.enabled = false;

        stack.push({ item :container,
                     destroyOnPop: true} // For GC
                   );
        views.append({object: view});
        pushed(view);
        return view;
    }

    function pop() {
        if (stack.depth == 1)
            return;
        stack.pop();
        var view = views.get(views.count - 1).object;
        views.remove(views.count - 1,1);

        var topView = top();
        if (topView)
            topView.enabled = true;
        popped(view);
    }

    function top() {
        var view = null;
        if (views.count > 0) {
            view = views.get(views.count - 1).object
        }
        return view;
    }

    width: 100
    height: 62

    StackView {
        id : stack
        anchors.fill: parent
        delegate: NavigationViewTransition {}
    }

    Component { // Create the container for the pushed ViewController
        id: containerFactory

        Item {
            property var tintColor : navigationView.tintColor;

            Stack.onStatusChanged:  {
                var child = children[0];
                switch (Stack.status) {
                case Stack.Inactive : //0
                    if (child.hasOwnProperty("viewDidDisappear"))
                        child.viewDidDisappear(true);
                    break;
                case Stack.Activating : //2
                    if (child.hasOwnProperty("viewWillAppear"))
                        child.viewWillAppear(true);
                    break;
                case Stack.Active : //3
                    if (child.hasOwnProperty("viewDidAppear"))
                        child.viewDidAppear(true);
                    break;
                case Stack.Deactivating : // 1
                    if (child.hasOwnProperty("viewWillDisappear"))
                        child.viewWillDisappear(true);
                    break;
                }
            }

        }
    }
}
