// NavigationItem object manages the buttons and views to be displayed in a NavigationBar object.
// Each view pushed into the NavigationController object may contains a NavigationItem object
// with buttons and views wants displayed in the navigation bar

// The NavigationItem simulates the behavior like the UINavigationItem in UIKit.

// Author: Ben Lau (benlau)

import QtQuick 2.0
import QtQml.Models 2.1

Item {
    property bool backButtonVisible : true

    // Same as leftBar
    property alias leftBarButtonItem : leftBarButtonItemModel.children

    property var leftBarButtonItems : ObjectModel {
        id: leftBarButtonItemModel
    }

    // Same as rightBar
    property alias rightBarButtonItem : rightBarButtonItemModel.children

    property var rightBarButtonItems : ObjectModel {
        id: rightBarButtonItemModel
    }

}
