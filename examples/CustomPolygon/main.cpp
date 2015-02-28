
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
    view.engine ()->addImportPath ("qrc:/import");
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/examples/CustomPolygon/ui.qml"));
    view.show ();

    return app.exec ();
}

