
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>

#include <QtQmlTricks>

#include <QTime>
#include <qmath.h>

static QVector<QPointF> getPolygonForCircleArc (const QPointF & center, qreal radius, int startAngle = 0, int endAngle = 359, bool clockWise = true) {
    static const int degrees = 360;
    static QVector<QPointF> trigoVector;
    if (trigoVector.isEmpty ()) {
        trigoVector.resize (degrees);
        const qreal PI_RAD = (M_PI / 180.0);
        for (int angle = 0; angle < degrees; angle++) {
            const qreal radians = (angle * PI_RAD);
            trigoVector [angle].setX (qCos (radians));
            trigoVector [angle].setY (qSin (radians));
        }
    }
    QVector<QPointF> ret;
    const int firstAngle = ((startAngle + degrees) % degrees);
    const int lastAngle  = ((endAngle   + degrees) % degrees);
    const int stepAngle  = (clockWise ? +1 : -1);
    for (int currAngle = firstAngle;
         currAngle != lastAngle;
         currAngle = ((currAngle + stepAngle + degrees) % degrees)) {
        ret.append (center + (trigoVector [currAngle] * radius));
    }
    return ret;
}

int main (int argc, char * argv []) {
    QGuiApplication app (argc, argv);

    QVector<QPointF> tmp = getPolygonForCircleArc (QPointF (78.9, 45.6), 12.3);

    QVariantList circle;
    circle.reserve (tmp.size ());
    foreach (const QPointF & val, tmp) {
        circle.append (QVariant::fromValue (val));
    }

    QQuickView view;
    registerQtQmlTricksModule (view.engine ());
    view.rootContext ()->setContextProperty ("Circle", QVariant::fromValue (circle));
    view.setResizeMode (QQuickView::SizeRootObjectToView);
    view.setSource (QUrl ("qrc:/ui_custompolygon.qml"));
    view.show ();

    return app.exec ();
}

