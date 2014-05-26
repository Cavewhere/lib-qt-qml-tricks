#ifndef QQMLVARIANTLISTMODEL_H
#define QQMLVARIANTLISTMODEL_H

#include "qqmlmodels.h"

class QQmlVariantListModelPrivate;

class QQmlVariantListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY (int count READ count NOTIFY countChanged)

public:
    explicit QQmlVariantListModel (QObject * parent);
    virtual ~QQmlVariantListModel ();

public: // QAbstractItemModel interface reimplemented
    virtual int rowCount (const QModelIndex & parent = QModelIndex ()) const;
    virtual bool setData (const QModelIndex & index, const QVariant & value, int role);
    virtual QVariant data (const QModelIndex & index, int role) const;
    virtual QHash<int, QByteArray> roleNames () const;

public slots: // public API
    void clear ();
    int count () const;
    bool isEmpty () const;
    void append (QVariant item);
    void prepend (QVariant item);
    void insert (int idx, QVariant item);
    void appendList (QVariantList itemList);
    void prependList (QVariantList itemList);
    void replace (int pos, QVariant item);
    void insertList (int idx, QVariantList itemList);
    void move (int idx, int pos);
    void remove (int idx);
    QVariant get (int idx) const;
    QVariantList list () const;

signals: // notifiers
    void countChanged (int count);

private:
    QQmlVariantListModelPrivate * m_privateImpl;
};

#endif // QQMLVARIANTLISTMODEL_H
