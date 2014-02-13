#ifndef QQMLOBJECTLISTMODEL_H
#define QQMLOBJECTLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

class QQmlObjectListModelPrivate;

class QQmlObjectListModel : public QAbstractListModel {
    Q_OBJECT

public: // public factory
    template <class ItemType> static QQmlObjectListModel * create (QObject * parent = NULL) {
        return new QQmlObjectListModel (ItemType::staticMetaObject, parent);
    }

protected: // protected constructor
    explicit QQmlObjectListModel (QMetaObject metaObj, QObject * parent);

public: // QAbstractItemModel interface reimplemented
    virtual int rowCount (const QModelIndex & parent) const;
    virtual bool setData (const QModelIndex & index, const QVariant & value, int role);
    virtual QVariant data (const QModelIndex & index, int role) const;
    virtual QHash<int, QByteArray> roleNames () const;

public slots: // public API
    void clear ();
    int count () const;
    int roleForName (QByteArray name) const;
    void append (QObject * item);
    void prepend (QObject * item);
    void insert (int idx, QObject * item);
    void remove (int idx);
    void remove (QObject * item);
    QObject * get (int idx) const;

private:
    QQmlObjectListModelPrivate * m_privateImpl;
};

#endif // QQMLOBJECTLISTMODEL_H
