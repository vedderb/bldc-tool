// It is a coordinator for handing the transition triggered by presentViewController()
import QtQuick 2.0

Item {
    id: coordinator
    x: 0
    y: 0
    width: parent.width
    height: parent.height

    property var container;

    // The previous view.
    property var prevView

    // The view that is going to present
    property var nextView;
    property var transition;

    signal dismissed();

    function present(animated) {
        if (animated) {
            nextView._modelTransition.presentTransition.start();
        } else {
            nextView.viewWillAppear(false);
            transition.presentTransitionFinished();
            nextView.viewDidAppear(false);
        }
    }

    function dismiss() {
        nextView._modelTransition.dismissTransition.start();
    }

    function setStatusBarHidden(view) {
        var hidden = view.prefersStatusBarHidden;
        var animation = view.preferredStatusBarUpdateAnimation;
        SystemMessenger.sendMessage("applicationSetStatusBarHidden", {
                                 hidden: hidden,
                                 animation : animation
                             });
    }

    Item {
        // Provide tintColor to view but it don't let enabled change propagate to the newView
        id : bridge
        property color tintColor: prevView.tintColor
    }

    Connections {
        target: transition

        onAboutToPresent: {
            nextView.viewWillAppear(true);
        }

        onPresented: {
            transition.presentTransitionFinished();

            nextView.viewDidAppear(true);
        }

        onAboutToDismiss: {
            nextView.viewWillDisappear(true);
        }

        onDismissed: {
            transition.dismissTransitionFinished();

            nextView.viewDidDisappear(true);

            coordinator.nextView._modelTransition.destroy(); // Destroy the transition object for GC
            coordinator.destroy();
        }
    }

    Connections {
        target: nextView
        ignoreUnknownSignals: true
        onViewWillAppear: {
            nextView.setNeedsStatusBarAppearanceUpdate();
        }

        onViewWillDisappear: {
            prevView.setNeedsStatusBarAppearanceUpdate();
            coordinator.dismissed();
        }
    }

    onNextViewChanged: {
        if (nextView) {
            nextView.parent = bridge;
            nextView.width = Qt.binding(function() {return container.width });
            nextView.height = Qt.binding(function() {return container.height });
        }
    }
}

