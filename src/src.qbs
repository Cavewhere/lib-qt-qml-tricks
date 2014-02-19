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
        fileTags: "source";
        overrideTags: false;
    }
    Group {
        name: "C++ headers";
        files: "*.h";
        fileTags: "source";
        overrideTags: false;
    }
    Group {
        name: "Includes";
        files: "Q*";
        excludeFiles: "*.*";
    }
}
