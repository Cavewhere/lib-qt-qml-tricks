#include "qquickpolygon.h"

QQuickPolygon::QQuickPolygon (QQuickItem * parent)
    : QQuickItem (parent)
    , m_border (0)
    , m_closed (true)
    , m_minX   (0.0)
    , m_maxX   (0.0)
    , m_minY   (0.0)
    , m_maxY   (0.0)
    , m_color  (Qt::magenta)
    , m_stroke (Qt::transparent)
    , m_node         (Q_NULLPTR)
    , m_foreNode     (Q_NULLPTR)
    , m_backNode     (Q_NULLPTR)
    , m_foreGeometry (Q_NULLPTR)
    , m_backGeometry (Q_NULLPTR)
    , m_foreMaterial (Q_NULLPTR)
    , m_backMaterial (Q_NULLPTR)
{
    setFlag (QQuickItem::ItemHasContents);
}

int QQuickPolygon::getBorder (void) const {
    return m_border;
}

bool QQuickPolygon::getClosed (void) const {
    return m_closed;
}

QColor QQuickPolygon::getColor (void) const {
    return m_color;
}

QColor QQuickPolygon::getStroke (void) const {
    return m_stroke;
}

QVariantList QQuickPolygon::getPoints (void) const {
    QVariantList ret;
    foreach (QPointF point, m_points) {
        ret.append (point);
    }
    return ret;
}

void QQuickPolygon::setBorder (int border) {
    if (m_border != border) {
        m_border = border;
        emit borderChanged ();
        update ();
    }
}

void QQuickPolygon::setClosed (bool closed) {
    if (m_closed != closed) {
        m_closed = closed;
        emit closedChanged ();
        update ();
    }
}

void QQuickPolygon::setColor (const QColor & color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged ();
        update ();
    }
}

void QQuickPolygon::setStroke (const QColor & stroke) {
    if (m_stroke != stroke) {
        m_stroke = stroke;
        emit strokeChanged ();
        update ();
    }
}
void QQuickPolygon::setPoints (const QVariantList & points) {
    m_points.clear ();
    foreach (QVariant tmp, points) {
        m_points.append (tmp.toPointF ());
    }
    emit pointsChanged ();
    processTriangulation ();
    update ();
}

QSGNode * QQuickPolygon::updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData) {
    Q_UNUSED (oldNode)
    Q_UNUSED (updatePaintNodeData)
    // remove old nodes
    if (m_backMaterial != Q_NULLPTR) {
        delete m_backMaterial;
    }
    if (m_foreMaterial != Q_NULLPTR) {
        delete m_foreMaterial;
    }
    if (m_backGeometry != Q_NULLPTR) {
        delete m_backGeometry;
    }
    if (m_foreGeometry != Q_NULLPTR) {
        delete m_foreGeometry;
    }
    if (m_backNode != Q_NULLPTR) {
        delete m_backNode;
    }
    if (m_foreNode != Q_NULLPTR) {
        delete m_foreNode;
    }
    if (m_node != Q_NULLPTR) {
        delete m_node;
    }
    m_node = new QSGNode;
    // polygon background tesselation
    if (!m_triangles.isEmpty () && m_color.alpha () > 0) {
        m_backGeometry = new QSGGeometry (QSGGeometry::defaultAttributes_Point2D (), m_triangles.size ());
        m_backGeometry->setDrawingMode (GL_TRIANGLES);
        QSGGeometry::Point2D * vertex = m_backGeometry->vertexDataAsPoint2D ();
        const int size = m_triangles.size ();
        for (int idx = 0; idx < size; idx++) {
            vertex [idx].x = m_triangles [idx].x ();
            vertex [idx].y = m_triangles [idx].y ();
        }
        m_backMaterial = new QSGFlatColorMaterial;
        m_backMaterial->setColor (m_color);
        m_backNode = new QSGGeometryNode;
        m_backNode->setGeometry (m_backGeometry);
        m_backNode->setMaterial (m_backMaterial);
        m_node->appendChildNode (m_backNode);
    }
    // polyline stroke
    if (!m_points.isEmpty () && m_border > 0 && m_stroke.alpha () > 0) {
        m_foreGeometry = new QSGGeometry (QSGGeometry::defaultAttributes_Point2D (), m_points.size () + (m_closed ? 1 : 0));
        m_foreGeometry->setDrawingMode (GL_LINE_STRIP);
        m_foreGeometry->setLineWidth (m_border);
        QSGGeometry::Point2D * vertex = m_foreGeometry->vertexDataAsPoint2D ();
        const int size = m_points.size ();
        for (int idx = 0; idx < size; idx++) {
            vertex [idx].x = m_points [idx].x ();
            vertex [idx].y = m_points [idx].y ();
        }
        if (m_closed) {
            vertex [size].x = m_points [0].x ();
            vertex [size].y = m_points [0].y ();
        }
        m_foreMaterial = new QSGFlatColorMaterial;
        m_foreMaterial->setColor (m_stroke);
        m_foreNode = new QSGGeometryNode;
        m_foreNode->setGeometry (m_foreGeometry);
        m_foreNode->setMaterial (m_foreMaterial);
        m_node->appendChildNode (m_foreNode);
    }
    return m_node;
}

void QQuickPolygon::processTriangulation (void) {
    // allocate and initialize list of Vertices in polygon
    m_triangles.clear ();
    const int n = m_points.size ();
    if (n >= 3) {
        QVector<int> index (n);
        for (int i = 0; i < n; i++) {
            index [i] = i;
        }
        // remove nv-2 Vertices, creating 1 triangle every time
        int nv = n;
        int count = (2 * nv); // error detection
        QPolygonF triangle (3);
        for (int v = (nv -1), u, w; nv > 2;) {
            // if we loop, it is probably a non-simple polygon
            count--;
            if (count > 0) {
                // three consecutive vertices in current polygon, <u,v,w>
                u = (v    < nv ? v    : 0); // previous
                v = (u +1 < nv ? u +1 : 0); // new v
                w = (v +1 < nv ? v +1 : 0); // next
                triangle [0] = m_points [index [u]];
                triangle [1] = m_points [index [v]];
                triangle [2] = m_points [index [w]];
                QPolygonF result = triangle.intersected (m_points);
                if (result.isClosed ()) {
                    result.removeLast ();
                }
                if (result == triangle) {
                    // output Triangle
                    m_triangles.append (m_points [index [u]]);
                    m_triangles.append (m_points [index [v]]);
                    m_triangles.append (m_points [index [w]]);
                    index.remove (v); // remove v from remaining polygon
                    nv--;
                    count = (2 * nv); // reset error detection counter
                }
            }
            else {
                // Triangulate: ERROR - probable bad polygon!
                break;
            }
        }
    }
}