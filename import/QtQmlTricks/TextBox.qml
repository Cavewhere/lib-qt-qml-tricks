import QtQuick 2.1;
import "Style.js" as Style;

FocusScope {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (input.contentWidth + padding * 2);
    implicitHeight: (input.contentHeight + padding * 2);

    property int   padding    : 6;
    property alias text       : input.text;
    property alias textFont   : input.font;
    property alias textColor  : input.color;
    property alias textAlign  : input.horizontalAlignment;
    property alias textHolder : holder.text;
    property alias inputMask  : input.inputMask;
    property alias validator  : input.validator;
    property alias rounding   : rect.radius;

    signal accepted ();

    function selectAll () {
        input.selectAll ();
    }
    function clear () {
        input.text = "";
    }

    Gradient {
        id: gradientIdle;

        GradientStop { color: Style.colorWhite; position: 0.0; }
        GradientStop { color: Style.colorWhite; position: 1.0; }
    }
    Gradient {
        id: gradientDisabled;

        GradientStop { color: Style.colorLightGray; position: 0.0; }
        GradientStop { color: Style.colorLightGray; position: 1.0; }
    }
    Rectangle {
        id: rect;
        radius: 3;
        antialiasing: true;
        gradient: (base.enabled ? gradientIdle : gradientDisabled);
        border {
            width: 1;
            color: (input.activeFocus ? Style.colorSteelBlue : Style.colorGray);
        }
        anchors.fill: parent;
    }
    Item {
        clip: true;
        anchors {
            fill: rect;
            margins: rect.border.width;
        }

        TextInput {
            id: input;
            focus: true;
            color: (base.enabled ? Style.colorBlack : Style.colorGray);
            font {
                family: Style.fontName;
                weight: Font.Light;
                pixelSize: Style.fontSizeNormal;
            }
            anchors {
                left: parent.left;
                right: parent.right;
                margins: padding;
                verticalCenter: parent.verticalCenter;
            }
            onAccepted: { base.accepted (); }
        }
    }
    Text {
        id: holder;
        font: input.font;
        color: Style.colorGray;
        visible: (!input.activeFocus && input.text.trim ().length === 0);
        horizontalAlignment: input.horizontalAlignment;
        anchors {
            left: parent.left;
            right: parent.right;
            margins: padding;
            verticalCenter: parent.verticalCenter;
        }
    }
}

