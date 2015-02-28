import QtQuick 2.0;

Item {
    id: layout;
    width: implicitWidth;
    height: implicitHeight;
    onLeftItemsChanged: {
        //console.warn ("-> LEFT ITEMS CHANGED", leftItems.length, layout);
        for (var idx = 0; idx < leftItems.length; idx++) {
            var item = leftItems [idx];
            if (item.parent !== layout) {
                item.parent = layout;
                item.widthChanged.connect   (internal.relayoutH);
                item.heightChanged.connect  (internal.relayoutH);
                item.visibleChanged.connect (internal.relayoutH);
            }
        }
        internal.relayoutH ();
    }
    onRightItemsChanged: {
        //console.warn ("-> RIGHT ITEMS CHANGED", rightItems.length, layout);
        for (var idx = 0; idx < rightItems.length; idx++) {
            var item = rightItems [idx];
            if (item.parent !== layout) {
                item.parent = layout;
                item.widthChanged.connect   (internal.relayoutH);
                item.heightChanged.connect  (internal.relayoutH);
                item.visibleChanged.connect (internal.relayoutH);
            }
        }
        internal.relayoutH ();
    }
    onSpacingChanged:  {
        //console.warn ("-> LAYOUT SPACING CHANGED", width, layout);
        internal.relayoutH ();
    }
    onWidthChanged: {
        //console.warn ("-> LAYOUT WIDTH CHANGED", width, layout);
        internal.realignV ();
    }
    Component.onCompleted: {
        //console.warn ("-> LAYOUT COMPLETED !", layout);
        internal.ready = true;
        internal.relayoutH ();
    }
    Component.onDestruction: {
        //console.warn ("-> LAYOUT DESTRUCTION !", layout);
        internal.ready = false;
    }

    property real spacing : 0;

    property bool dontWrap : false;

    property list<Item>     leftItems;
    property list<Item>     rightItems;
    property list<QtObject> otherObjects;

    default property alias nonChildren : layout.otherObjects;

    QtObject {
        id: internal;

        property bool ready : false;

        function keepBiggest (item1, item2, prop) {
            if (item1 && item2) {
                return (item1 [prop] > item2 [prop] ? item1 : item2);
            }
            else if (item1 && !item2) {
                return item1;
            }
            else if (!item1 && item2) {
                return item2;
            }
            else {
                return null;
            }
        }

        function relayoutH () {
            //console.count ("-------- relayoutH --------");
            if (layout && ready) {
                //console.warn ("-> RELAYOUT ...", layout);
                ready = false;
                //console.time ("RELAYOUT");
                var idx, child;
                //console.warn ("-> LAYOUT LEFT ITEMS ANCHORS");
                var mostFarL = null;
                var mostBigL = null;
                for (idx = 0; idx < leftItems.length; idx++) {
                    child = leftItems [idx];
                    if (child.visible) {
                        mostBigL = keepBiggest (child, mostBigL, "height");
                        child.anchors.left             = undefined;
                        child.anchors.horizontalCenter = undefined;
                        if (mostFarL) {
                            child.anchors.left       = mostFarL.right;
                            child.anchors.leftMargin = Qt.binding (function () { return layout.spacing; });
                        }
                        else {
                            child.anchors.left       = layout.left;
                            child.anchors.leftMargin = 0;
                        }
                        mostFarL = child;
                    }
                }
                //console.warn ("-> LAYOUT RIGHT ITEMS ANCHORS");
                var mostBigR = null;
                var mostFarR = null;
                for (idx = rightItems.length -1; idx >= 0; idx--) {
                    child = rightItems [idx];
                    if (child.visible) {
                        mostBigR = keepBiggest (child, mostBigR, "height");
                        child.anchors.left             = undefined;
                        child.anchors.horizontalCenter = undefined;
                        if (mostFarR) {
                            child.anchors.right       = mostFarR.left;
                            child.anchors.rightMargin = Qt.binding (function () { return layout.spacing; });
                        }
                        else {
                            child.anchors.right       = layout.right;
                            child.anchors.rightMargin = 0;
                        }
                        mostFarR = child;
                    }
                }
                //console.warn("-> FORCE SIZE OF GUIDES AND UPDATE LAYOUT IMPLICIT WIDTH");
                guideR.height        = (mostBigR ? mostBigR.height : 0);
                guideL.height        = (mostBigL ? mostBigL.height : 0);
                guideL.anchors.right = (mostFarL ? mostFarL.right  : undefined);
                guideR.anchors.left  = (mostFarR ? mostFarR.left   : undefined);
                layout.implicitWidth = (guideL.width + spacing + guideR.width);
                //console.timeEnd ("RELAYOUT");
                ready = true;
                realignV ();
                //console.warn ("-> RELAYOUT.", layout);
            }
            else {
                //console.warn ("-> DON'T RELAYOUT !", layout);
            }
        }

        function realignV () {
            //console.count ("-------- realignV --------");
            if (layout && ready) {
                //console.warn ("-> REALIGN...", layout);
                ready = false;
                //console.time ("REALIGN");
                var idx, child, tmpBaseline;
                //console.warn ("-> FIND LEFT TEXT ITEMS");
                var tmpMaxTxtL = null;
                for (idx = 0; idx < leftItems.length; idx++) {
                    child = leftItems [idx];
                    if (child.visible) {
                        if ("text" in child) {
                            tmpMaxTxtL = keepBiggest (child, tmpMaxTxtL, "contentHeight");
                        }
                    }
                }
                //console.warn ("-> FIND RIGHT TEXT ITEMS");
                var tmpMaxTxtR = null;
                for (idx = rightItems.length -1; idx >= 0; idx--) {
                    child = rightItems [idx];
                    if (child.visible) {
                        if ("text" in child) {
                            tmpMaxTxtR = keepBiggest (child, tmpMaxTxtR, "contentHeight");
                        }
                    }
                }
                //console.warn ("-> COMPUTE BIGGEST TEXT ITEM");
                var tmpMaxTxtGlobal = keepBiggest (tmpMaxTxtL, tmpMaxTxtR, "contentHeight");
                //console.warn ("-> DECIDE IF WRAP IS NEEDED");
                var wrap = !dontWrap && (layout.implicitWidth > layout.width);
                //console.warn ("-> REALIGN LEFT ITEMS");
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
                //console.warn ("-> REALIGN RIGHT ITEMS");
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
                //console.warn ("-> FORCE GUIDES ALIGN AND UPDATE LAYOUT IMPLICIT HEIGHT");
                if (wrap) {
                    layout.implicitHeight         = (guideL.height + guideR.height);
                    guideL.anchors.verticalCenter = undefined;
                    guideR.anchors.verticalCenter = undefined;
                    guideL.anchors.bottom         = layout.verticalCenter;
                    guideR.anchors.top            = layout.verticalCenter;

                }
                else {
                    layout.implicitHeight         = (guideL.height > guideR.height ? guideL.height : guideR.height);
                    guideL.anchors.bottom         = undefined;
                    guideR.anchors.top            = undefined;
                    guideL.anchors.verticalCenter = layout.verticalCenter;
                    guideR.anchors.verticalCenter = layout.verticalCenter;
                }
                //console.timeEnd ("REALIGN");
                ready = true;
                //console.warn ("-> REALIGN.", layout);
            }
            else {
                //console.warn ("-> DON'T REALIGN !", layout);
            }
        }
    }
    Item {
        id: guideL;
        anchors.left: layout.left;
        onWidthChanged: {
            //console.warn ("-> GUIDE LEFT WIDTH CHANGED", width, layout);
            internal.realignV ();
        }
        onHeightChanged: {
            //console.warn ("-> GUIDE LEFT HEIGHT CHANGED", height, layout);
            internal.realignV ();
        }

        /*Rectangle {
            color: "lime";
            opacity: 0.5;
            anchors.fill: parent;
        }*/
    }
    Item {
        id: guideR;
        anchors.right: layout.right;
        onWidthChanged: {
            //console.warn ("-> GUIDE RIGHT WIDTH CHANGED", width, layout);
            internal.realignV ();
        }
        onHeightChanged: {
            //console.warn ("-> GUIDE RIGHT HEIGHT CHANGED", height, layout);
            internal.realignV ();
        }

        /*Rectangle {
            color: "magenta";
            opacity: 0.5;
            anchors.fill: parent;
        }*/
    }
}
