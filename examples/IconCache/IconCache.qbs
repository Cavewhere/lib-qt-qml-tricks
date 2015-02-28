import qbs 1.0;

Application {
    name: "example-icon-cache";
    targetName: "IconCache";

    Depends { name: "cpp"; }
    Depends {
        name: "Qt";
        submodules: ["core", "gui", "qml", "quick", "svg"];
    }
    Depends { name: "lib-qt-qml-tricks"; }
    Group {
        name: "C++ sources & headers";
        files: ["*.cpp", "*.h"];
    }
    Group {
        name: "QML documents";
        files: "*.qml";
    }
    Group {
        name: "Qt resources";
        files: "../../*.qrc";
    }
}
