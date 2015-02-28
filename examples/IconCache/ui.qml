import QtQuick     2.1;
import QtQmlTricks 1.0;

Rectangle {
    id: window;
    width: 600;
    height: 400;

    Row {
        spacing: 20;
        anchors.centerIn: parent;

        Repeater {
            model: [
                { "size" :  20, "color" : "gray"        },
                { "size" :  32, "color" : "orange"      },
                { "size" :  48, "color" : "steelblue"   },
                { "size" :  64, "color" : "transparent" },
            ];
            delegate: Image {
                cache: true;
                smooth: false;
                fillMode: Image.Pad;
                asynchronous: true;
                antialiasing: false;

                SvgIconHelper on source {
                    icon: "mark";
                    size: modelData ["size"];
                    color: Qt.lighter (modelData ["color"], 1.0);
                }
            }
        }
    }
}
