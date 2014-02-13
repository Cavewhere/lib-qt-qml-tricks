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
}
