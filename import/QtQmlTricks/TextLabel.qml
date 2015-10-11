import QtQuick 2.1;
import "Style.js" as Style;

Text {
    color: (enabled ? Style.colorBlack : Style.colorGray);
    font {
        weight: Font.Light;
        family: Style.fontName;
        pixelSize: Style.fontSizeNormal;
    }
}
