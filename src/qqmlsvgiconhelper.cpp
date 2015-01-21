
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
    : QObject           (parent)
    , m_size            (0)
    , m_verticalRatio   (1.0)
    , m_horizontalRatio (1.0)
    , m_color           (QColor  ())
    , m_icon            (QString ())
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

qreal QQmlSvgIconHelper::getVerticalRatio (void) const {
    return m_verticalRatio;
}

qreal QQmlSvgIconHelper::getHorizontalRatio (void) const {
    return m_horizontalRatio;
}

QColor QQmlSvgIconHelper::getColor (void) const {
    return m_color;
}

QString QQmlSvgIconHelper::getIcon (void) const {
    return m_icon;
}

void QQmlSvgIconHelper::setSize (int size) {
    if (m_size != size) {
        m_size = size;
        refresh ();
        emit sizeChanged ();
    }
}

void QQmlSvgIconHelper::setVerticalRatio (qreal ratio) {
    if (m_verticalRatio != ratio) {
        m_verticalRatio = ratio;
        refresh ();
        emit verticalRatioChanged ();
    }
}

void QQmlSvgIconHelper::setHorizontalRatio (qreal ratio) {
    if (m_horizontalRatio != ratio) {
        m_horizontalRatio = ratio;
        refresh ();
        emit horizontalRatioChanged ();
    }
}

void QQmlSvgIconHelper::setColor (QColor color) {
    if (m_color != color) {
        m_color = color;
        refresh ();
        emit colorChanged ();
    }
}

void QQmlSvgIconHelper::setIcon (QString icon) {
    if (m_icon != icon) {
        m_icon = icon;
        refresh ();
        emit iconChanged ();
    }
}

void QQmlSvgIconHelper::refresh (void) {
    if (!m_icon.isEmpty () && m_size > 0) {
        QImage image (m_size * m_horizontalRatio, m_size * m_verticalRatio, QImage::Format_ARGB32);
        QString uri (m_icon
                     % "?color="  % m_color.name ()
                     % "&width="  % QString::number (image.width  ())
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
