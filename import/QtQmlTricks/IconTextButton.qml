import QtQuick 2.1;
import QtQmlTricks 1.0;

MouseArea {
    id: clicker;
    width: implicitWidth;
    height: implicitHeight;
    states: [
        State {
            name: "icon_and_text";
            when: (img.visible && lbl.visible);

            PropertyChanges {
                target: clicker;
                implicitWidth: (img.width + lbl.contentWidth + padding * 3);
                implicitHeight: (img.height > lbl.contentHeight ? img.height + padding * 2: lbl.contentHeight + padding * 2);
            }
            AnchorChanges {
                target: img;
                anchors {
                    left: parent.left;
                    verticalCenter: parent.verticalCenter;
                }
            }
            AnchorChanges {
                target: lbl;
                anchors {
                    left: img.right;
                    right: parent.right;
                    verticalCenter: parent.verticalCenter;
                }
            }
        },
        State {
            name: "text_only";
            when: (!img.visible && lbl.visible);

            PropertyChanges {
                target: clicker;
                implicitWidth: (lbl.contentWidth + padding * 2);
                implicitHeight: (lbl.contentHeight + padding * 2);
            }
            AnchorChanges {
                target: lbl;
                anchors {
                    verticalCenter: parent.verticalCenter;
                    horizontalCenter: parent.horizontalCenter;
                }
            }
        },
        State {
            name: "icon_only";
            when: (img.visible && !lbl.visible);

            PropertyChanges {
                target: clicker;
                implicitWidth: (img.width + padding * 2);
                implicitHeight: (img.height + padding * 2);
            }
            AnchorChanges {
                target: img;
                anchors {
                    verticalCenter: parent.verticalCenter;
                    horizontalCenter: parent.horizontalCenter;
                }
            }
        },
        State {
            name: "empty";
            when: (!img.visible && !lbl.visible);

            PropertyChanges {
                target: clicker;
                implicitWidth: 0;
                implicitHeight: 0;
            }
        }
    ]

    property int padding : 12;

    property alias label      : lbl.text;
    property alias labelFont  : lbl.font;
    property alias labelColor : lbl.color;
    property alias iconSize   : helper.size;
    property alias iconName   : helper.icon;
    property alias iconColor  : helper.color;
    property alias backColor  : rect.color;
    property alias rounding   : rect.radius;

    Rectangle {
        id: shadow;
        color: Qt.darker (backColor, 1.35);
        radius: rounding;
        visible: !pressed;
        antialiasing: true;
        anchors {
            fill: parent;
            topMargin: +2;
            bottomMargin: -2;
        }
    }
    Rectangle {
        id: rect;
        color: (pressed ? "lightblue" : "lightgray");
        radius: 6;
        antialiasing: true;
        border {
            color: "steelblue";
            width: (pressed ? 1 : 0);
        }
        anchors.fill: parent;
    }
    Image {
        id: img;
        width: (helper.size * helper.horizontalRatio);
        height: (helper.size * helper.verticalRatio);
        visible: (status !== Image.Null);
        fillMode: Image.Pad
        anchors.margins: padding;

        SvgIconHelper on source {
            id: helper;
            size: 24;
            color: lbl.color;
        }
    }
    Text {
        id: lbl;
        color: (pressed ? "darkblue" : "black");
        visible: (text !== "");
        horizontalAlignment: (img.visible ? Text.AlignLeft : Text.AlignHCenter);
        font {
            weight: Font.Light;
            pixelSize: 18;
        }
        anchors.margins: padding;
    }
}
