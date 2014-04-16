#ifndef QQMLVARIANTLISTMODEL_P_H
#define QQMLVARIANTLISTMODEL_P_H

#include <QByteArray>
#include <QChar>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QString>
#include <QVariant>
#include <QVector>

class QQmlVariantListModel;

class QQmlVariantListModelPrivate : public QObject {
    Q_OBJECT

public:
    explicit QQmlVariantListModelPrivate (QQmlVariantListModel * parent);

    void updateCounter ();

    int                     m_count;
    QVariantList            m_items;
    QHash<int, QByteArray>  m_roles;
    QQmlVariantListModel  * m_publicObject;
};


#endif // QQMLVARIANTLISTMODEL_P_H
