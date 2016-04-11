.pragma library

// A helper to create object from QML file / Component.
function createObject(source,parent,options) {
    var view;

    if (typeof source === "string") {
        var comp = Qt.createComponent(source);
        if (comp === undefined || comp.status === 3) { // Component.Error
            console.warn("Error loading QML source: ",source);
            console.warn(comp.errorString());
            return;
        }
        view = comp.createObject(parent,options || {});
    } else if ( String(source).indexOf("QQmlComponent") === 0) {
        // It is a component object
        view = source.createObject(parent,options || {});
        if (view === null) {
            console.warn(source.errorString());
            return;
        }
    } else { // It is an already created object
        view = source;
        view.parent = parent;
    }

    return view;
}
