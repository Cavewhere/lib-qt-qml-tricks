import QtQuick 2.1;

FocusScope {
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (input.contentWidth + padding * 2);
    implicitHeight: (input.contentHeight + padding * 2);

    property int padding : 6;

    property color backgroundColor : "white";
    property color foregroundColor : "black";
    property color highlightColor  : "steelblue";
    property color secondaryColor  : "lightgray";
    property color borderColor     : "gray";

    property alias textFont        : input.font;
    property alias rounding        : rect.radius;
    property alias editableText    : input.text;
    property alias isEditLocked    : input.readOnly;
    property alias placeholderText : placeholder.text;

    Rectangle {
        id: rect;
        color: backgroundColor;
        radius: 4;
        antialiasing: true;
        anchors.fill: parent;
    }
    Item {
        clip: true;
        anchors.fill: parent;

        TextInput {
            id: input;
            focus: true;
            color: (enabled ? foregroundColor : secondaryColor);
            selectByMouse: true;
            selectionColor: highlightColor;
            selectedTextColor: backgroundColor;
            activeFocusOnPress: true;
            font {
                weight: Font.Light;
                pixelSize: 18;
            }
            anchors {
                left: parent.left;
                right: parent.right;
                margins: padding;
                verticalCenter: parent.verticalCenter;
            }
        }
        Text {
            id: placeholder;
            font: input.font;
            color: secondaryColor;
            visible: (!input.activeFocus && !input.length);
            anchors.fill: input;
        }
    }
    Rectangle {
        color: "transparent";
        radius: rect.radius;
        antialiasing: true;
        border {
            width: (input.activeFocus ? 2 : 1);
            color: (input.activeFocus ? highlightColor : borderColor);
        }
        anchors.fill: parent;
    }
}
