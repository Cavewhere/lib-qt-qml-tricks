import QtQuick 2.1;

FocusScope {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: 200;
    implicitHeight: 200;

    property bool       showBorder    : true;
    property bool       indicatorOnly : false;
    property alias      background    : rect.color;
    property Flickable  flickableItem : null;
    property alias      headerItem    : loaderHeader.sourceComponent;
    property alias      footerItem    : loaderFooter.sourceComponent;

    default property alias content : base.flickableItem;

    Rectangle {
        id: rect;
        color: "white";
        border {
            width: (showBorder ? 1 : 0);
            color: "gray";
        }
        anchors.fill: parent;
    }
    Loader {
        id: loaderHeader;
        visible: item;
        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right;
            margins: rect.border.width;
        }

        Rectangle {
            z: -1;
            gradient: Gradient {
                GradientStop { position: 0; color: "lightgray"; }
                GradientStop { position: 1; color: rect.color; }
            }
            anchors.fill: parent;
        }
    }
    Loader {
        id: loaderFooter;
        visible: item;
        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom;
            margins: rect.border.width;
        }

        Rectangle {
            z: -1;
            gradient: Gradient {
                GradientStop { position: 0; color: rect.color; }
                GradientStop { position: 1; color: "lightgray"; }
            }
            anchors.fill: parent;
        }
    }
    Item {
        id: container;
        clip: true;
        children: flickableItem;
        anchors {
            top: (loaderHeader.item ? loaderHeader.bottom : parent.top);
            left: parent.left;
            right: parent.right;
            bottom: (loaderFooter.item ? loaderFooter.top : parent.bottom);
            topMargin: rect.border.width;
            leftMargin: rect.border.width;
            rightMargin: (scrollbarY.visible ? scrollbarY.width : rect.border.width);
            bottomMargin: (scrollbarX.visible ? scrollbarX.height : rect.border.width);
        }

        Binding {
            target: (flickableItem ? flickableItem.anchors : null);
            property: "fill";
            value: container;
        }
        Binding {
            target: flickableItem;
            property: "boundsBehavior";
            value: Flickable.StopAtBounds;
        }
        Binding {
            target: flickableItem;
            property: "interactive";
            value: true;
        }
    }
    Item {
        id: scrollbarX;
        height: (indicatorOnly ? 6 : 18);
        visible: (flickableItem && flickableItem.flickableDirection !== Flickable.VerticalFlick);
        anchors {
            left: container.left;
            right: container.right;
            bottom: parent.bottom;
            rightMargin: (scrollbarY.visible ? scrollbarY.width : 0);
        }

        Rectangle {
            id: backBottom;
            color: "gray";
            opacity: (flickableItem && flickableItem.contentWidth > container.width ? 0.5 : 0.15);
            anchors.fill: parent;
        }
        MouseArea {
            id: grooveHoriz;
            clip: true;
            enabled: !indicatorOnly;
            drag {
                axis: Drag.XAxis;
                target: handleHoriz;
                minimumX: 0;
                maximumX: (grooveHoriz.width - handleHoriz.width);
            }
            anchors.fill: parent;
            onPositionChanged: {
                flickableItem.contentX = (flickableItem.contentWidth * handleHoriz.x / grooveHoriz.width);
            }

            Rectangle {
                id: handleHoriz;
                color: "lightgray";
                radius: (indicatorOnly ? 2 : 5);
                visible: (flickableItem && flickableItem.visibleArea.widthRatio < 1.0);
                antialiasing: true;
                border {
                    width: (indicatorOnly ? 1 : 2);
                    color: "darkgray";
                }
                anchors {
                    top: parent.top;
                    bottom: parent.bottom;
                }

                Binding on x {
                    when: (flickableItem && !grooveHoriz.pressed);
                    value: (grooveHoriz.width * flickableItem.visibleArea.xPosition);
                }
                Binding on width {
                    when: (flickableItem && !grooveHoriz.pressed);
                    value: Math.max (grooveHoriz.width * flickableItem.visibleArea.widthRatio, 40);
                }
            }
        }
    }
    Item {
        id: scrollbarY;
        width: (indicatorOnly ? 6 : 18);
        visible: (flickableItem && flickableItem.flickableDirection !== Flickable.HorizontalFlick);
        anchors {
            top: container.top;
            right: parent.right;
            bottom: container.bottom;
            bottomMargin: (scrollbarX.visible ? scrollbarX.height : 0);
        }

        Rectangle {
            id: backRight;
            color: "gray";
            opacity: (flickableItem && flickableItem.contentHeight > container.height ? 0.5 : 0.15);
            anchors.fill: parent;
        }
        MouseArea {
            id: grooveVertic;
            clip: true;
            enabled: !indicatorOnly;
            drag {
                axis: Drag.YAxis;
                target: handleVertic;
                minimumY: 0;
                maximumY: (grooveVertic.height - handleVertic.height);
            }
            anchors.fill: parent;
            onPositionChanged: {
                flickableItem.contentY = (flickableItem.contentHeight * handleVertic.y / grooveVertic.height);
            }

            Rectangle {
                id: handleVertic;
                color: "lightgray";
                radius: (indicatorOnly ? 2 : 5);
                visible: (flickableItem && flickableItem.visibleArea.heightRatio < 1.0);
                antialiasing: true;
                border {
                    width: (indicatorOnly ? 1 : 2);
                    color: "darkgray";
                }
                anchors {
                    left: parent.left;
                    right: parent.right;
                }

                Binding on y {
                    when: (flickableItem && !grooveVertic.pressed);
                    value: (grooveVertic.height * flickableItem.visibleArea.yPosition);
                }
                Binding on height {
                    when: (flickableItem && !grooveVertic.pressed);
                    value: Math.max (grooveVertic.height * flickableItem.visibleArea.heightRatio, 40);
                }
            }
        }
    }
    Rectangle {
        color: "gray";
        opacity: Math.max (backRight.opacity, backBottom.opacity);
        anchors {
            top: scrollbarY.bottom;
            left: scrollbarX.right;
            right: container.right;
            bottom: container.bottom;
        }
    }
}
