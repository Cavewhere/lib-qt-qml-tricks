
#include "qqmlsvgiconhelper.h"

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QPainter>
#include <QStringBuilder>
#include <QCoreApplication>
#include <QCryptographicHash>

QString      QQmlSvgIconHelper::s_basePath;
QString      QQmlSvgIconHelper::s_cachePath;
QSvgRenderer QQmlSvgIconHelper::s_renderer;

QQmlSvgIconHelper::QQmlSvgIconHelper (QObject * parent)
    : QObject           (parent)
    , m_size            (0)
    , m_ready           (false)
    , m_verticalRatio   (1.0)
    , m_horizontalRatio (1.0)
    , m_color           (Qt::transparent)
    , m_icon            (QString ())
{
    if (s_basePath.isEmpty ()) {
        QQmlSvgIconHelper::s_basePath = qApp->applicationDirPath ();
    }
    if (s_cachePath.isEmpty ()) {
         QQmlSvgIconHelper::s_cachePath = (QDir::homePath () % "/.CachedSvgIcon/" % qApp->applicationName ());
    }
}

QQmlSvgIconHelper::~QQmlSvgIconHelper (void) {

}

void QQmlSvgIconHelper::classBegin (void) {
    m_ready = false;
}

void QQmlSvgIconHelper::componentComplete (void) {
    m_ready = true;
    refresh ();
}

void QQmlSvgIconHelper::setTarget (const QQmlProperty & target) {
    m_property = target;
    refresh ();
}

void QQmlSvgIconHelper::setBasePath (const QString & basePath) {
    QQmlSvgIconHelper::s_basePath = basePath;
}

void QQmlSvgIconHelper::setCachePath (const QString & cachePath) {
    QQmlSvgIconHelper::s_cachePath = cachePath;
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
    if (m_ready) {
        QUrl url;
        if (!m_icon.isEmpty () && m_size > 0 && m_horizontalRatio > 0.0 && m_verticalRatio > 0.0) {
            QImage image (m_size * m_horizontalRatio, m_size * m_verticalRatio, QImage::Format_ARGB32);
            QString uri (m_icon
                         % "?color="  % (m_color.isValid () ? m_color.name () : "none")
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
                if (QFile::exists (sourcePath)) {
                    s_renderer.load (sourcePath);
                    if (s_renderer.isValid ()) {
                        s_renderer.render (&painter);
                        if (m_color.isValid () && m_color.alpha () > 0) {
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
                        url = QUrl::fromLocalFile (cachedPath);
                    }
                }
                else {
                    qWarning () << ">>> QmlSvgIconHelper : Can't render" << sourcePath << ", no such file !";
                }
            }
            else {
                url = QUrl::fromLocalFile (cachedPath);
            }
        }
        if (m_property.isValid () && m_property.isWritable ()) {
            m_property.write (url);
        }
    }
}
