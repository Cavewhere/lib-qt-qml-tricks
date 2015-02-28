import qbs;
import qbs.Process;

Project {
    name: "The Qt QML Tricks";
    references: [
        "src/QtLibrary.qbs",
        "import/QtQmlTricks/QmlComponents.qbs",
        "examples/IconCache/IconCache.qbs",
        "examples/NiceModels/NiceModels.qbs",
        "examples/CustomPolygon/CustomPolygon.qbs",
    ];

    Product {
        name: "sdk-utilities";

        Export {
            cpp.defines: ['QML_TRICK_IMPORT="' + project.sourceDirectory + '/import"'];
            cpp.includePaths: "./src";

            Depends { name: "cpp"; }
            Depends {
                name: "Qt";
                submodules: ["core", "gui", "qml", "quick", "svg"];
            }
            Depends { name: "lib-qt-qml-tricks"; }
        }
        Group {
            name: "Includes";
            prefix: "src/";
            files: "Q*";
            excludeFiles: "*.*";
        }
    }
    Product {
        name: "project-utils";
        type: "docs";

        Group {
            name: "Git files";
            files: [
                ".gitignore"
            ];
        }
        Group {
            name: "Doyxgen config";
            prefix: "doc/"
            files: ["Doxyfile"];
            fileTags: "doxyconf";
        }
        Group {
            name: "Schema";
            prefix: "doc/"
            files: ["*.svg"];
        }
        Group {
            name: "Doxygen CSS style";
            prefix: "doc/"
            files: ["*.css"];
            fileTags: "style";
        }
        Group {
            name: "Doxygen C++ inputs";
            prefix: "src/";
            files: ["*.h", "*.cpp"];
            fileTags: "source";
        }
        Group {
            name: "Doxygen JavaScript & QML inputs";
            prefix: "import/**/";
            files: ["*.qml", "*.js"];
            fileTags: "source";
        }
        Group {
            name: "MarkDown documents";
            files: ["*.md"];
            fileTags: "markdown";
        }
        Rule {
            inputs: ["doxyconf", "source", "style", "markdown"];
            multiplex: "true";
            prepare: {
                var cmd = new JavaScriptCommand ();
                cmd.description = "generating documentation from doxygen config";
                cmd.highlight   = "doxygen";
                cmd.sourceCode  = function () {
                    for (var idx = 0; idx < inputs ["doxyconf"].length; idx++) {
                        var file = inputs ["doxyconf"][idx].filePath;
                        var proc    = new Process ();
                        proc.setWorkingDirectory (product.sourceDirectory + "/doc");
                        proc.exec ("doxygen", [file]);
                    }
                }
                return cmd;
            }

            Artifact {
                fileTags: "docs";
                fileName: "force.doc";
                alwaysUpdated: true;
            }
        }
    }
}
