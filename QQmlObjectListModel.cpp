#include "QQmlObjectListModel.h"
#include "QQmlObjectListModel_p.h"

QQmlObjectListModel::QQmlObjectListModel (QMetaObject metaObj, QObject * parent)
    : QAbstractListModel (parent)
    , d_ptr (new QQmlObjectListModelPrivate (this))
{
    d_ptr->m_metaObj = metaObj;
    d_ptr->m_roles.insert (0, "qtObject");
    for (int i = 0; i < d_ptr->m_metaObj.propertyCount (); i++) {
        int role = i+1;
        QMetaProperty metaProp = d_ptr->m_metaObj.property (i);
        d_ptr->m_roles.insert (role, metaProp.name ());
        if (metaProp.hasNotifySignal ()) {
            d_ptr->m_signalIdxToRole.insert (metaProp.notifySignalIndex (), role);
        }
    }
    for (int j = 0; j < d_ptr->metaObject ()->methodCount (); j++) {
        QMetaMethod metaMethod = d_ptr->metaObject ()->method (j);
        if (metaMethod.name () == "onItemPropertyChanged") {
            d_ptr->m_handler = metaMethod;
            break;
        }
    }
}

int QQmlObjectListModel::rowCount (const QModelIndex & parent) const
{
    Q_UNUSED (parent);
    return d_ptr->m_items.count ();
}

QVariant QQmlObjectListModel::data (const QModelIndex & index, int role) const
{
    QVariant ret;
    QObject * item = get (index.row ());
    if (item != NULL) {
        ret.setValue (role > 0 ? item->property (d_ptr->m_roles.value (role)) : QVariant::fromValue (item));
    }
    return ret;
}

QHash<int, QByteArray> QQmlObjectListModel::roleNames () const
{
    return d_ptr->m_roles;
}

bool QQmlObjectListModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
    bool ret = false;
    QObject * item = get (index.row ());
    if (item != NULL && role > 0) {
        ret = item->setProperty (d_ptr->m_roles.value (role), value);
    }
    return ret;
}

int QQmlObjectListModel::roleForName (QByteArray name) const
{
    return d_ptr->m_roles.key (name, -1);
}

int QQmlObjectListModel::count () const
{
    return d_ptr->m_items.size ();
}

void QQmlObjectListModel::clear () {
    beginResetModel ();
    foreach (QObject * item, d_ptr->m_items)
    {
        if (item != NULL) {
            item->deleteLater ();
        }
    }
    d_ptr->m_items.clear ();
    endResetModel ();
}

void QQmlObjectListModel::append (QObject * item)
{
    if (item != NULL) {
        beginInsertRows (QModelIndex (), d_ptr->m_items.count (), d_ptr->m_items.count ());
        d_ptr->m_items.append (item);
        d_ptr->prepareItem (item);
        endInsertRows ();
    }
}

void QQmlObjectListModel::prepend (QObject * item)
{
    if (item != NULL) {
        beginInsertRows (QModelIndex (), 0, 0);
        d_ptr->m_items.prepend (item);
        d_ptr->prepareItem (item);
        endInsertRows ();
    }
}

void QQmlObjectListModel::insert (int idx, QObject * item)
{
    if (item != NULL) {
        beginInsertRows (QModelIndex (), idx, idx);
        d_ptr->m_items.insert (idx, item);
        d_ptr->prepareItem (item);
        endInsertRows ();
    }
}

void QQmlObjectListModel::remove (QObject * item)
{
    if (item != NULL) {
        int idx = d_ptr->m_items.indexOf (item);
        remove (idx);
    }
}

void QQmlObjectListModel::remove (int idx)
{
    if (idx >= 0 && idx < d_ptr->m_items.size ()) {
        beginRemoveRows (QModelIndex (), idx, idx);
        QObject * item = d_ptr->m_items.takeAt (idx);
        if (item != NULL) {
            item->disconnect ();
            item->deleteLater ();
        }
        endRemoveRows ();
    }
}

QObject * QQmlObjectListModel::get (int idx) const
{
    QObject * ret = NULL;
    if (idx >= 0 && idx < d_ptr->m_items.size ()) {
        ret = d_ptr->m_items.value (idx);
    }
    return ret;
}

QQmlObjectListModelPrivate::QQmlObjectListModelPrivate (QQmlObjectListModel * parent)
    : QObject (parent)
    , d_func (parent)
{

}

void QQmlObjectListModelPrivate::onItemPropertyChanged ()
{
    int row = m_items.indexOf (sender ());
    int role = m_signalIdxToRole.key (senderSignalIndex ());
    if (row >= 0 && role >= 0) {
        QModelIndex idx = d_func->index (row, 0, QModelIndex ());
        emit d_func->dataChanged (idx, idx, QVector<int> () << role);
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
