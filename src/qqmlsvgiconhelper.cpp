
#include "qqmlsvgiconhelper.h"

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringBuilder>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QCryptographicHash>

QString      QQmlSvgIconHelper::s_basePath;
QString      QQmlSvgIconHelper::s_cachePath = (QStandardPaths::writableLocation (QStandardPaths::CacheLocation) % "/CachedSvgIcon");
QSvgRenderer QQmlSvgIconHelper::s_renderer;

QQmlSvgIconHelper::QQmlSvgIconHelper (QObject * parent)
    : QObject (parent)
    , m_size  (0)
    , m_ratio (1.0)
    , m_color (QColor ())
    , m_icon  (QString ())
{

}

QQmlSvgIconHelper::~QQmlSvgIconHelper (void) {

}

void QQmlSvgIconHelper::setTarget (const QQmlProperty & target) {
    m_property = target;
    refresh ();
}

void QQmlSvgIconHelper::setBasePath (const QString & basePath) {
    QQmlSvgIconHelper::s_basePath = basePath;
}

int QQmlSvgIconHelper::getSize (void) const {
    return m_size;
}

qreal QQmlSvgIconHelper::getRatio (void) const {
    return m_ratio;
}

QColor QQmlSvgIconHelper::getColor (void) const {
    return m_color;
}

QString QQmlSvgIconHelper::getIcon (void) const {
    return m_icon;
}

void QQmlSvgIconHelper::setSize (int arg) {
    if (m_size != arg) {
        m_size = arg;
        refresh ();
        emit sizeChanged ();
    }
}

void QQmlSvgIconHelper::setRatio (qreal arg) {
    if (m_ratio != arg) {
        m_ratio = arg;
        refresh ();
        emit ratioChanged ();
    }
}

void QQmlSvgIconHelper::setColor (QColor arg) {
    if (m_color != arg) {
        m_color = arg;
        refresh ();
        emit colorChanged ();
    }
}

void QQmlSvgIconHelper::setIcon (QString arg) {
    if (m_icon != arg) {
        m_icon = arg;
        refresh ();
        emit iconChanged ();
    }
}

void QQmlSvgIconHelper::refresh (void) {
    if (!m_icon.isEmpty () && m_size > 0) {
        QImage image (m_size, m_size * m_ratio, QImage::Format_ARGB32);
        QString uri (m_icon
                     % "?color=" % m_color.name ()
                     % "&width=" % QString::number (image.width ())
                     % "&height=" % QString::number (image.height ()));
        QString hash (QCryptographicHash::hash (uri.toLocal8Bit (), QCryptographicHash::Md5).toHex ());
        QString sourcePath (s_basePath  % "/" % m_icon % ".svg");
        QString cachedPath (s_cachePath % "/" % hash   % ".png");
        if (!QFile::exists (cachedPath)) {
            QPainter painter (&image);
            image.fill (Qt::transparent);
            painter.setRenderHint (QPainter::Antialiasing,            true);
            painter.setRenderHint (QPainter::SmoothPixmapTransform,   true);
            painter.setRenderHint (QPainter::HighQualityAntialiasing, true);
            s_renderer.load (sourcePath);
            if (s_renderer.isValid ()) {
                s_renderer.render (&painter);
                if (m_color.isValid ()) {
                    QColor tmp (m_color);
                    for (int x (0); x < image.width (); x++) {
                        for (int y (0); y < image.height (); y++) {
                            tmp.setAlpha (qAlpha (image.pixel (x, y)));
                            image.setPixel (x, y, tmp.rgba ());
                        }
                    }
                }
                QDir ().mkpath (s_cachePath);
                image.save (cachedPath, "PNG", 0);
            }
        }
        m_property.write (QUrl::fromLocalFile (cachedPath));
    }
}
