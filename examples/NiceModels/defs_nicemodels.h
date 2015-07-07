#ifndef DEFS_H
#define DEFS_H

#include <QObject>
#include <QString>

#include <QQmlHelpers>

QML_ENUM_CLASS (MyEnum, Unknown = 0, First, Second, Third, Fourth, Fifth, Sixth, Seventh)

class MyItem : public QObject {
    Q_OBJECT
    QML_WRITABLE_PROPERTY (int,          foo)
    QML_WRITABLE_PROPERTY (QString,      bar)
    QML_READONLY_PROPERTY (MyEnum::Type, test)
    QML_CONSTANT_PROPERTY (QString,      type)
    QML_CONSTANT_PROPERTY (bool,         model)

public:
    explicit MyItem (QObject * parent = NULL) : QObject (parent) {
        m_foo  = 0;
        m_bar  = "";
        m_test = MyEnum::Unknown;
        m_type = metaObject ()->className ();
    }
};

#endif // DEFS_H
