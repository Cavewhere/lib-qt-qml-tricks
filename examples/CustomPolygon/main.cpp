
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <qqml.h>

#include <QQuickPolygon>

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    qmlRegisterType<QQuickPolygon> ("QtQmlTricks", 1, 0, "Polygon");

    QQuickView view;
    view.engine ()->addImportPath (QML_TRICK_IMPORT);
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/ui.qml"));
    view.show ();

    return app.exec ();
}

