import QtQuick     2.1;
import QtQmlTricks 1.0;

Rectangle {
    id: window;
    width: 600;
    height: 400;

    readonly property real centerX : (width  / 2);
    readonly property real centerY : (height / 2);

    readonly property real size : (Math.min (centerX, centerY) * 0.85);

    Timer {
        id: timer;
        repeat: true;
        running: true;
        interval: 1000;
        triggeredOnStart: true;
        onTriggered: { polygon.count++; }
    }
    FocusScope {
        focus: true;
        Keys.onPressed: { timer.running = !timer.running; }
    }
    Polygon {
        id: polygon;
        color: Qt.hsla (0.5, 0.5, 0.5, 0.5);
        points:{
            var ret = [];
            for (var idx = 0; idx < count; idx++) {
                var rad = (Math.PI * 2) * (idx / count);
                ret.push (Qt.point (centerX + size * Math.cos (rad),
                                    centerY + size * Math.sin (rad)));
            }
            return ret;
        }

        property int count : 0;

        Repeater {
            model: parent.points;
            delegate: Text {
                text: ("P" + model.index);
                x: (modelData ["x"] - contentWidth  / 2);
                y: (modelData ["y"] - contentHeight / 2);
                color: "white";
                style: Text.Outline;
                styleColor: "black";
                font {
                    bold: true;
                    pixelSize: 20;
                }
            }
        }
    }
}
