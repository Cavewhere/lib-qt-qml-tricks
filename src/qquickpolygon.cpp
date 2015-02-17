
#include "qquickpolygon.h"

#include <QSGNode>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>

QQuickPolygon::QQuickPolygon (QQuickItem * parent)
    : QQuickItem (parent)
    , m_color (Qt::transparent)
{
    setFlag (QQuickItem::ItemHasContents);
}

QQuickPolygon::~QQuickPolygon (void) { }

QVariantList QQuickPolygon::getPoints (void) const {
    QVariantList ret;
    foreach (QPointF point, m_points) {
        ret.append (point);
    }
    return ret;
}

QColor QQuickPolygon::getColor (void) const {
    return m_color;
}

void QQuickPolygon::setPoints (QVariantList points) {
    m_points.clear ();
    foreach (QVariant tmp, points) {
        m_points.append (tmp.value<QPointF> ());
    }
    emit pointsChanged ();
    update ();
}

void QQuickPolygon::setColor (QColor color) {
    if (m_color != color) {
        m_color = color;
        emit colorChanged ();
        update ();
    }
}

QSGNode * QQuickPolygon::updatePaintNode (QSGNode * oldNode, UpdatePaintNodeData * updatePaintNodeData) {
    Q_UNUSED (updatePaintNodeData)
    if (oldNode) {
        delete oldNode;
    }
    QSGNode * ret = NULL;
    const QVector<QPointF> triangles = processTriangulation ();
    const int size = triangles.size ();
    if (size > 0 && m_color.alpha () > 0) {
        QSGGeometry * geometry = new QSGGeometry (QSGGeometry::defaultAttributes_Point2D (), size);
        geometry->setDrawingMode (GL_TRIANGLE_STRIP);
        QSGGeometry::Point2D * vertex = geometry->vertexDataAsPoint2D ();
        for (int idx = 0; idx < size; idx++) {
            vertex [idx].x = triangles [idx].x ();
            vertex [idx].y = triangles [idx].y ();
        }
        QSGFlatColorMaterial * material = new QSGFlatColorMaterial;
        material->setColor (m_color);
        QSGGeometryNode * newNode = new QSGGeometryNode;
        newNode->setFlag (QSGNode::OwnsGeometry);
        newNode->setFlag (QSGNode::OwnsMaterial);
        newNode->setMaterial (material);
        newNode->setGeometry (geometry);
        ret = newNode;
    }
    return ret;
}

bool QQuickPolygon::isInsideTriangle (qreal Ax, qreal Ay, qreal Bx, qreal By, qreal Cx, qreal Cy, qreal Px, qreal Py) {
    qreal ax  (Cx - Bx);
    qreal ay  (Cy - By);
    qreal bx  (Ax - Cx);
    qreal by  (Ay - Cy);
    qreal cx  (Bx - Ax);
    qreal cy  (By - Ay);
    qreal apx (Px - Ax);
    qreal apy (Py - Ay);
    qreal bpx (Px - Bx);
    qreal bpy (Py - By);
    qreal cpx (Px - Cx);
    qreal cpy (Py - Cy);
    qreal aCROSSbp (ax * bpy - ay * bpx);
    qreal cCROSSap (cx * apy - cy * apx);
    qreal bCROSScp (bx * cpy - by * cpx);
    return ((aCROSSbp >= 0.0) && (bCROSScp >= 0.0) && (cCROSSap >= 0.0));
}

bool QQuickPolygon::snip (int u, int v, int w, int n, int * V) {
    static const qreal EPSILON (0.0000000001);
    bool ret (false);
    qreal Ax (m_points [V [u]].x ());
    qreal Ay (m_points [V [u]].y ());
    qreal Bx (m_points [V [v]].x ());
    qreal By (m_points [V [v]].y ());
    qreal Cx (m_points [V [w]].x ());
    qreal Cy (m_points [V [w]].y ());
    if (EPSILON <= (((Bx - Ax) * (Cy - Ay)) - ((By - Ay) * (Cx - Ax)))) {
        for (int p = 0; p < n; p++) {
            if ((p == u) || (p == v) || (p == w)) {
                continue;
            }
            qreal Px (m_points [V [p]].x ());
            qreal Py (m_points [V [p]].y ());
            if (isInsideTriangle (Ax, Ay, Bx, By, Cx, Cy, Px, Py)) {
                ret = false;
                break;
            }
        }
        ret = true;
    }
    return ret;
}

QVector<QPointF> QQuickPolygon::processTriangulation (void) {
    QVector<QPointF> triangles;
    // allocate and initialize list of Vertices in polygon
    const int n (m_points.size ());
    if (n >= 3) {
        int * V = new int [n];
        // compute polygon area
        qreal area (0.0);
        for (int p = (n -1), q = 0; q < n; p = q++) {
            area += (m_points [p].x () * m_points [q].y () - m_points [q].x () * m_points [p].y ());
        }
        // we want a counter-clockwise polygon in V
        if (area > 0.0) {
            for (int v = 0; v < n; v++) {
                V [v] = v;
            }
        }
        else {
            for (int v = 0; v < n; v++) {
                V [v] = (n - v -1);
            }
        }
        int nv (n);
        // remove nv-2 Vertices, creating 1 triangle every time
        int count (2 * nv); // error detection
        for (int m = 0, v = (nv -1); nv > 2;) {
            // if we loop, it is probably a non-simple polygon
            if (0 >= (count--)) {
                break; // Triangulate: ERROR - probable bad polygon!
            }
            // three consecutive vertices in current polygon, <u,v,w>
            int u = v;
            if (nv <= u) {
                u = 0; // previous
            }
            v = (u +1);
            if (nv <= v) {
                v = 0; // new v
            }
            int w = (v +1);
            if (nv <= w) {
                w = 0; // next
            }
            if (snip (u, v, w, nv, V)) {
                // output Triangle
                triangles.append (m_points [V [u]]);
                triangles.append (m_points [V [v]]);
                triangles.append (m_points [V [w]]);
                m++;
                // remove v from remaining polygon
                for (int s = v, t = (v +1); t < nv; s++, t++) {
                    V [s] = V [t];
                }
                nv--;
                // reset error detection counter
                count = (2 * nv);
            }
        }
        delete V;
    }
    return triangles;
}
