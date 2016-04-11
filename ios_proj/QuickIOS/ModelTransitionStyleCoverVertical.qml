/*
Author: Ben Lau (benlau)
License: Apache License
Project: https://github.com/hilarycheng/quickios
*/
import QtQuick 2.0
import "./priv"

QtObject {
    id: transition

    /// The container provide the geometry information
    property Item container : null;

    /// The view going to be presented or dismissed.
    property Item nextView : Item {}

    /// The original view.
    property Item prevView : Item {}

    readonly property int duration : 300;

    signal aboutToDismiss
    signal dismissed
    signal aboutToPresent
    signal presented

    property int _height : nextView ? nextView.height : 0

    function presentTransitionFinished() {
        nextView.x = Qt.binding(function() { return container ? container.x  : 0});
        nextView.y = Qt.binding(function() { return container ? container.y : 0});
        nextView.width = Qt.binding(function() { return container ? container.width : 0 });
        nextView.height = Qt.binding(function() { return container ? container.height : 0});
        prevView.enabled = false;
        nextView.enabled = true;
    }

    function dismissTransitionFinished() {
        nextView.visible = false;
        nextView.enabled = false;
        prevView.enabled = true;
    }

    property var presentTransition: ParallelAnimation {

        PropertyAnimation {
            target: nextView
            property: "y"
            from: _height
            to: 0
            duration: transition.duration
            easing.type: Easing.OutExpo
            alwaysRunToEnd : true
        }

        onStarted: aboutToPresent();

        onStopped: {
            presented();
        }
    }

    property var dismissTransition: ParallelAnimation {
        PropertyAnimation {
            target: nextView
            property: "y"
            from: 0
            to: _height
            duration: transition.duration
            easing.type: Easing.Linear
        }

        onStarted: aboutToDismiss();

        onStopped: {
            dismissed();
        }
    }

}

