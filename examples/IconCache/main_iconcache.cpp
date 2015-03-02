
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <qqml.h>

#include <QtQmlTricks>

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    QQmlSvgIconHelper::setBasePath ("://");

    QQuickView view;
    registerQtQmlTricksModule (view.engine ());
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/ui_iconcache.qml"));
    view.show ();

    return app.exec ();
}

