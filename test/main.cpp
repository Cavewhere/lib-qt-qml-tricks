
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <QDateTime>

#include <QQmlObjectListModel>

#include "defs.h"

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    QQmlObjectListModel * testModel = QQmlObjectListModel::create<MyItem> (&app);

    int year = QDateTime::currentDateTime ().date ().year ();
    QDate date (year, 1, 1);
    while (date.year () == year) {
        MyItem * item = new MyItem;
        item->set_foo (date.dayOfYear ());
        item->set_bar (date.toString (Qt::DefaultLocaleLongDate));
        item->update_test (MyEnum::Type (date.dayOfWeek ()));
        testModel->append (item);
        date = date.addDays (1);
    }

    QQuickView view;
    view.rootContext ()->setContextProperty ("testModel", testModel);
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/ui.qml"));
    view.show ();

    return app.exec ();
}

