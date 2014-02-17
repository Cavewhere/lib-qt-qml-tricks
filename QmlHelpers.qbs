import qbs 1.0;

Project {
    name: "The QML Helpers Library";
    references: [
        "src/src.qbs",
        "doc/doc.qbs",
        "test/test.qbs"
    ];

    Product {
        name: "project-utils";

        Group {
            name: "MarkDown documents";
            files: "*.md";
        }
        Group {
            name: "Git files";
            files: [
                ".gitignore"
            ];
        }
    }
}
