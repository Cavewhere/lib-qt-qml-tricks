import QtQuick 2.0;

/**
 * @brief Item that layouts its children within a row, respecting size hints and spacers.
 *
 */
Item {
    id: layout;
    width: implicitWidth;
    height: implicitHeight;
    onWidthChanged: { relayout (); }
    onChildrenChanged: {
        if (layout && layout.ready) {
            var tmp = [];
            var dirty = false;
            items.forEach (function (item) {
                if (item && item ["parent"] === layout) {
                    tmp.push (item);
                }
                else {
                    dirty = true;
                }
            });
            for (var idx = 0; idx < children.length; idx++) {
                var child = children [idx];
                if (items.indexOf (child) < 0 && !("delegate" in child)) {
                    child.visibleChanged.connect        (relayout);
                    child.implicitWidthChanged.connect  (relayout);
                    child.implicitHeightChanged.connect (relayout);
                    tmp.push (child);
                    dirty = true;
                }
            }
            if (dirty) {
                items = tmp;
                relayout ();
            }
        }
    }
    Component.onCompleted: {
        ready = true;
        layout.childrenChanged ();
    }
    Component.onDestruction: {
        ready = false;
    }

    property int spacing : 0;
    property var items   : [];

    property bool ready : false;

    default property alias content : layout.data;

    function relayout () {
        if (layout && layout.ready) {
            var tmpW = 0;
            var tmpH = 0;
            var nbStretch = 0;
            var nb = 0;
            items.forEach (function (child) {
                if (child && child ["visible"]) {
                    if (child ["implicitHeight"] > tmpH) {
                        tmpH = child ["implicitHeight"];
                    }
                    if (nb) {
                        tmpW += spacing;
                    }
                    if (child ["implicitWidth"] >= 0) {
                        tmpW += child ["implicitWidth"];
                    }
                    else {
                        nbStretch++;
                    }
                    nb++;
                }
            });
            implicitWidth = tmpW;
            implicitHeight = tmpH;
            var autoSize = (nbStretch > 0 ? (width - implicitWidth) / nbStretch : 0);
            var currX = 0;
            items.forEach (function (child) {
                if (child && child ["visible"]) {
                    if (currX) {
                        currX += spacing;
                    }
                    child ["x"] = currX;
                    child ["width"] = (child ["implicitWidth"] >= 0 ? child ["implicitWidth"] : autoSize);
                    currX += child ["width"];
                }
            });
        }
    }

    /* CONTENT HERE */
}
