import QtQuick 2.0;

Item {
    id: layout;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (guideL.width + spacing + guideR.width);
    implicitHeight: (guideL.height > guideR.height ? guideL.height : guideR.height);
    onLeftItemsChanged: {
        for (var idx = 0; idx < leftItems.length; idx++) {
            var item = leftItems [idx];
            if (item.parent !== layout) {
                item.parent = layout;
                item.widthChanged.connect   (internal.realignV);
                item.heightChanged.connect  (internal.relayoutH);
                item.visibleChanged.connect (internal.relayoutH);
            }
        }
        internal.relayoutH ();
    }
    onRightItemsChanged: {
        for (var idx = 0; idx < rightItems.length; idx++) {
            var item = rightItems [idx];
            if (item.parent !== layout) {
                item.parent = layout;
                item.widthChanged.connect   (internal.realignV);
                item.heightChanged.connect  (internal.relayoutH);
                item.visibleChanged.connect (internal.relayoutH);
            }
        }
        internal.relayoutH ();
    }
    Component.onCompleted:   { internal.ready = true; }
    Component.onDestruction: { internal.ready = false; }

    property real spacing : 0;

    property bool dontWrap : false;

    property list<Item>     leftItems;
    property list<Item>     rightItems;
    property list<QtObject> otherObjects;

    default property alias nonChildren : layout.otherObjects;

    QtObject {
        id: internal;
        onWrapChanged: { realignV (); }

        property bool ready : false;

        readonly property bool wrap : (!dontWrap && ((guideL.width + layout.spacing + guideR.width) > layout.width));

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
            if (layout && ready) {
                var idx, child;

                var tmpLastL = null;
                var tmpMaxHL = null;
                for (idx = 0; idx < leftItems.length; idx++) {
                    child = leftItems [idx];
                    if (child.visible) {
                        tmpMaxHL = keepBiggest (child, tmpMaxHL, "height");
                        child.anchors.left = undefined;
                        child.anchors.horizontalCenter = undefined;
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

                var tmpMaxHR = null;
                var tmpLastR = null;
                for (idx = rightItems.length -1; idx >= 0; idx--) {
                    child = rightItems [idx];
                    if (child.visible) {
                        tmpMaxHR = keepBiggest (child, tmpMaxHR, "height");
                        child.anchors.left = undefined;
                        child.anchors.horizontalCenter = undefined;
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
                mostBigL = tmpMaxHL;
                mostBigR = tmpMaxHR;
                ready    = true;

                realignV ();
            }
        }

        function realignV () {
            if (layout && ready) {
                var idx, child, tmpBaseline;

                var tmpMaxTxtL = null;
                for (idx = 0; idx < leftItems.length; idx++) {
                    child = leftItems [idx];
                    if (child.visible) {
                        if ("text" in child) {
                            tmpMaxTxtL = keepBiggest (child, tmpMaxTxtL, "contentHeight");
                        }
                    }
                }

                var tmpMaxTxtR = null;
                for (idx = rightItems.length -1; idx >= 0; idx--) {
                    child = rightItems [idx];
                    if (child.visible) {
                        if ("text" in child) {
                            tmpMaxTxtR = keepBiggest (child, tmpMaxTxtR, "contentHeight");
                        }
                    }
                }

                var tmpMaxTxtGlobal = keepBiggest (tmpMaxTxtL, tmpMaxTxtR, "contentHeight");

                for (idx = 0; idx < leftItems.length; idx++) {
                    child = leftItems [idx];
                    if (child.visible) {
                        tmpBaseline = undefined;
                        if ("text" in child) {
                            if (wrap) {
                                if (tmpMaxTxtL && tmpMaxTxtL !== child) {
                                    tmpBaseline = tmpMaxTxtL.baseline;
                                }
                            }
                            else {
                                if (tmpMaxTxtGlobal && tmpMaxTxtGlobal !== child) {
                                    tmpBaseline = tmpMaxTxtGlobal.baseline;
                                }
                            }
                        }
                        if (tmpBaseline) {
                            child.anchors.verticalCenter = undefined;
                            child.anchors.baseline       = tmpBaseline;
                        }
                        else {
                            child.anchors.baseline       = undefined;
                            child.anchors.verticalCenter = guideL.verticalCenter;
                        }
                    }
                }

                for (idx = rightItems.length -1; idx >= 0; idx--) {
                    child = rightItems [idx];
                    if (child.visible) {
                        tmpBaseline = undefined;
                        if ("text" in child) {
                            if (wrap) {
                                if (tmpMaxTxtR && tmpMaxTxtR !== child) {
                                    tmpBaseline = tmpMaxTxtR.baseline;
                                }
                            }
                            else {
                                if (tmpMaxTxtGlobal && tmpMaxTxtGlobal !== child) {
                                    tmpBaseline = tmpMaxTxtGlobal.baseline;
                                }
                            }
                        }
                        if (tmpBaseline) {
                            child.anchors.verticalCenter = undefined;
                            child.anchors.baseline       = tmpBaseline;
                        }
                        else {
                            child.anchors.baseline       = undefined;
                            child.anchors.verticalCenter = guideR.verticalCenter;
                        }
                    }
                }
            }
        }
    }
    Item {
        id: guideL;
        height: (internal.mostBigL ? internal.mostBigL.height : 0);
        anchors {
            top: (internal.wrap ? layout.top : undefined);
            left: layout.left;
            right: (internal.mostFarL ? internal.mostFarL.right : undefined);
            verticalCenter: (!internal.wrap ? layout.verticalCenter : undefined);
        }
    }
    Item {
        id: guideR;
        height: (internal.mostBigR ? internal.mostBigR.height : 0);
        anchors {
            left: (internal.mostFarR ? internal.mostFarR.left : undefined);
            right: layout.right;
            bottom: (internal.wrap ? layout.bottom : undefined);
            verticalCenter: (!internal.wrap ? layout.verticalCenter : undefined);
        }
    }
}
