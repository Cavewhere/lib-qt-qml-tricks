
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <QDateTime>
#include <qqml.h>

#include <QQmlObjectListModel>
#include <QQmlSvgIconHelper>
#include <QQuickPolygon>

#include "defs.h"

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    QQmlObjectListModel * testModel = QQmlObjectListModel::create<MyItem> (&app);
    testModel->setRoleNameForUid ("bar");

    int year = QDateTime::currentDateTime ().date ().year ();
    QDate date (year, 1, 1);
    while (date.year () == year) {
        MyItem * item = new MyItem;
        item->set_foo (date.dayOfYear ());
        item->set_bar (date.toString ("yyyy-MM-dd"));
        item->update_test (MyEnum::Type (date.dayOfWeek ()));
        testModel->append (item);
        date = date.addDays (1);
    }

    QQmlSvgIconHelper::setBasePath (":/test");
    qmlRegisterType<QQmlSvgIconHelper> ("QtQmlTricks", 1, 0, "SvgIconHelper");
    qmlRegisterType<QQuickPolygon>     ("QtQmlTricks", 1, 0, "Polygon");

    QQuickView view;
    view.engine ()->addImportPath ("qrc:/import");
    view.rootContext ()->setContextProperty ("testModel", testModel);
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/test/ui.qml"));
    view.show ();

    return app.exec ();
}

