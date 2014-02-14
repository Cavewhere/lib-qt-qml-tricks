#include "QQmlObjectListModel.h"
#include "QQmlObjectListModel_p.h"

QQmlObjectListModel::QQmlObjectListModel (QMetaObject metaObj, QObject * parent)
    : QAbstractListModel (parent)
    , m_privateImpl (new QQmlObjectListModelPrivate (this))
{
    m_privateImpl->m_metaObj = metaObj;
    m_privateImpl->m_roles.insert (0, "qtObject");
    for (int propertyIdx = 0; propertyIdx < m_privateImpl->m_metaObj.propertyCount (); propertyIdx++) {
        int role = (propertyIdx +1);
        QMetaProperty metaProp = m_privateImpl->m_metaObj.property (propertyIdx);
        m_privateImpl->m_roles.insert (role, metaProp.name ());
        if (metaProp.hasNotifySignal ()) {
            m_privateImpl->m_signalIdxToRole.insert (metaProp.notifySignalIndex (), role);
        }
    }
}

int QQmlObjectListModel::rowCount (const QModelIndex & parent) const
{
    Q_UNUSED (parent);
    return m_privateImpl->m_items.count ();
}

QVariant QQmlObjectListModel::data (const QModelIndex & index, int role) const
{
    QVariant ret;
    QObject * item = get (index.row ());
    if (item != NULL) {
        ret.setValue (role > 0 ? item->property (m_privateImpl->m_roles.value (role)) : QVariant::fromValue (item));
    }
    return ret;
}

QHash<int, QByteArray> QQmlObjectListModel::roleNames () const
{
    return m_privateImpl->m_roles;
}

bool QQmlObjectListModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
    bool ret = false;
    QObject * item = get (index.row ());
    if (item != NULL && role > 0) {
        ret = item->setProperty (m_privateImpl->m_roles.value (role), value);
    }
    return ret;
}

int QQmlObjectListModel::roleForName (QByteArray name) const
{
    return m_privateImpl->m_roles.key (name, -1);
}

int QQmlObjectListModel::count () const
{
    return m_privateImpl->m_items.size ();
}

bool QQmlObjectListModel::isEmpty () const
{
    return m_privateImpl->m_items.isEmpty ();
}

bool QQmlObjectListModel::contains (QObject * item) const
{
    return m_privateImpl->m_items.contains (item);
}

int QQmlObjectListModel::indexOf (QObject * item) const
{
    return m_privateImpl->m_items.indexOf (item);
}

void QQmlObjectListModel::clear () {
    beginResetModel ();
    foreach (QObject * item, m_privateImpl->m_items)
    {
        if (item != NULL) {
            item->deleteLater ();
        }
    }
    m_privateImpl->m_items.clear ();
    endResetModel ();
}

void QQmlObjectListModel::append (QObject * item)
{
    if (item != NULL) {
        beginInsertRows (QModelIndex (), m_privateImpl->m_items.count (), m_privateImpl->m_items.count ());
        m_privateImpl->m_items.append (item);
        m_privateImpl->prepareItem (item);
        endInsertRows ();
    }
}

void QQmlObjectListModel::prepend (QObject * item)
{
    if (item != NULL) {
        beginInsertRows (QModelIndex (), 0, 0);
        m_privateImpl->m_items.prepend (item);
        m_privateImpl->prepareItem (item);
        endInsertRows ();
    }
}

void QQmlObjectListModel::insert (int idx, QObject * item)
{
    if (item != NULL) {
        beginInsertRows (QModelIndex (), idx, idx);
        m_privateImpl->m_items.insert (idx, item);
        m_privateImpl->prepareItem (item);
        endInsertRows ();
    }
}

void QQmlObjectListModel::remove (QObject * item)
{
    if (item != NULL) {
        int idx = m_privateImpl->m_items.indexOf (item);
        remove (idx);
    }
}

void QQmlObjectListModel::remove (int idx)
{
    if (idx >= 0 && idx < m_privateImpl->m_items.size ()) {
        beginRemoveRows (QModelIndex (), idx, idx);
        QObject * item = m_privateImpl->m_items.takeAt (idx);
        if (item != NULL) {
            item->disconnect ();
            if (item->parent () == this) { // FIXME : maybe that's not the best way to test ownership ?
                item->deleteLater ();
            }
        }
        endRemoveRows ();
    }
}

QObject * QQmlObjectListModel::get (int idx) const
{
    QObject * ret = NULL;
    if (idx >= 0 && idx < m_privateImpl->m_items.size ()) {
        ret = m_privateImpl->m_items.value (idx);
    }
    return ret;
}

QObjectList QQmlObjectListModel::list () const {
    return m_privateImpl->m_items;
}

QQmlObjectListModelPrivate::QQmlObjectListModelPrivate (QQmlObjectListModel * parent)
    : QObject (parent)
    , m_publicObject (parent)
{
    for (int methodIdx = 0; methodIdx < metaObject ()->methodCount (); methodIdx++) {
        QMetaMethod metaMethod = metaObject ()->method (methodIdx);
        if (metaMethod.name () == "onItemPropertyChanged") { // FIXME : maybe find a cleaner way to get it
            m_handler = metaMethod;
            break;
        }
    }
}

void QQmlObjectListModelPrivate::onItemPropertyChanged ()
{
    int row = m_items.indexOf (sender ());
    int role = m_signalIdxToRole.key (senderSignalIndex ());
    if (row >= 0 && role >= 0) {
        QModelIndex idx = m_publicObject->index (row, 0, QModelIndex ());
        emit m_publicObject->dataChanged (idx, idx, QVector<int> () << role);
    }
}

void QQmlObjectListModelPrivate::prepareItem (QObject * item)
{
    if (item != NULL) {
        if (item->parent () == NULL) {
            item->setParent (this);
        }
        foreach (int signalIdx, m_signalIdxToRole.keys ()) {
            QMetaMethod notifier = item->metaObject ()->method (signalIdx);
            QObject::connect (item, notifier,
                              this, m_handler,
                              (Qt::ConnectionType) (Qt::DirectConnection | Qt::UniqueConnection));
        }
    }
}
