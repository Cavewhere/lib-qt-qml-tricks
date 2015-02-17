#ifndef QQUICKPOLYGON_H
#define QQUICKPOLYGON_H

#include <QQuickItem>
#include <QVariant>
#include <QPointF>
#include <QColor>
#include <QList>

class QQuickPolygon : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY (QColor       color  READ getColor  WRITE setColor  NOTIFY colorChanged)
    Q_PROPERTY (QVariantList points READ getPoints WRITE setPoints NOTIFY pointsChanged)

public:
    explicit QQuickPolygon (QQuickItem * parent = NULL);
    virtual ~QQuickPolygon (void);

    Q_INVOKABLE QColor       getColor  (void) const;
    Q_INVOKABLE QVariantList getPoints (void) const;

public slots:
    void setColor  (QColor       color);
    void setPoints (QVariantList points);

signals:
    void colorChanged  (void);
    void pointsChanged (void);

protected:
    virtual QSGNode * updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData);

    bool snip (int u, int v, int w, int n, int * V);

    bool isInsideTriangle (qreal Ax, qreal Ay, qreal Bx, qreal By, qreal Cx, qreal Cy, qreal Px, qreal Py);

    QVector<QPointF> processTriangulation (void);

private:
    qreal m_minX;
    qreal m_maxX;
    qreal m_minY;
    qreal m_maxY;
    QColor m_color;
    QVector<QPointF> m_points;
};

#endif // QQUICKPOLYGON_H
