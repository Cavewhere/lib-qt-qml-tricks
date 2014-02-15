import qbs 1.0;

Product {
    name: "qdoc-doxygen";

    Group {
        name: "Doyxgen config";
        files: "Doxyfile";
    }
    Group {
        name: "CSS style";
        files: "*.css";
    }
}
