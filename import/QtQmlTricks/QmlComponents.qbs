import qbs

StaticLibrary {
    name: "qml-js-imports";
    targetName: "QmlJsTricks";

    Depends { name: "Qt"; }
    Depends { name: "cpp"; }
    Group {
        name: "JavaScript modules";
        files: "*.js";
    }
    Group {
        name: "QML components";
        files: "*.qml";
    }
    Group {
        name: "QML directory";
        files: "qmldir";
    }
    Group {
        name: "Qt resources";
        files: "*.qrc";
    }
}
