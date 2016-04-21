import QtQuick 2.0
import QtQuick.Controls 1.2

StackViewDelegate {
    id: root

    readonly property int duration : 460
    readonly property int easingType : Easing.OutExpo
    readonly property real margin : 0.38

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
           easing.type: easingType
       }

       PropertyAnimation {
           target: exitItem
           property: "x"
           from: 0
           to: -target.width * root.margin
           easing.type: easingType
           duration: root.duration
       }
    }

    popTransition: StackViewTransition {
       PropertyAnimation {
           target: enterItem
           property: "x"
           from: -target.width * root.margin
           to: 0
           duration: root.duration
           easing.type:  easingType
       }
       PropertyAnimation {
           target: exitItem
           property: "x"
           from: 0
           to: target.width
           duration: root.duration
           easing.type:  easingType
       }
   }

    replaceTransition: pushTransition
}
