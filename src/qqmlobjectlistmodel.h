#ifndef QQMLOBJECTLISTMODEL_H
#define QQMLOBJECTLISTMODEL_H

#include "qqmlmodels.h"

class QQmlObjectListModelPrivate;

class QQmlObjectListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY (int count READ count NOTIFY countChanged)

public: // public factory and casts
    template <class ItemType> static QQmlObjectListModel * create (QObject * parent = NULL)
    {
        return new QQmlObjectListModel (ItemType::staticMetaObject, parent);
    }
    template <class ItemType> ItemType * getAs (int idx) const
    {
        return qobject_cast<ItemType *> (get (idx));
    }
    template <class ItemType> QList<ItemType *> listAs () const
    {
        QList<ItemType *> ret;
        for (int idx = 0; idx < count (); idx++) {
            ret.append (qobject_cast<ItemType *> (get (idx)));
        }
        return ret;
    }
    virtual ~QQmlObjectListModel ();

protected: // protected constructor
    explicit QQmlObjectListModel (QMetaObject metaObj, QObject * parent);

public: // QAbstractItemModel interface reimplemented
    virtual int rowCount (const QModelIndex & parent = QModelIndex ()) const;
    virtual bool setData (const QModelIndex & index, const QVariant & value, int role);
    virtual QVariant data (const QModelIndex & index, int role) const;
    virtual QHash<int, QByteArray> roleNames () const;

public slots: // public methods API
    void clear ();
    int count () const;
    bool isEmpty () const;
    bool contains (QObject * item) const;
    int indexOf (QObject * item) const;
    int roleForName (QByteArray name) const;
    void append (QObject * item);
    void prepend (QObject * item);
    void insert (int idx, QObject * item);
    void append (QObjectList itemList);
    void prepend (QObjectList itemList);
    void insert (int idx, QObjectList itemList);
    void move (int idx, int pos);
    void remove (int idx);
    void remove (QObject * item);
    QObject * get (int idx) const;
    QObject * first () const;
    QObject * last () const;
    QObjectList list () const;
    QObject * getByUid (QString uid) const;
    void setRoleNameForUid (QByteArray name);

signals: // notifiers
    void countChanged (int count);

private: // pimpl
    QQmlObjectListModelPrivate * m_privateImpl;
};

#endif // QQMLOBJECTLISTMODEL_H
