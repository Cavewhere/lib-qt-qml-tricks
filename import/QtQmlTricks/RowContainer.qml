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
        var dirty = false;
        for (var idx = 0; idx < children.length; idx++) {
            var child = children [idx];
            if (items.indexOf (child) < 0 && !("delegate" in child)) {
                child.visibleChanged.connect        (relayout);
                child.implicitWidthChanged.connect  (relayout);
                child.implicitHeightChanged.connect (relayout);
                items.push (child);
                dirty = true;
            }
        }
        var tmp = [];
        items.forEach (function (item) {
            if (item && item ["parent"] === layout) {
                tmp.push (item);
            }
            else {
                dirty = true;
            }
        });
        items = tmp;
        if (dirty) {
            relayout ();
        }
    }

    property int spacing : 0;
    property var items   : [];

    default property alias content : layout.data;

    function relayout () {
        var tmpW = 0;
        var tmpH = 0;
        var nbStretch = 0;
        items.forEach (function (child) {
            if (child && child ["visible"]) {
                if (child ["implicitHeight"] > tmpH) {
                    tmpH = child ["implicitHeight"];
                }
                if (tmpW) {
                    tmpW += spacing;
                }
                if (child ["implicitWidth"] >= 0) {
                    tmpW += child ["implicitWidth"];
                }
                else {
                    nbStretch++;
                }
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

    /* CONTENT HERE */
}
