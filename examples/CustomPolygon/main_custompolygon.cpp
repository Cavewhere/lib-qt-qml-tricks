
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>

#include <QtQmlTricks>

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    QQuickView view;
    registerQtQmlTricksModule (view.engine ());
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/ui_custompolygon.qml"));
    view.show ();

    return app.exec ();
}

