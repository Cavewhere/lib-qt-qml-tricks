
#include <QObject>
#include <QList>
#include <QAbstractListModel>

#define NO_PARENT QModelIndex ()
#define BASE_ROLE Qt::UserRole
#define EMPTY_STR QStringLiteral ("")
#define EMPTY_BA  QByteArrayLiteral ("")

/*!
    \defgroup QT_QML_MODELS Qt models for QML

    Brings a bunch of nice, ready-to-use, C++ models for usual use-cases in C++/QML apps :
    \li Exposing a list of QObject-derived objects to QML while supporting dynamic changes
    \li Using a simple list of QVariant / QVariantMap just as easy as the plain QML ListModel item
    \li etc...
*/
