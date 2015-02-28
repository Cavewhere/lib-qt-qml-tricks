import qbs 1.0;

StaticLibrary {
    name: "lib-qt-qml-tricks";
    targetName: "QtQmlTricks";

    Depends { name: "cpp"; }
    Depends { name: "Qt"; submodules: ["core", "qml", "gui", "quick", "svg"]; }
    Group {
        name: "C++ sources";
        files: "*.cpp";
        fileTags: "source";
        overrideTags: false;
    }
    Group {
        name: "C++ headers";
        files: "*.h";
        fileTags: "source";
        overrideTags: false;
    }
}
