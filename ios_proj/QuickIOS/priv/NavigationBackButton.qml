/// The back button in Navigation Bar
import QtQuick 2.0
import QuickIOS 0.1

BarButtonItem {
    id: backButton
    image: "qrc:///QuickIOS/images/back.png"
    objectName : "NavigationBarBackButton"

    Item {

        states : [
          State {
              when: !backButton.enabled
              PropertyChanges {
                  target: backButton
                  opacity: 0
              }
          },
          State {
                name: "Enabled"
                when: backButton.enabled
                PropertyChanges {
                    target: backButton
                    opacity: 1
                }
          }
        ]

        transitions: [

            Transition {
                from: "*"
                to: "Enabled"
                SequentialAnimation {

                    PauseAnimation {
                        duration: 160
                    }

                    PropertyAnimation {
                        duration: 160
                        target: backButton
                        property: "opacity"
                        from : 0
                        to : 1
                    }
                }

            },
            Transition {
                from: "Enabled"
                to: "*"
                PropertyAnimation {
                    duration: 130
                    target: backButton
                    property: "opacity"
                    to : 0
                }
            }

        ]
    }
}

