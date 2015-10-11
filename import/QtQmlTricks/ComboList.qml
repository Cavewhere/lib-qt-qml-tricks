import QtQuick 2.1;
import "Style.js" as Style;

Item {
    id: base;
    width: implicitWidth;
    height: implicitHeight;
    implicitWidth: (lbl.contentWidth + padding * 2);
    implicitHeight: (lbl.contentHeight + padding * 2);

    property int   padding   : 6;
    property alias textColor : lbl.color;
    property alias backColor : rect.color;
    property alias rounding  : rect.radius;

    property ListModel model : null;

    property int          currentIdx   : -1;
    readonly property var currentValue : (model && currentIdx >= 0 && currentIdx < model.count ? model.get (currentIdx) ["value"] : undefined);
    readonly property var currentKey   : (model && currentIdx >= 0 && currentIdx < model.count ? model.get (currentIdx) ["key"]   : undefined);

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
        id: gradientDisabled;

        GradientStop { color: Style.colorLightGray; position: 0.0; }
        GradientStop { color: Style.colorLightGray; position: 1.0; }
    }
    MouseArea {
        id: clicker;
        anchors.fill: parent;
        onClicked: {
            if (dropdownItem) {
                destroyDropdown ();
            }
            else {
                createDropdown ();
            }
        }
        Component.onDestruction: { destroyDropdown (); }

        property Item dropdownItem : null;

        readonly property Item rootItem : {
            var tmp = base;
            while ("parent" in tmp && tmp ["parent"] !== null && "visible" in tmp ["parent"]) {
                tmp = tmp ["parent"];
            }
            return tmp;
        }

        function createDropdown () {
            dropdownItem = compoDropdown.createObject (rootItem, { "referenceItem" : base });
        }

        function destroyDropdown () {
            if (dropdownItem) {
                dropdownItem.destroy ();
                dropdownItem = null;
            }
        }
    }
    Rectangle {
        id: rect;
        radius: 3;
        antialiasing: true;
        gradient: (enabled
                   ? (clicker.pressed ||
                      clicker.dropdownItem
                      ? gradientPressed
                      : gradientIdle)
                   : gradientDisabled);
        border {
            width: 1;
            color: Style.colorGray;
        }
        anchors.fill: parent;
    }
    TextLabel {
        id: lbl;
        text: (currentValue || "");
        color: (enabled ? Style.colorBlack : Style.colorGray);
        elide: Text.ElideRight;
        visible: (text !== "");
        anchors {
            left: parent.left;
            right: arrow.left;
            margins: padding;
            verticalCenter: parent.verticalCenter;
        }
    }
    Item {
        id: arrow;
        clip: true;
        width: 12;
        height: (width / 2);
        anchors {
            right: parent.right;
            margins: padding;
            verticalCenter: parent.verticalCenter;
        }

        Rectangle {
            color: Style.colorBlack;
            width: (parent.height * Math.SQRT2);
            height: width;
            rotation: 45;
            antialiasing: true;
            anchors {
                verticalCenter: parent.top;
                horizontalCenter: parent.horizontalCenter;
            }
        }
    }
    Component {
        id: compoDropdown;

        MouseArea {
            id: dimmer;
            anchors.fill: parent;
            onWheel: { }
            onPressed: { clicker.destroyDropdown (); }

            property Item referenceItem : null;

            Rectangle {
                color: "lightgray";
                height: Math.max (layout.height, 24);
                border {
                    width: 1;
                    color: Style.colorGray;
                }
                Component.onCompleted: {
                    if (dimmer.referenceItem) {
                        var pos = mapFromItem (dimmer.referenceItem.parent,
                                               dimmer.referenceItem.x,
                                               dimmer.referenceItem.y +
                                               dimmer.referenceItem.height);
                        width = dimmer.referenceItem.width;
                        x = pos ["x"];
                        y = pos ["y"];
                    }
                }

                Column {
                    id: layout;
                    anchors {
                        top: parent.top;
                        left: parent.left;
                        right: parent.right;
                    }

                    Repeater {
                        model: base.model;
                        delegate: MouseArea {
                            height: 24;
                            anchors {
                                left: layout.left;
                                right: layout.right;
                            }
                            onClicked: {
                                currentIdx = model.index;
                                clicker.destroyDropdown ();
                            }

                            TextLabel {
                                clip: true;
                                text: (model ["value"] || "");
                                font.bold: (model.index === currentIdx);
                                anchors {
                                    left: parent.left;
                                    right: parent.right;
                                    margins: padding;
                                    verticalCenter: parent.verticalCenter;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
