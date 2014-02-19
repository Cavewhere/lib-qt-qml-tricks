import qbs 1.0;
import qbs.Process;

Product {
    name: "qdoc-doxygen";

    Group {
        name: "Doyxgen config";
        files: "Doxyfile";
        fileTags: "doxyconf";
        overrideTags: false;
    }
    Group {
        name: "CSS style";
        files: "*.css";
        fileTags: "style";
        overrideTags: false;
    }
    Rule {
        inputs: "doxyconf";
        auxiliaryInputs: ["source", "style", "markdown"];
        prepare: {
            var cmd = new JavaScriptCommand ();
            var list = input.fileName.split ("/");
            cmd.description = "generating documentation from " + list [list.length -1];
            cmd.highlight   = 'doxygen';
            cmd.sourceCode  = function () {
                var proc    = new Process ();
                proc.setWorkingDirectory (product.sourceDirectory);
                proc.exec ("doxygen", [ input.fileName ]);
            }
            return cmd;
        }

        Artifact {
            alwaysUpdated: true;
        }
    }
}
