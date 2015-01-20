import QtQuick 2.1;
import QtQmlTricks 1.0;

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
                    text: "<b>%1.</b> %2 (%3) -> %4 = %5".arg (model.foo).arg (model.bar).arg (model.test).arg (model.type).arg (model.qtObject);
                }
            }
        }
    }
    Timer {
        repeat: true;
        running: true;
        interval: 1000;
        onTriggered: {
            if (listOperations.length) {
                listOperations.shift () ();
            }
        }

        property var listOperations : [
            function () {
                console.log ("model count=", testModel.count);
            },
            function () {
                var uid = "2014-04-08";
                var obj = testModel.getByUid (uid);
                console.log ("uid=", uid, "obj=", obj, "val=", obj.bar);
            },
            function () {
                console.log ("model remove idx 3=", (testModel.remove (3) || "DONE"));
            },
            function () {
                console.log ("model move idx 100 to position 1=", (testModel.move (100, 1) || "DONE"));
            },
            function () {
                console.log ("'foobar' at 2 :", "foobar".at (2));
            },
            function () {
                var arr = ["fo", "ob", "ar"];
                console.log ("arr before=", JSON.stringify (arr));
                console.log ("arr first=", arr.first ());
                console.log ("arr last=", arr.last ());
                arr.removeAt (1);
                console.log ("arr after remove at 1 =", JSON.stringify (arr));
            },
            function () {
                console.log ("remove all 'o' in 'foobar'=", "foobar".remove ('o'));
            },
        ];
    }
}
