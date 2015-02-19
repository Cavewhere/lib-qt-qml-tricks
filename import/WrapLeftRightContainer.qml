import QtQuick 2.0;

Item {
    id: layout;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (guideL.width + spacing + guideR.width);
    implicitHeight: (internal.wrap
                     ? (guideL.height + spacing + guideR.height)
                     : Math.max (guideL.height, guideR.height));
    onWidthChanged: { internal.realignV (); }
    onImplicitWidthChanged: { internal.realignV (); }
    onLeftItemsChanged: {
        for (var idx = 0; idx < leftItems.length; idx++) {
            var item = leftItems [idx];
            item.parent = layout;
            item.heightChanged.connect  (internal.relayoutH);
            item.visibleChanged.connect (internal.relayoutH);
        }
        internal.relayoutH ();
    }
    onRightItemsChanged: {
        for (var idx = 0; idx < rightItems.length; idx++) {
            var item = rightItems [idx];
            item.parent = layout;
            item.heightChanged.connect  (internal.relayoutH);
            item.visibleChanged.connect (internal.relayoutH);
        }
        internal.relayoutH ();
    }

    property real spacing : 0;

    property list<Item>     leftItems;
    property list<Item>     rightItems;
    property list<QtObject> otherObjects;

    default property alias nonChildren : layout.otherObjects;

    QtObject {
        id: internal;

        property bool ready    : false;
        property bool wrap     : false;

        property Item mostBigL : null;
        property Item mostFarL : null;

        property Item mostBigR : null;
        property Item mostFarR : null;

        function keepBiggest (item1, item2, prop) {
            if ((item1 && !item2) || (item1 && item2 && item1 [prop] > item2 [prop])) {
                return item1;
            }
            else if ((!item1 && item2) || (item1 && item2 && item1 [prop] < item2 [prop])) {
                return item2;
            }
            else {
                return null;
            }
        }

        function relayoutH () {
            var idx, child;

            var tmpLastL = null;
            for (idx = 0; idx < leftItems.length; idx++) {
                child = leftItems [idx];
                if (child.visible) {
                    if (tmpLastL) {
                        child.anchors.left       = tmpLastL.right;
                        child.anchors.leftMargin = Qt.binding (function () { return layout.spacing; });
                    }
                    else {
                        child.anchors.left       = layout.left;
                        child.anchors.leftMargin = 0;
                    }
                    tmpLastL = child;
                }
            }

            var tmpLastR = null;
            for (idx = rightItems.length -1; idx >= 0; idx--) {
                child = rightItems [idx];
                if (child.visible) {
                    if (tmpLastR) {
                        child.anchors.right       = tmpLastR.left;
                        child.anchors.rightMargin = Qt.binding (function () { return layout.spacing; });
                    }
                    else {
                        child.anchors.right       = layout.right;
                        child.anchors.rightMargin = 0;
                    }
                    tmpLastR = child;
                }
            }

            ready    = false;
            mostFarL = tmpLastL;
            mostFarR = tmpLastR;
            ready    = true;
        }

        function realignV () {
            var idx, center, child;

            var tmpMaxHL = null;
            var tmpMaxTxtL = null;
            for (idx = 0; idx < leftItems.length; idx++) {
                child = leftItems [idx];
                if (child.visible) {
                    tmpMaxHL = keepBiggest (child, tmpMaxHL, "height");
                    if ("text" in child) {
                        tmpMaxTxtL = keepBiggest (child, tmpMaxTxtL, "contentHeight");
                    }
                }
            }

            var tmpMaxHR = null;
            var tmpMaxTxtR = null;
            for (idx = rightItems.length -1; idx >= 0; idx--) {
                child = rightItems [idx];
                if (child.visible) {
                    tmpMaxHR = keepBiggest (child, tmpMaxHR, "height");
                    if ("text" in child) {
                        tmpMaxTxtR = keepBiggest (child, tmpMaxTxtR, "contentHeight");
                    }
                }
            }

            var tmpMaxTxtGlobal = keepBiggest (tmpMaxTxtL, tmpMaxTxtR, "contentHeight");

            var tmpWrap = (implicitWidth > width);

            for (idx = 0; idx < leftItems.length; idx++) {
                child = leftItems [idx];
                if (child.visible) {
                    center = true;
                    child.anchors.baseline = undefined;
                    child.anchors.verticalCenter = undefined;
                    if ("text" in child) {
                        if (tmpWrap) {
                            if (tmpMaxTxtL && tmpMaxTxtL !== child) {
                                child.anchors.baseline = tmpMaxTxtL.baseline;
                                center = false;
                            }
                        }
                        else {
                            if (tmpMaxTxtGlobal && tmpMaxTxtGlobal !== child) {
                                child.anchors.baseline = tmpMaxTxtGlobal.baseline;
                                center = false;
                            }
                        }
                    }
                    if (center) {
                        child.anchors.verticalCenter = guideL.verticalCenter;
                    }
                }
            }

            for (idx = rightItems.length -1; idx >= 0; idx--) {
                child = rightItems [idx];
                if (child.visible) {
                    center = true;
                    child.anchors.baseline = undefined;
                    child.anchors.verticalCenter = undefined;
                    if ("text" in child) {
                        if (tmpWrap) {
                            if (tmpMaxTxtR && tmpMaxTxtR !== child) {
                                child.anchors.baseline = tmpMaxTxtR.baseline;
                                center = false;
                            }
                        }
                        else {
                            if (tmpMaxTxtGlobal && tmpMaxTxtGlobal !== child) {
                                child.anchors.baseline = tmpMaxTxtGlobal.baseline;
                                center = false;
                            }
                        }
                    }
                    if (center) {
                        child.anchors.verticalCenter = guideR.verticalCenter;
                    }
                }
            }

            ready    = false;
            wrap     = tmpWrap;
            mostBigL = tmpMaxHL;
            mostBigR = tmpMaxHR;
            ready    = true;
        }
    }
    Item {
        id: guideL;
        height: (internal.ready && internal.mostBigL ? internal.mostBigL.height : 0);
        anchors {
            top: (internal.ready && internal.wrap ? layout.top : undefined);
            left: layout.left;
            right: (internal.ready && internal.mostFarL ? internal.mostFarL.right : undefined);
            bottom: undefined;
            verticalCenter: (internal.ready && !internal.wrap ? layout.verticalCenter : undefined);
        }
    }
    Item {
        id: guideR;
        height: (internal.ready && internal.mostBigR ? internal.mostBigR.height : 0);
        anchors {
            top: undefined;
            left: (internal.ready && internal.mostFarR ? internal.mostFarR.left : undefined);
            right: layout.right;
            bottom: (internal.ready && internal.wrap ? layout.bottom : undefined);
            verticalCenter: (internal.ready && !internal.wrap ? layout.verticalCenter : undefined);
        }
    }
