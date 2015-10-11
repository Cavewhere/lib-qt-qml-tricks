.pragma library;

var fontSizeSmall  = 11;
var fontSizeNormal = 14;
var fontSizeBig    = 16;
var fontSizeTitle  = 18;

var colorGray      = "gray";
var colorBlack     = "black";
var colorWhite     = "white";
var colorDarkGray  = "darkgray";
var colorLightGray = "lightgray";
var colorDarkRed   = "darkred";
var colorOrange    = "orange";
var colorSteelBlue = "steelblue";
var colorDarkBlue  = "darkblue";
var colorLightBlue = "lightblue";

var fontName = selectFont ([
                               "Sail Sans Pro",
                               "Source Sans Pro",
                               "Ubuntu",
                               "Roboto",
                               "Droid Sans",
                               "Liberation Sans",
                               "Trebuchet MS",
                               "Deja Vu Sans",
                               "Tahoma",
                               "Arial",
                           ],
                           "sans-serif");

var fontFixedName = selectFont ([
                                    "Ubuntu Mono",
                                    "Deja Vu Mono",
                                    "Courier New",
                                    "Lucida Console",
                                ],
                                "monospace");

function gray (val) {
    var tmp = (val / 255);
    return Qt.rgba (tmp, tmp, tmp, 1.0);
}
function opacify (tint, alpha) {
    var tmp = Qt.darker (tint, 1.0);
    return Qt.rgba (tmp.r, tmp.g, tmp.b, alpha);
}

function selectFont (list, fallback) {
    var ret;
    var all = Qt.fontFamilies ();
    for (var idx = 0; idx < list.length; idx++) {
        var tmp = list [idx];
        if (all.indexOf (tmp) >= 0) {
            ret = tmp;
            break;
        }
    }
    return (ret || fallback);
}
