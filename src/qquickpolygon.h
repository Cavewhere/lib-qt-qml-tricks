#ifndef QMLPOLYGON_H
#define QMLPOLYGON_H

#include <QQuickItem>
#include <QVariant>
#include <QColor>
#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QSGNode>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

class QQuickPolygon : public QQuickItem {
    Q_OBJECT
    Q_PROPERTY (bool         closed READ getClosed WRITE setClosed NOTIFY closedChanged) // whether last point should connect to first
    Q_PROPERTY (qreal        border READ getBorder WRITE setBorder NOTIFY borderChanged) // border width
    Q_PROPERTY (QColor       color  READ getColor  WRITE setColor  NOTIFY colorChanged)  // back color
    Q_PROPERTY (QColor       stroke READ getStroke WRITE setStroke NOTIFY strokeChanged) // border color
    Q_PROPERTY (QVariantList points READ getPoints WRITE setPoints NOTIFY pointsChanged) // points list

public:
    explicit QQuickPolygon (QQuickItem * parent = NULL);

    Q_INVOKABLE bool         getClosed (void) const;
    Q_INVOKABLE qreal        getBorder (void) const;
    Q_INVOKABLE QColor       getColor  (void) const;
    Q_INVOKABLE QColor       getStroke (void) const;
    Q_INVOKABLE QVariantList getPoints (void) const;

public slots:
    void setClosed (bool closed);
    void setBorder (qreal border);
    void setColor  (const QColor & color);
    void setStroke (const QColor & stroke);
    void setPoints (const QVariantList & points);

signals:
    void colorChanged  (void);
    void pointsChanged (void);
    void borderChanged (void);
    void closedChanged (void);
    void strokeChanged (void);

protected:
    virtual QSGNode * updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData);

protected slots:
    void processTriangulation (void);

private:
    bool m_closed;
    qreal m_border;
    qreal m_minX;
    qreal m_maxX;
    qreal m_minY;
    qreal m_maxY;
    QColor m_color;
    QColor m_stroke;
    QPolygonF m_points;
    QVector<QPointF> m_triangles;
    QSGNode *              m_node;
    QSGGeometryNode *      m_foreNode;
    QSGGeometryNode *      m_backNode;
    QSGGeometry *          m_foreGeometry;
    QSGGeometry *          m_backGeometry;
    QSGFlatColorMaterial * m_foreMaterial;
    QSGFlatColorMaterial * m_backMaterial;
};

#endif // QMLPOLYGON_H
