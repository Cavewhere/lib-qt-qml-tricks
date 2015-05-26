#ifndef QQMLOBJECTLISTMODEL_H
#define QQMLOBJECTLISTMODEL_H

#include "qqmlmodels.h"

class QQmlObjectListModelPrivate;

class QQmlObjectListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY (int count READ count NOTIFY countChanged)

public: // public factory and casts
    template<class ItemType> static QQmlObjectListModel * create (QObject * parent = Q_NULLPTR) {
        return new QQmlObjectListModel (ItemType::staticMetaObject, parent);
    }
    template<class ItemType> ItemType * getAs (int idx) const {
        return qobject_cast<ItemType *> (get (idx));
    }
    template<class ItemType> ItemType * getByUidAs (QString uid) const {
        return qobject_cast<ItemType *> (getByUid (uid));
    }
    template<class ItemType> QList<ItemType *> listAs (void) const {
        QList<ItemType *> ret;
        ret.reserve (size ());
        for (int idx = 0; idx < count (); idx++) {
            ret.append (qobject_cast<ItemType *> (get (idx)));
        }
        return ret;
    }
    template<class ItemType> class QIterableListWrapper {
    public:
        class const_iterator : public QObjectList::const_iterator {
        public:
            explicit const_iterator (const QObjectList::const_iterator & other) : QObjectList::const_iterator (other) { }
            inline ItemType * const operator * (void) const {
                return qobject_cast<ItemType *> (QObjectList::const_iterator::operator * ());
            }
        };
        explicit QIterableListWrapper (const QQmlObjectListModel & list) : m_list (list) { }
        const_iterator begin (void) const {
            return const_iterator (m_list.begin ());
        }
        const_iterator end (void) const {
            return const_iterator (m_list.end ());
        }
    private:
        const QQmlObjectListModel & m_list;
    };
    template<class ItemType> QIterableListWrapper<ItemType> iterateAs (void) const {
        return QIterableListWrapper<ItemType> (* this);
    }
    virtual ~QQmlObjectListModel (void);

protected: // protected constructor
    explicit QQmlObjectListModel (QMetaObject metaObj, QObject * parent);

public: // QAbstractItemModel interface reimplemented
    virtual int rowCount (const QModelIndex & parent = QModelIndex ()) const;
    virtual bool setData (const QModelIndex & index, const QVariant & value, int role);
    virtual QVariant data (const QModelIndex & index, int role) const;
    virtual QHash<int, QByteArray> roleNames (void) const;
    virtual QObjectList::const_iterator begin (void) const;
    virtual QObjectList::const_iterator end (void) const;

public slots: // public methods API
    void clear (void);
    int size (void) const;
    int count (void) const;
    bool isEmpty (void) const;
    int indexOf (QObject * item) const;
    bool contains (QObject * item) const;
    void append (QObject * item);
    void append (QObjectList itemList);
    void prepend (QObject * item);
    void prepend (QObjectList itemList);
    void insert (int idx, QObject * item);
    void insert (int idx, QObjectList itemList);
    void remove (int idx);
    void remove (QObject * item);
    void move (int idx, int pos);
    void setRoleNameForUid (QByteArray name);
    int roleForName (QByteArray name) const;
    QObject * get (int idx) const;
    QObject * getByUid (QString uid) const;
    QObject * first (void) const;
    QObject * last (void) const;
    QObjectList list (void) const;

signals: // notifiers
    void countChanged (int count);

private: // pimpl
    QQmlObjectListModelPrivate * m_privateImpl;
};

#endif // QQMLOBJECTLISTMODEL_H
