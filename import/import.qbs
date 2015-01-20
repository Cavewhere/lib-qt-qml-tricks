import qbs 1.0;

Product {
    name: "qml-js-imports";

    Group {
        name: "JavaScript modules";
        prefix: "QtQmlTricks/";
        files: "*.js";
    }
    Group {
        name: "QML components";
        prefix: "QtQmlTricks/";
        files: "*.qml";
    }
    Group {
        name: "QML directory";
        prefix: "QtQmlTricks/";
        files: "qmldir";
    }
}

