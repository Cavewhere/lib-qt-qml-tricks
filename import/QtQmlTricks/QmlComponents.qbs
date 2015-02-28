import qbs

Product {
    name: "qml-js-imports";

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
}
