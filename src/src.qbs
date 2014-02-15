import qbs 1.0;

StaticLibrary {
    name: "lib-qt-tricks";
    targetName: "QmlHelpers";

    Export {
        Depends { name: "cpp"; }

        cpp.includePaths: ["."];
    }
    Depends { name: "Qt.core"; }
    Group {
        name: "C++ sources";
        files: "*.cpp";
    }
    Group {
        name: "C++ headers";
        files: "*.h";
    }
    Group {
        name: "Includes";
        files: "Q*";
        excludeFiles: "*.*";
    }
}
