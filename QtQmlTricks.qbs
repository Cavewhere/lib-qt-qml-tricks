import qbs 1.0;
import qbs.Process;

Project {
    name: "The Qt QML Tricks";
    references: [
        "src/src.qbs",
        "test/test.qbs",
        "import/import.qbs"
    ];

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
            name: "CSS style";
            prefix: "doc/"
            files: ["*.css"];
            fileTags: "style";
        }
        Group {
            name: "C++ & JavaScript & QML inputs";
            prefix: "src/**/";
            files: ["*.h", "*.cpp", "*.qml", "*.js"];
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
