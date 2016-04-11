/* SegmentedControl implements the UISegmentedControl in QML way.

  The limitation:

  1. It don't support to modify the detailed style yet.
     Only tintColor is supported

  2. It can't use image as the tab icon.

 */
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQml.Models 2.1
import QtQuick.Controls 1.2 as QuickControl
import "./priv"
import "./def"

ViewController {
    id: segmentedControl

    default property alias __children : tabView.children
    property alias count : tabView.count
    property alias currentIndex : tabView.currentIndex
    property alias numberOfSegments : tabView.count
    property alias selectedSegmentIndex : tabView.currentIndex

    property color barTintColor : "#ffffff"

    /// Return the item segmented view at index.
    function itemAt(index) {
        return tabView.getTab(index).children[0];
    }

    MouseArea {
        anchors.fill: parent
    }

    TabView {
        id: tabView
        anchors.fill: parent

        property int previousIndex : -1

        style: SegmentedControlTabViewStyle {
            id: tabViewStyle
            tintColor : segmentedControl.tintColor
            backgroundColor : barTintColor
        }

        onCurrentIndexChanged: {
            var tab = getTab(currentIndex);
            tab.tintColor = Qt.binding(function() { return segmentedControl.tintColor});

            if (previousIndex >= 0) {
                var prevTab = getTab(previousIndex);
                if (prevTab.children.length > 0)
                    emitDisappear(prevTab.children[0]);
            }

            if (tab.children.length > 0)
                emitAppear(tab.children[0]);
            previousIndex = currentIndex;
        }

        function emitDisappear(tab) {
            if (tab.hasOwnProperty("viewWillDisappear"))
                tab.viewWillDisappear(false);

            if (tab.hasOwnProperty("viewDidDisappear"))
                tab.viewDidDisappear(false);
        }

        function emitAppear(tab) {
            if (tab.hasOwnProperty("viewWillAppear"))
                tab.viewWillAppear(false);

            if (tab.hasOwnProperty("viewDidAppear"))
                tab.viewDidAppear(false);
        }

        Component.onCompleted: {
            var tab = getTab(currentIndex);
            tab.tintColor = Qt.binding(function() { return segmentedControl.tintColor});
            if (tab.children.length > 0)
                emitAppear(tab.children[0]);
            previousIndex = currentIndex;
        }
    }


}
