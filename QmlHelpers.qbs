import qbs 1.0;

Project {
    name: "The QML Helpers Library";

    StaticLibrary {
        name: "lib-qml-helpers";
        targetName: "QmlHelpers";

        Depends { name: "cpp"; }
        Depends { name: "Qt.core"; }
        Group {
            name: "C++ sources";
            files: "Q*.cpp";
        }
        Group {
            name: "C++ headers";
            files: "Q*.h";
        }
    }
    Application {
        name: "test-app";
        targetName: "testQmlHelpers";

        Depends { name: "cpp"; }
        Depends {
            name: "Qt";
            submodules: ["core", "gui", "network", "qml", "quick"]
        }
        Depends { name: "lib-qml-helpers"; }
        Group {
            name: "C++ sources";
            files: "main.cpp";
        }
        Group {
            name: "C++ headers";
            files: "defs.h";
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
}
