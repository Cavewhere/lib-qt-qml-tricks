import QtQuick 2.1;

FocusScope {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: 200;
    implicitHeight: 200;

    property bool       showBorder    : true;
    property alias      background    : rect.color;
    property Flickable  flickableItem : null;

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
    Item {
        id: container;
        clip: true;
        children: flickableItem;
        anchors {
            fill: parent;
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
        height: 18;
        visible: (flickableItem && flickableItem.flickableDirection !== Flickable.VerticalFlick);
        anchors {
            left: parent.left;
            right: parent.right;
            bottom: parent.bottom;
            rightMargin: (scrollbarY.visible ? scrollbarY.width : 0);
        }

        Rectangle {
            color: "darkgray";
            anchors.fill: parent;
        }
        MouseArea {
            id: grooveHoriz;
            clip: true;
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
                radius: 5;
                visible: (flickableItem && flickableItem.visibleArea.widthRatio < 1.0);
                antialiasing: true;
                border {
                    width: 2;
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
                    value: (grooveHoriz.width * flickableItem.visibleArea.widthRatio);
                }
            }
        }
    }
    Item {
        id: scrollbarY;
        width: 18;
        visible: (flickableItem && flickableItem.flickableDirection !== Flickable.HorizontalFlick);
        anchors {
            top: parent.top;
            right: parent.right;
            bottom: parent.bottom;
            bottomMargin: (scrollbarX.visible ? scrollbarX.height : 0);
        }

        Rectangle {
            color: "darkgray";
            anchors.fill: parent;
        }
        MouseArea {
            id: grooveVertic;
            clip: true;
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
                radius: 5;
                visible: (flickableItem && flickableItem.visibleArea.heightRatio < 1.0);
                antialiasing: true;
                border {
                    width: 2;
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
                    value: (grooveVertic.height * flickableItem.visibleArea.heightRatio);
                }
            }
        }
    }
    Rectangle {
        color: "lightgray";
        anchors {
            top: scrollbarY.bottom;
            left: scrollbarX.right;
            right: parent.right;
            bottom: parent.bottom;
        }
    }
}
