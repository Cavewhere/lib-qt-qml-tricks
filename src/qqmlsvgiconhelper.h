#ifndef QQMLSVGICONHELPER_H
#define QQMLSVGICONHELPER_H

#include <QObject>
#include <QString>
#include <QColor>
#include <QSvgRenderer>
#include <QQmlProperty>
#include <QQmlPropertyValueSource>

class QQmlSvgIconHelper : public QObject, public QQmlPropertyValueSource {
    Q_OBJECT
    Q_INTERFACES (QQmlPropertyValueSource)
    Q_PROPERTY (int     size  READ getSize  WRITE setSize  NOTIFY sizeChanged)
    Q_PROPERTY (qreal   ratio READ getRatio WRITE setRatio NOTIFY ratioChanged)
    Q_PROPERTY (QColor  color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY (QString icon  READ getIcon  WRITE setIcon  NOTIFY iconChanged)

public:
    explicit QQmlSvgIconHelper (QObject * parent = NULL);
    virtual ~QQmlSvgIconHelper (void);

    virtual void setTarget (const QQmlProperty & target);

    static void setBasePath (const QString & basePath);

    int     getSize  (void) const;
    qreal   getRatio (void) const;
    QColor  getColor (void) const;
    QString getIcon  (void) const;

public slots:
    void setSize  (int     arg);
    void setRatio (qreal   arg);
    void setColor (QColor  arg);
    void setIcon  (QString arg);

signals:
    void sizeChanged  (void);
    void ratioChanged (void);
    void colorChanged (void);
    void iconChanged  (void);

protected:
    void refresh (void);

private:
    int     m_size;
    qreal   m_ratio;
    QColor  m_color;
    QString m_icon;

    QQmlProperty m_property;

    static QString      s_basePath;
    static QString      s_cachePath;
    static QSvgRenderer s_renderer;
};

#endif // QQMLSVGICONHELPER_H
