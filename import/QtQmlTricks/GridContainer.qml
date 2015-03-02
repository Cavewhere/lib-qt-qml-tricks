import QtQuick 2.0;

/**
 * @brief Item that layouts its children within a grid according to row/column count.
 *
 */
Item {
    id: layout;
    width: implicitWidth;
    height: implicitHeight;
    onColsChanged: { relayout (); }
    onWidthChanged: { relayout (); }
    onHeightChanged: { relayout (); }
    onCapacityChanged: { relayout (); }
    onFillEmptyChanged: { relayout (); }
    onVerticalFlowChanged: { relayout (); }
    onInvertDirectionChanged: { relayout (); }
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
            items = tmp;
            if (dirty) {
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

    property int cols : 1;
    property int rows : 1;

    property int rowSpacing : 0;
    property int colSpacing : 0;

    property int capacity : -1;

    property bool ready : false;

    property bool fillEmpty       : true;
    property bool verticalFlow    : false;
    property bool invertDirection : false;

    property var items : [];

    function relayout () {
        if (layout && layout.ready) {
            var count = 0;
            var maxChildWidth = 0;
            var maxChildHeight = 0;
            items.forEach (function (child) {
                if (child && (child ["visible"] || !fillEmpty)) {
                    if (child ["implicitWidth"] > maxChildWidth) {
                        maxChildWidth = child ["implicitWidth"];
                    }
                    if (child ["implicitHeight"] > maxChildHeight) {
                        maxChildHeight = child ["implicitHeight"];
                    }
                    count++;
                }
            });
            if (cols > 0) {
                if (capacity > 0) {
                    rows = Math.ceil (capacity / cols);
                }
                else if (count > 0) {
                    rows = Math.ceil (count / cols);
                }
                else {
                    rows = 1;
                }
            }
            else {
                rows = 1;
            }
            implicitWidth  = (cols * maxChildWidth)  + ((cols -1) * colSpacing);
            implicitHeight = (rows * maxChildHeight) + ((rows -1) * rowSpacing);
            var itemWidth  = (((width  + colSpacing) / cols) - colSpacing);
            var itemHeight = (((height + rowSpacing) / rows) - rowSpacing);
            var curX = (invertDirection ? width  - itemWidth  : 0);
            var curY = (invertDirection ? height - itemHeight : 0);
            var stepX = (itemWidth  + colSpacing);
            var stepY = (itemHeight + rowSpacing);
            if (rows && cols) {
                var nb = 0;
                items.forEach (function (child) {
                    if (child && (child ["visible"]|| !fillEmpty)) {
                        child ["width"]  = itemWidth;
                        child ["height"] = itemHeight;
                        child ["x"]      = curX;
                        child ["y"]      = curY;
                        if (!verticalFlow) { // horizontal
                            if ((nb +1) % cols > 0) { // next column
                                curX = (invertDirection ? curX - stepX : curX + stepX);
                            }
                            else { // next row
                                curX = (invertDirection ? width - itemWidth : 0);
                                curY = (invertDirection ? curY - stepY : curY + stepY);
                            }
                        }
                        else { // vertical
                            if ((nb +1) % rows > 0) { // next row
                                curY = (invertDirection ? curY - stepY : curY + stepY);
                            }
                            else { // next column
                                curY = (invertDirection ? height - itemHeight : 0);
                                curX = (invertDirection ? curX - stepX : curX + stepX);
                            }
                        }
                        nb++;
                    }
                });
            }
        }
    }

    /* CONTENT HERE */
}
