import QtQuick 2.1;

Rectangle {
    id: window;
    width: 600;
    height: 400;

    Flickable {
        id: flicker;
        contentWidth: width;
        contentHeight: layout.height;
        anchors.fill: parent;

        Column {
            id: layout;
            anchors {
                top: parent.top;
                left: parent.left;
                right: parent.right;
            }

            Repeater {
                model: testModel;
                delegate: Text {
                    text: "<b>%1.</b> %2 (%3) -> %4 = %5 [lifetime %6 secs]".arg (model.foo).arg (model.bar).arg (model.test).arg (model.type).arg (model.qtObject).arg (timer.secs);

                    Timer {
                        id: timer;
                        repeat: true;
                        running: true;
                        interval: 1000;
                        onTriggered: { secs++; }

                        property real secs : 0;
                    }
                }
            }
        }
    }
    Timer {
        repeat: true;
        running: true;
        interval: 2000;
        onTriggered: {
            if (listOperations.length) {
                listOperations.shift () ();
            }
        }

        property var listOperations : [
            function () { testModel.remove (3); },
            function () { testModel.move   (100, 1); },
        ];
    }
}
