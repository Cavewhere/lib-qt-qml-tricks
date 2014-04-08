import QtQuick 2.1;

Rectangle {
    id: window;
    width: 600;
    height: 400;
    Component.onCompleted: {
        console.log ("count=", testModel.count);
        var obj = testModel.getByUid ("2014-04-08");
        console.log ("obj=", obj, obj.bar);
    }

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
                    text: "<b>%1.</b> %2 (%3) -> %4 = %5".arg (model.foo).arg (model.bar).arg (model.test).arg (model.type).arg (model.qtObject);
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
