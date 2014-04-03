#include "qqmlobjectlistmodel.h"
#include "qqmlobjectlistmodel_p.h"

/*!
    \class QQmlObjectListModel

    \ingroup QT_QML_MODELS

    \brief Provides a generic way to generate a list model from QObject derived class, suitable for QML

    QQmlObjectListModel is a convenience subclass \c QAbstractListModel that makes use of C++ templates
    and Qt Meta Object to extract properties from a \c QObject derived class and create according roles
    inside the model.

    This is a far better way than to expose directly a \c QList<QObject *> inside a \c QVariant.

    And this is far simpler than doing all Qt model stuff manually : no subclassing or reimplementing need.

    The class was designed so that most of the added API is really common with \c QList one.

    \b Note : Simply needs that the class used for items inherits \c QObject and has Qt Meta Properties.

    \sa QQmlVariantListModel
*/


/*!
    \fn static QQmlObjectListModel * QQmlObjectListModel::create (QObject * parent = NULL)

    \details A factory to create a new model from a class that will be used as item type.

    \tparam ItemType The class to use as item
    \param parent The owner object for the model
    \return The newly created and configurerd model

    This is a template method, meant to be used like this :
    \code
        QQmlObjectListModel * myModel = QQmlObjectListModel::create<MyItemClass>(this);
    \endcode

    No other customization in needed after that.
*/


/*!
    \fn ItemType * QQmlObjectListModel::getAs () const

    \details A template method to retreive a given item as a precise \c T* Qt object pointer.

    \tparam ItemType The class to use as return pointer type
    \param idx The position of the item in the model
    \return The typed pointer to the object, or \c NULL if the type doesn't match

    \sa get(int) const
*/


/*!
    \fn QList<ItemType *> QQmlObjectListModel::listAs () const

    \details A template method to retreive all the items as \c QList<T*> typed Qt object pointer list.

    \tparam ItemType The class as object type to use in the returned pointer list
    \return A strongly typed \c QList of items Qt object pointers

    \sa list() const
*/


/*!
    \internal
*/
QQmlObjectListModel::QQmlObjectListModel (QMetaObject metaObj, QObject * parent)
    : QAbstractListModel (parent)
    , m_privateImpl (new QQmlObjectListModelPrivate (this))
{
    m_privateImpl->m_metaObj = metaObj;
    m_privateImpl->m_roles.insert (0, "qtObject");
    for (int propertyIdx = 0; propertyIdx < m_privateImpl->m_metaObj.propertyCount (); propertyIdx++) {
        int role = m_privateImpl->m_roles.count ();
        QMetaProperty metaProp = m_privateImpl->m_metaObj.property (propertyIdx);
        m_privateImpl->m_roles.insert (role, metaProp.name ());
        if (metaProp.hasNotifySignal ()) {
            m_privateImpl->m_signalIdxToRole.insert (metaProp.notifySignalIndex (), role);
        }
    }
//    qWarning () << "roles=" << m_privateImpl->m_roles;
//    qWarning () << "notifiers=" << m_privateImpl->m_signalIdxToRole;
//    foreach (int sigIdx, m_privateImpl->m_signalIdxToRole.keys ()) {
//        int role = m_privateImpl->m_signalIdxToRole.value (sigIdx);
//        qWarning () << "sigIdx=" << sigIdx << m_privateImpl->m_metaObj.method (sigIdx).name () << "role=" << role << m_privateImpl->m_roles.value (role);
//    }
}

/*!
    \internal
*/
QQmlObjectListModel::~QQmlObjectListModel ()
{
    clear ();
}

/*!
    \internal
*/
int QQmlObjectListModel::rowCount (const QModelIndex & parent) const
{
    Q_UNUSED (parent);
    return m_privateImpl->m_items.count ();
}

/*!
    \details Returns the data in a specific index for a given role.

    Reimplemented for QAbstractItemModel.

    \param index The item index (row, column and parent)
    \param role The role for property
    \return The data in the role

    \b Note : the \c 0 role is a pointer to item object itself.
*/
QVariant QQmlObjectListModel::data (const QModelIndex & index, int role) const
{
    QVariant ret;
    QObject * item = get (index.row ());
    if (item != NULL) {
        ret.setValue (role > 0 ? item->property (m_privateImpl->m_roles.value (role)) : QVariant::fromValue (item));
    }
    return ret;
}

/*!
    \details Returns the roles available in the model.

    Reimplemented for QAbstractItemModel.

    \return The hash table of role to name matching

    \b Note : an additional \c 'qtObject' role is added for convenience.
*/
QHash<int, QByteArray> QQmlObjectListModel::roleNames () const
{
    return m_privateImpl->m_roles;
}

/*!
    \details Modifies the data in a specific index for a given role.

    Reimplemented for QAbstractItemModel.

    \param index The item index (row, column and parent)
    \param value The data to write
    \param role The role for property
    \return Weither the modification was done
*/
bool QQmlObjectListModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
    bool ret = false;
    QObject * item = get (index.row ());
    if (item != NULL && role > 0) {
        ret = item->setProperty (m_privateImpl->m_roles.value (role), value);
    }
    return ret;
}

/*!
    \details Returns the role associated to the given property name.

    \param name The property name inside the item class
    \return The matching role, \c -1 if not found
*/
int QQmlObjectListModel::roleForName (QByteArray name) const
{
    return m_privateImpl->m_roles.key (name, -1);
}

/*!
    \details Counts the items in the model.

    \return The count of items in the model
*/
int QQmlObjectListModel::count () const
{
    return m_privateImpl->m_items.size ();
}

/*!
    \details Tests the content of the model.

    \return Whether the model contains no item
*/
bool QQmlObjectListModel::isEmpty () const
{
    return m_privateImpl->m_items.isEmpty ();
}

/*!
    \details Tests the presence of a given item in the model.

    \param item The pointer to the item
    \return Whether the item was found
*/
bool QQmlObjectListModel::contains (QObject * item) const
{
    return m_privateImpl->m_items.contains (item);
}

/*!
    \details Finds the position of given item in the model.

    \param item The pointer to the item
    \return The row index of the item, \c -1 if not found
*/
int QQmlObjectListModel::indexOf (QObject * item) const
{
    return m_privateImpl->m_items.indexOf (item);
}

/*!
    \details Delete all the items in the model.

    \b Note : The items objects will be removed from the model but they will be destructed
    only if they have no parent (because the model took the ownership).
*/
void QQmlObjectListModel::clear ()
{
    beginResetModel ();
    foreach (QObject * item, m_privateImpl->m_items) {
        m_privateImpl->dereferenceItem (item);
    }
    m_privateImpl->m_items.clear ();
    endResetModel ();
}

/*!
    \details Adds the given item at the end of the model.

    \param item The pointer to the item

    \sa prepend(QObject*), insert(int,QObject*)
*/
void QQmlObjectListModel::append (QObject * item)
{
    if (item != NULL) {
        int pos = m_privateImpl->m_items.count ();
        beginInsertRows (NO_PARENT, pos, pos);
        m_privateImpl->m_items.append (item);
        m_privateImpl->referenceItem (item);
        endInsertRows ();
    }
}

/*!
    \details Adds the given item at the beginning of the model.

    \param item The pointer to the item

    \sa append(QObject*), insert(int,QObject*)
*/
void QQmlObjectListModel::prepend (QObject * item)
{
    if (item != NULL) {
        beginInsertRows (NO_PARENT, 0, 0);
        m_privateImpl->m_items.prepend (item);
        m_privateImpl->referenceItem (item);
        endInsertRows ();
    }
}

/*!
    \details Adds the given item at a certain position in the model.

    \param idx The position where the item must be added
    \param item The pointer to the item

    \sa append(QObject*), prepend(QObject*)
*/
void QQmlObjectListModel::insert (int idx, QObject * item)
{
    if (item != NULL) {
        beginInsertRows (NO_PARENT, idx, idx);
        m_privateImpl->m_items.insert (idx, item);
        m_privateImpl->referenceItem (item);
        endInsertRows ();
    }
}

/*!
    \details Adds the given list of items at the end of the model.

    \param itemList The list of items

    \sa prepend(QObjectList), insert(int, QObjectList)
*/
void QQmlObjectListModel::append (QObjectList itemList)
{
    itemList.removeAll (NULL);
    if (!itemList.isEmpty ()) {
        int pos = m_privateImpl->m_items.count ();
        beginInsertRows (NO_PARENT, pos, pos + itemList.count ());
        m_privateImpl->m_items.append (itemList);
        foreach (QObject * item, itemList) {
            m_privateImpl->referenceItem (item);
        }
        endInsertRows ();
    }
}

/*!
    \details Adds the given list of items at the beginning of the model.

    \param itemList The list of items

    \sa append(QObjectList), insert(int, QObjectList)
*/
void QQmlObjectListModel::prepend (QObjectList itemList)
{
    itemList.removeAll (NULL);
    if (!itemList.isEmpty ()) {
        beginInsertRows (NO_PARENT, 0, itemList.count ());
        int offset = 0;
        foreach (QObject * item, itemList) {
            m_privateImpl->m_items.insert (offset, item);
            m_privateImpl->referenceItem (item);
        }
        endInsertRows ();
    }
}

/*!
    \details Adds the given list of items at a certain position in the model.

    \param idx The position where the items must be added
    \param itemList The list of items

    \sa append(QObjectList), prepend(QObjectList)
*/
void QQmlObjectListModel::insert (int idx, QObjectList itemList)
{
    itemList.removeAll (NULL);
    if (!itemList.isEmpty ()) {
        beginInsertRows (NO_PARENT, idx, idx + itemList.count ());
        int offset = 0;
        foreach (QObject * item, itemList) {
            m_privateImpl->m_items.insert (idx + offset, item);
            m_privateImpl->referenceItem (item);
            offset++;
        }
        endInsertRows ();
    }
}

/*!
    \details Moves an item from the model to another position.

    \param idx The current position of the item
    \param pos The position where it willl be after the move
*/
void QQmlObjectListModel::move (int idx, int pos)
{
    beginMoveRows (NO_PARENT, idx, idx, NO_PARENT, pos);
    m_privateImpl->m_items.move (idx, pos);
    endMoveRows ();
}

/*!
    \details Remove an item from the model.

    \param item The pointer to the item object
*/
void QQmlObjectListModel::remove (QObject * item)
{
    if (item != NULL) {
        int idx = m_privateImpl->m_items.indexOf (item);
        remove (idx);
    }
}

/*!
    \details Remove an item from the model.

    \param idx The position of the item in the model
*/
void QQmlObjectListModel::remove (int idx)
{
    if (idx >= 0 && idx < m_privateImpl->m_items.size ()) {
        beginRemoveRows (NO_PARENT, idx, idx);
        QObject * item = m_privateImpl->m_items.takeAt (idx);
        m_privateImpl->dereferenceItem (item);
        endRemoveRows ();
    }
}

/*!
    \details Retreives a model item as standard Qt object pointer.

    \param idx The position of the item in the model
    \return A pointer to the \c QObject

    \sa getAs(int) const
*/
QObject * QQmlObjectListModel::get (int idx) const
{
    QObject * ret = NULL;
    if (idx >= 0 && idx < m_privateImpl->m_items.size ()) {
        ret = m_privateImpl->m_items.value (idx);
    }
    return ret;
}

QObject * QQmlObjectListModel::first () const
{
    return m_privateImpl->m_items.first ();
}

QObject * QQmlObjectListModel::last () const
{
    return m_privateImpl->m_items.last ();
}

/*!
    \details Retreives all the items of the model as a standard Qt object pointer list.

    \return A \c QObjectList containing all the pointers

    \sa listAs() const
*/
QObjectList QQmlObjectListModel::list () const
{
    return m_privateImpl->m_items;
}

/*!
    \internal
*/
QQmlObjectListModelPrivate::QQmlObjectListModelPrivate (QQmlObjectListModel * parent)
    : QObject (parent)
    , m_publicObject (parent)
{
    m_handler = metaObject ()->method (metaObject ()->indexOfMethod ("onItemPropertyChanged()"));
}

/*!
    \internal
*/
void QQmlObjectListModelPrivate::onItemPropertyChanged ()
{
    int row = m_items.indexOf (sender ());
    int sig = senderSignalIndex ();
    int role = m_signalIdxToRole.value (sig);
    if (row >= 0 && role >= 0) {
        QModelIndex index = m_publicObject->index (row, 0, NO_PARENT);
        QVector<int> vec;
        vec << role;
        //qWarning () << "onItemPropertyChanged" << "row=" << row << "sig=" << sig << m_metaObj.method (sig).name () << "vec=" << vec << m_roles.value (role);
        emit m_publicObject->dataChanged (index, index, vec);
    }
}

/*!
    \internal
*/
void QQmlObjectListModelPrivate::referenceItem (QObject * item)
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

/*!
    \internal
*/
void QQmlObjectListModelPrivate::dereferenceItem (QObject * item)
{
    if (item != NULL) {
        item->disconnect ();
        if (item->parent () == this) { // FIXME : maybe that's not the best way to test ownership ?
            item->deleteLater ();
        }
    }
}
