import QtQuick 2.0
import QtQuick.Controls 1.2
import "./def"

Tab {
    property color tintColor : parent && parent.tintColor ? parent.tintColor : Constant.tintColor
}

