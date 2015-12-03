#ifndef QQMLGADGETLISTMODEL_H
#define QQMLGADGETLISTMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QChar>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QMetaMethod>
#include <QMetaObject>
#include <QMetaProperty>
#include <QObject>
#include <QString>
#include <QStringBuilder>
#include <QVariant>
#include <QVector>

template<typename T> QList<T> qListFromVariant (const QVariantList & list) {
    QList<T> ret;
    ret.reserve (list.size ());
    for (QVariantList::const_iterator it = list.constBegin (); it != list.constEnd (); it++) {
        const QVariant & var = (QVariant) (* it);
        ret.append (var.value<T> ());
    }
    return ret;
}

template<typename T> QVariantList qListToVariant (const QList<T> & list) {
    QVariantList ret;
    ret.reserve (list.size ());
    for (typename QList<T>::const_iterator it = list.constBegin (); it != list.constEnd (); it++) {
        const T & val = (T) (* it);
        ret.append (QVariant::fromValue (val));
    }
    return ret;
}

// custom foreach for QList, which uses no copy and check pointer non-null
/*
#define FOREACH_PTR_IN_QLIST(_type_, _var_, _list_) \
    for (typename QList<_type_ *>::const_iterator it = _list_.begin (); it != _list_.end (); it++) \
        for (_type_ * _var_ = (_type_ *) (* it); _var_ != Q_NULLPTR; _var_ = Q_NULLPTR)
*/

class QQmlGadgetListModelBase : public QAbstractListModel { // abstract Qt base class
    Q_OBJECT
    Q_PROPERTY (int count READ count NOTIFY countChanged)

public:
    explicit QQmlGadgetListModelBase (QObject * parent = Q_NULLPTR) : QAbstractListModel (parent) { }

    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) {
        return QAbstractListModel::setData(index, value, role);
    }

    virtual QVariant data(const QModelIndex& index, int role) const = 0;

public slots: // virtual methods API for QML
    virtual int size (void) const = 0;
    virtual int count (void) const = 0;
    virtual bool isEmpty (void) const = 0;
    virtual bool contains (QVariant item) const = 0;
    virtual int indexOf (QVariant item) const = 0;
    virtual int roleForName (const QByteArray & name) const = 0;
    virtual void clear (void) = 0;
    virtual void append (QVariant item) = 0;
    virtual void prepend (QVariant item) = 0;
    virtual void insert (int idx, QVariant item) = 0;
    virtual void move (int idx, int pos) = 0;
    virtual void remove (QVariant item) = 0;
    virtual void remove (int idx) = 0;
    virtual void replace(int idx, QVariant item) = 0;
    virtual QVariant get (int idx) const = 0;
    virtual QVariant get (const QString & uid) const = 0;
    virtual QVariant getFirst (void) const = 0;
    virtual QVariant getLast (void) const = 0;
    virtual QVariantList toVarArray (void) const = 0;
    virtual bool setData(int idx, const QVariant& value, const QByteArray& roleName) = 0;
    virtual QVariant data(int idx, const QByteArray& roleName) const = 0;

//protected slots: // internal callback
//    virtual void onItemPropertyChanged (void) = 0;

signals: // notifier
    void countChanged (void);
};

template<class ItemType> class QQmlGadgetListModel : public QQmlGadgetListModelBase {
public:
    explicit QQmlGadgetListModel (QObject *          parent      = Q_NULLPTR,
                                  const QByteArray & displayRole = QByteArray (),
                                  const QByteArray & uidRole     = QByteArray ())
        : QQmlGadgetListModelBase (parent)
        , m_count (0)
        , m_uidRoleName (uidRole)
        , m_dispRoleName (displayRole)
        , m_metaObj (ItemType::staticMetaObject)
    {
        static QSet<QByteArray> roleNamesBlacklist;
        if (roleNamesBlacklist.isEmpty ()) {
            roleNamesBlacklist << QByteArrayLiteral ("id")
                               << QByteArrayLiteral ("index")
                               << QByteArrayLiteral ("class")
                               << QByteArrayLiteral ("model")
                               << QByteArrayLiteral ("modelData");
        }
        static const char * HANDLER = "onItemPropertyChanged()";
        m_handler = metaObject ()->method (metaObject ()->indexOfMethod (HANDLER));
        if (!displayRole.isEmpty ()) {
            m_roles.insert (Qt::DisplayRole, QByteArrayLiteral ("display"));
        }
        m_roles.insert (baseRole (), QByteArrayLiteral ("qtObject"));
        const int len = m_metaObj.propertyCount ();
        for (int propertyIdx = 0, role = (baseRole () +1); propertyIdx < len; propertyIdx++, role++) {
            QMetaProperty metaProp = m_metaObj.property (propertyIdx);
            const QByteArray propName = QByteArray (metaProp.name ());
            if (!roleNamesBlacklist.contains (propName)) {
                m_roles.insert (role, propName);
                if (metaProp.hasNotifySignal ()) {
                    m_signalIdxToRole.insert (metaProp.notifySignalIndex (), role);
                }
            }
            else {
                static const QByteArray CLASS_NAME = (QByteArrayLiteral ("QQmlGadgetListModel<") % m_metaObj.className () % '>');
                qWarning () << "Can't have" << propName << "as a role name in" << qPrintable (CLASS_NAME);
            }
        }
    }
    bool setData (const QModelIndex & index, const QVariant & value, int role) {
        bool ret = false;
        int row = index.row();
        if(row >= 0 && row < m_items.size()) {
            ItemType& item = m_items[row];
            const QByteArray rolename = (role != Qt::DisplayRole ? m_roles.value (role, emptyBA ()) : m_dispRoleName);
            if (role != baseRole () && !rolename.isEmpty ()) {
                int propertyIndex = m_metaObj.indexOfProperty(rolename);
                ret = m_metaObj.property(propertyIndex).writeOnGadget(&item, value);
                if (ret) {
                    QVector<int> roles;
                    roles.resize(1);
                    roles.first() = role;
                    emit dataChanged(index, index, roles);
                }
            }
        }
        return ret;
    }
    QVariant data (const QModelIndex & index, int role) const {
        QVariant ret;
        int row = index.row();
        if(row >= 0 && row < m_items.size()) {
            const ItemType& item = at (row);
            const QByteArray rolename = (role != Qt::DisplayRole ? m_roles.value (role, emptyBA ()) : m_dispRoleName);
            if (!rolename.isEmpty ()) {
                int propertyIndex = m_metaObj.indexOfProperty(rolename);
                ret.setValue (role != baseRole () ? m_metaObj.property(propertyIndex).readOnGadget(&item) : QVariant::fromValue (item));
            }
        }
        return ret;
    }
    QHash<int, QByteArray> roleNames (void) const {
        return m_roles;
    }
    typedef typename QList<ItemType>::iterator iterator;
    iterator begin (void) {
        return m_items.begin ();
    }
    iterator end (void) {
        return m_items.end ();
    }
    typedef typename QList<ItemType>::const_iterator const_iterator;
    const_iterator constBegin (void) const {
        return m_items.constBegin ();
    }
    const_iterator constEnd (void) const {
        return m_items.constEnd ();
    }

public: // C++ API
    ItemType at (int idx) const {
        ItemType ret;
        if (idx >= 0 && idx < m_items.size ()) {
            ret = m_items.value (idx);
        }
        return ret;
    }
    ItemType getByUid (const QString & uid) const {
        ItemType ret;
        if(!m_indexByUid.contains(uid)) {
            return *(m_indexByUid.value (uid, Q_NULLPTR));
        }
        return ret;
    }
    int roleForName (const QByteArray & name) const {
        return m_roles.key (name, -1);
    }
    int count (void) const {
        return m_count;
    }
    int size (void) const {
        return m_count;
    }
    bool isEmpty (void) const {
        return m_items.isEmpty ();
    }
    bool contains (ItemType item) const {
        return m_items.contains (item);
    }
    int indexOf (ItemType item) const {
        return m_items.indexOf (item);
    }
    void clear (void) {
        if (!m_items.isEmpty ()) {
            beginRemoveRows (noParent (), 0, m_items.count () -1);
            for(int i = 0; i < m_items.size(); i++) {
                dereferenceItem(&m_items[i]);
            }
            m_items.clear ();
            endRemoveRows ();
            updateCounter ();
        }
    }
    void append (ItemType item) {
        const int pos = m_items.count ();
        beginInsertRows (noParent (), pos, pos);
        m_items.append (item);
        referenceItem (&m_items.last());
        endInsertRows ();
        updateCounter ();
    }
    void prepend (ItemType item) {
        beginInsertRows (noParent (), 0, 0);
        m_items.prepend (item);
        referenceItem (&m_items.first());
        endInsertRows ();
        updateCounter ();
    }
    void insert (int idx, ItemType item) {
        beginInsertRows (noParent (), idx, idx);
        m_items.insert (idx, item);
        referenceItem (&m_items[idx]);
        endInsertRows ();
        updateCounter ();
    }
    void append (const QList<ItemType> & itemList) {
        insert(m_items.count(), itemList);
    }
    void prepend (const QList<ItemType> & itemList) {
        insert(0, itemList);
    }
    void insert (int idx, const QList<ItemType> & itemList) {
        if (!itemList.isEmpty ()) {
            beginInsertRows (noParent (), idx, idx + itemList.count () -1);
            m_items.reserve (m_items.count () + itemList.count ());
            for(int i = 0; i < itemList.count(); i++) {
                m_items.insert(idx + i, itemList.at(i));
                referenceItem(&m_items[i]);
            }
            endInsertRows ();
            updateCounter ();
        }
    }
    void move (int idx, int pos) {
        if (idx != pos) {
            const int lowest  = qMin (idx, pos);
            const int highest = qMax (idx, pos);
            beginMoveRows (noParent (), highest, highest, noParent (), lowest);
            m_items.move (highest, lowest);
            endMoveRows ();
        }
    }
    void remove (ItemType item) {
        const int idx = m_items.indexOf (item);
        remove (idx);
    }
    void remove (int idx) {
        if (idx >= 0 && idx < m_items.size ()) {
            beginRemoveRows (noParent (), idx, idx);
            ItemType& item = m_items[idx]; //.takeAt (idx);
            dereferenceItem (&item);
            m_items.takeAt(idx);
            endRemoveRows ();
            updateCounter ();
        }
    }
    void replace (int idx, ItemType item) {
        if(idx >= 0 && idx < m_items.size()) {
            if(item != m_items.at(idx)) {
                m_items.replace(idx, item);
                QModelIndex modelIndex = index(idx);
                dataChanged(modelIndex, modelIndex, roleNames().keys().toVector());
            }
        }
    }
    ItemType first (void) const {
        return m_items.first ();
    }
    ItemType last (void) const {
        return m_items.last ();
    }
    const QList<ItemType> & toList (void) const {
        return m_items;
    }

public: // QML slots implementation
    void append (QVariant item) {
        append (item.value<ItemType>());
    }
    void prepend (QVariant item) {
        prepend (item.value<ItemType>());
    }
    void insert (int idx, QVariant item) {
        insert (idx, item.value<ItemType>());
    }
    void remove (QVariant item) {
        remove (item.value<ItemType>());
    }
    void replace (int idx, QVariant item) {
        replace (idx, item.value<ItemType>());
    }
    bool contains (QVariant item) const {
        return contains (item.value<ItemType>());
    }
    int indexOf (QVariant item) const {
        return indexOf (item.value<ItemType>());
    }
    int indexOf (const QString & uid) const {
        return indexOf (get (uid));
    }
    QVariant get (int idx) const {
        return QVariant::fromValue(at(idx));
    }
    QVariant get (const QString & uid) const {
        return QVariant::fromValue(getByUid(uid));
    }
    QVariant getFirst (void) const {
        return QVariant::fromValue(first());
    }
    QVariant getLast (void) const {
        return QVariant::fromValue(last());
    }
    QVariantList toVarArray (void) const {
        return qListToVariant<ItemType> (m_items);
    }
    bool setData(int idx, const QVariant& value, const QByteArray& roleName) {
        return setData(index(idx), value, roleForName(roleName));
    }
    QVariant data(int idx, const QByteArray& rolename) const {
        return data(index(idx), roleForName(rolename));
    }


protected: // internal stuff
    static const QString & emptyStr (void) {
        static const QString ret = QStringLiteral ("");
        return ret;
    }
    static const QByteArray & emptyBA (void) {
        static const QByteArray ret = QByteArrayLiteral ("");
        return ret;
    }
    static const QModelIndex & noParent (void) {
        static const QModelIndex ret = QModelIndex ();
        return ret;
    }
    static const int & baseRole (void) {
        static const int ret = Qt::UserRole;
        return ret;
    }
    int rowCount (const QModelIndex & parent = QModelIndex ()) const {
        Q_UNUSED (parent);
        return m_items.count ();
    }
    void referenceItem (ItemType * item) {
        if (item != Q_NULLPTR) {
            if (!m_uidRoleName.isEmpty ()) {
                const QString key = m_indexByUid.key (item, emptyStr ());
                if (!key.isEmpty ()) {
                    m_indexByUid.remove (key);
                }
                int propertyIndex = m_metaObj.indexOfProperty(m_uidRoleName);
                const QString value = m_metaObj.property(propertyIndex).readOnGadget(item).toString ();
                if (!value.isEmpty ()) {
                    m_indexByUid.insert (value, item);
                }
            }
        }
    }
    void dereferenceItem (ItemType* item) {
        if (item != Q_NULLPTR) {
            if (!m_uidRoleName.isEmpty ()) {
                const QString key = m_indexByUid.key (item, emptyStr ());
                if (!key.isEmpty ()) {
                    m_indexByUid.remove (key);
                }
            }
        }
    }

    inline void updateCounter (void) {
        if (m_count != m_items.count ()) {
            m_count = m_items.count ();
            emit countChanged ();
        }
    }


private: // data members
    int                        m_count;
    QByteArray                 m_uidRoleName;
    QByteArray                 m_dispRoleName;
    QMetaObject                m_metaObj;
    QMetaMethod                m_handler;
    QHash<int, QByteArray>     m_roles;
    QHash<int, int>            m_signalIdxToRole;
    QList<ItemType>            m_items;
    QHash<QString, ItemType*>  m_indexByUid;
};

#define QML_OBJMODEL_PROPERTY(type, name) \
    protected: Q_PROPERTY (QQmlGadgetListModelBase * name READ get_##name CONSTANT) \
    private: QQmlGadgetListModel<type> * m_##name; \
    public: QQmlGadgetListModel<type> * get_##name (void) const { return m_##name; } \
    private:

#endif // QQMLOBJECTLISTMODEL_H
