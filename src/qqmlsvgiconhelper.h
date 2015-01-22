#ifndef QQMLSVGICONHELPER_H
#define QQMLSVGICONHELPER_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QSvgRenderer>
#include <QQmlProperty>
#include <QQmlParserStatus>
#include <QQmlPropertyValueSource>

class QQmlSvgIconHelper : public QObject, public QQmlParserStatus, public QQmlPropertyValueSource {
    Q_OBJECT
    Q_INTERFACES (QQmlParserStatus)
    Q_INTERFACES (QQmlPropertyValueSource)
    Q_PROPERTY (int     size            READ getSize            WRITE setSize            NOTIFY sizeChanged)
    Q_PROPERTY (qreal   verticalRatio   READ getVerticalRatio   WRITE setVerticalRatio   NOTIFY verticalRatioChanged)
    Q_PROPERTY (qreal   horizontalRatio READ getHorizontalRatio WRITE setHorizontalRatio NOTIFY horizontalRatioChanged)
    Q_PROPERTY (QColor  color           READ getColor           WRITE setColor           NOTIFY colorChanged)
    Q_PROPERTY (QString icon            READ getIcon            WRITE setIcon            NOTIFY iconChanged)

public:
    explicit QQmlSvgIconHelper (QObject * parent = NULL);
    virtual ~QQmlSvgIconHelper (void);

    virtual void setTarget (const QQmlProperty & target);
    virtual void classBegin (void);
    virtual void componentComplete (void);

    static void setBasePath  (const QString & basePath);
    static void setCachePath (const QString & cachePath);

    int     getSize             (void) const;
    qreal   getVerticalRatio    (void) const;
    qreal   getHorizontalRatio  (void) const;
    QColor  getColor            (void) const;
    QString getIcon             (void) const;

public slots:
    void setSize            (int     size);
    void setVerticalRatio   (qreal   ratio);
    void setHorizontalRatio (qreal   ratio);
    void setColor           (QColor  color);
    void setIcon            (QString icon);

signals:
    void sizeChanged            (void);
    void verticalRatioChanged   (void);
    void horizontalRatioChanged (void);
    void colorChanged           (void);
    void iconChanged            (void);

protected:
    void refresh (void);

private:
    int     m_size;
    bool    m_ready;
    qreal   m_verticalRatio;
    qreal   m_horizontalRatio;
    QColor  m_color;
    QString m_icon;

    QQmlProperty m_property;

    static QString      s_basePath;
    static QString      s_cachePath;
    static QSvgRenderer s_renderer;
};

#endif // QQMLSVGICONHELPER_H
