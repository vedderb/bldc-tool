import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

//import bldc 1.0

RadioButton {
    property int firmwareSource

    id: firmwareRadioButton

    checked: firmwareCurrentSource === firmwareSource
    onClicked: {
        firmwareCurrentSource = firmwareSource
    }

    style: RadioButtonStyle{
        label: Text {
            renderType: Text.NativeRendering
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: "Helvetica"
            font.pointSize: 12
            text: control.text
        }
    }
}
