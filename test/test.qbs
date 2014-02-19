import qbs 1.0;

Application {
    name: "test-app";
    targetName: "testQtQmlTricks";

    Depends { name: "cpp"; }
    Depends {
        name: "Qt";
        submodules: ["core", "gui", "network", "qml", "quick"];
    }
    Depends { name: "lib-qt-qml-tricks"; }
    Group {
        name: "C++ sources";
        files: "*.cpp";
    }
    Group {
        name: "C++ headers";
        files: "*.h";
    }
    Group {
        name: "QML documents";
        files: "*.qml";
    }
    Group {
        name: "Qt resources";
        files: "*.qrc";
    }
}
