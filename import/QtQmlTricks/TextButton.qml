import QtQuick 2.1;
import "Style.js" as Style;

MouseArea {
    id: clicker;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (lbl.contentWidth + padding * 2);
    implicitHeight: (lbl.contentHeight + padding * 2);

    property int   padding   : 6;
    property bool  checked   : false;
    property alias text      : lbl.text;
    property alias textFont  : lbl.font;
    property alias textColor : lbl.color;
    property alias backColor : rect.color;
    property alias rounding  : rect.radius;

    Gradient {
        id: gradientIdle;

        GradientStop { color: Qt.lighter (Style.colorLightGray, 1.15); position: 0.0; }
        GradientStop { color: Qt.darker  (Style.colorLightGray, 1.15); position: 1.0; }
    }
    Gradient {
        id: gradientPressed;

        GradientStop { color: Qt.darker  (Style.colorDarkGray, 1.15); position: 0.0; }
        GradientStop { color: Qt.lighter (Style.colorDarkGray, 1.15); position: 1.0; }
    }
    Gradient {
        id: gradientChecked;

        GradientStop { color: Qt.darker  (Style.colorLightBlue, 1.15); position: 0.0; }
        GradientStop { color: Qt.lighter (Style.colorLightBlue, 1.15); position: 1.0; }
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
        gradient: (clicker.enabled
                   ? (checked
                      ? gradientChecked
                      : (pressed
                         ? gradientPressed
                         : gradientIdle))
                   : gradientDisabled);
        border {
            width: 1;
            color: (checked ? Style.colorSteelBlue : Style.colorGray);
        }
        anchors.fill: parent;
    }
    Text {
        id: lbl;
        color: (clicker.enabled ? (checked ? Style.colorDarkBlue : Style.colorBlack) : Style.colorGray);
        visible: (text !== "");
        font {
            family: Style.fontName;
            weight: Font.Light;
            pixelSize: Style.fontSizeNormal;
        }
        anchors.centerIn: parent;
    }
}
