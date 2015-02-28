
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <qqml.h>

#include <QQmlSvgIconHelper>

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    QQmlSvgIconHelper::setBasePath ("://examples/IconCache");
    qmlRegisterType<QQmlSvgIconHelper> ("QtQmlTricks", 1, 0, "SvgIconHelper");

    QQuickView view;
    view.engine ()->addImportPath ("qrc:/import");
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/examples/IconCache/ui.qml"));
    view.show ();

    return app.exec ();
}

