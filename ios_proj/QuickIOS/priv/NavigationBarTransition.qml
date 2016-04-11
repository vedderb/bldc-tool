import QtQuick 2.0
import QtQuick.Controls 1.2

StackViewDelegate {
    id: root

    readonly property int duration : 400

    readonly property int movingEasing : Easing.OutQuart
    readonly property int opacityEasing : Easing.OutQuart

    function transitionFinished(properties)
    {
        properties.exitItem.x = 0
        properties.exitItem.y = 0
    }

    pushTransition: StackViewTransition {
       PropertyAnimation {
           target: enterItem
           property: "x"
           from: target.width
           to: 0
           duration: root.duration
           easing.type: movingEasing
       }

       PropertyAnimation {
           target: enterItem.titleView
           property: "opacity"
           from : 0
           to : 1
           duration: root.duration
           easing.type: opacityEasing
       }

       PropertyAnimation {
           target: exitItem
           property: "x"
           from: 0
           to: -target.width
           duration: root.duration
           easing.type: movingEasing

       }

       PropertyAnimation {
           target: exitItem.titleView
           property : "opacity"
           from : 1
           to : 0
           duration: root.duration
           easing.type:opacityEasing

       }
    }

    popTransition: StackViewTransition {
       PropertyAnimation {
           target: enterItem
           property: "x"
           from: -target.width
           to: 0
           duration: root.duration
           easing.type: movingEasing
       }

       PropertyAnimation {
           target: enterItem.titleView
           property : "opacity"
           from : 0
           to : 1
           duration: root.duration
           easing.type: opacityEasing
       }

       PropertyAnimation {
           target: exitItem
           property: "x"
           from: 0
           to: target.width
           duration: root.duration
           easing.type: movingEasing
       }
   }

    replaceTransition: pushTransition
}
