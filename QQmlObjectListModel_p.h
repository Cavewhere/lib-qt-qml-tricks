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

#include "QQmlObjectListModel.h"

class QQmlObjectListModelPrivate : public QObject {
    Q_OBJECT

public:
    explicit QQmlObjectListModelPrivate (QQmlObjectListModel * parent);

    Q_SLOT void onItemPropertyChanged ();

    void prepareItem (QObject * item);

    QMetaObject            m_metaObj;
    QMetaMethod            m_handler;
    QObjectList            m_items;
    QHash<int, QByteArray> m_roles;
    QHash<int, int>        m_signalIdxToRole;
    QQmlObjectListModel  * d_func;
};

#endif // QQMLOBJECTLISTMODEL_P_H
