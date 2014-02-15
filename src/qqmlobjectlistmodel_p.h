#ifndef QQMLOBJECTLISTMODEL_P_H
#define QQMLOBJECTLISTMODEL_P_H

#include <QByteArray>
#include <QChar>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QString>
#include <QVariant>
#include <QVector>

class QQmlObjectListModel;

class QQmlObjectListModelPrivate : public QObject {
    Q_OBJECT

public:
    explicit QQmlObjectListModelPrivate (QQmlObjectListModel * parent);

    Q_SLOT void onItemPropertyChanged ();

    void referenceItem   (QObject * item);
    void dereferenceItem (QObject * item);

    QMetaObject            m_metaObj;
    QMetaMethod            m_handler;
    QObjectList            m_items;
    QHash<int, QByteArray> m_roles;
    QHash<int, int>        m_signalIdxToRole;
    QQmlObjectListModel  * m_publicObject;
};

#endif // QQMLOBJECTLISTMODEL_P_H
