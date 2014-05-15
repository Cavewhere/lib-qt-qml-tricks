import qbs 1.0;

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
}

