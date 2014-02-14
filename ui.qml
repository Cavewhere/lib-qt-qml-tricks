import QtQuick 2.1;

Rectangle {
    width: 600;
    height: 400;

    ListView {
        model: testModel;
        delegate: Text {
            text: "<b>%1.</b> %2 (%3) -> %4 = %5".arg (model.foo).arg (model.bar).arg (model.test).arg (model.type).arg (model.qtObject);
        }
        anchors.fill: parent;
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
