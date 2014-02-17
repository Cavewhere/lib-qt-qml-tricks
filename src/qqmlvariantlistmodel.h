#ifndef QQMLVARIANTLISTMODEL_H
#define QQMLVARIANTLISTMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class QQmlVariantListModelPrivate;

class QQmlVariantListModel : public QAbstractListModel {
    Q_OBJECT

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
    void append (QVariantList itemList);
    void prepend (QVariantList itemList);
    void insert (int idx, QVariantList itemList);
    void move (int idx, int pos);
    void remove (int idx);
    QVariant get (int idx) const;
    QVariantList list () const;

private:
    QQmlVariantListModelPrivate * m_privateImpl;
};

#endif // QQMLVARIANTLISTMODEL_H
