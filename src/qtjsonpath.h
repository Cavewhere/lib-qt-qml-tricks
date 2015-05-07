#ifndef QTJSONPATH
#define QTJSONPATH

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringList>
#include <QString>
#include <QVariant>

class QtJsonPath {
public:
    explicit QtJsonPath (QJsonValue & jsonVal) {
        QJsonObject jsonObj = jsonVal.toObject ();
        if (!jsonObj.isEmpty ()) {
            initWithNode (jsonObj);
        }
        else {
            QJsonArray jsonArray = jsonVal.toArray ();
            if (!jsonArray.isEmpty ()) {
                initWithNode (jsonArray);
            }
            else { }
        }
    }

    explicit QtJsonPath (QJsonObject & jsonObj) {
        initWithNode (jsonObj);
    }

    explicit QtJsonPath (QJsonArray & jsonArray) {
        initWithNode (jsonArray);
    }

    explicit QtJsonPath (QJsonDocument & jsonDoc) {
        QJsonObject jsonObj = jsonDoc.object ();
        if (!jsonObj.isEmpty ()) {
            initWithNode (jsonObj);
        }
        else {
            QJsonArray jsonArray = jsonDoc.array ();
            if (!jsonArray.isEmpty ()) {
                initWithNode (jsonArray);
            }
            else { }
        }
    }

    QVariant getValue (QString path, QVariant fallback = QVariant ()) const {
        QVariant ret;
        QStringList list = path.split ('/', QString::SkipEmptyParts);
        if (!list.empty () && !m_rootNode.isUndefined () && !m_rootNode.isNull ()) {
            QJsonValue currNode = m_rootNode;
            bool error = false;
            const unsigned int len = list.size ();
            for (unsigned int depth = 0; (depth < len) && (!error); depth++) {
                QString part = list.at (depth);
                bool isNum = false;
                int index = part.toInt (&isNum, 10);
                if (isNum) { // NOTE : array subpath
                    if (currNode.isArray ()) {
                        QJsonArray arrayNode = currNode.toArray ();
                        if (index < arrayNode.size ()) {
                            currNode = arrayNode.at (index);
                        }
                        else { error = true; }
                    }
                    else { error = true; }
                }
                else { // NOTE : object subpath
                    if (currNode.isObject ()) {
                        QJsonObject objNode = currNode.toObject ();
                        if (objNode.contains (part)) {
                            currNode = objNode.value (part);
                        }
                        else { error = true; }
                    }
                    else { error = true; }
                }
            }
            ret = (!error ? currNode.toVariant () : fallback);
        }
        return ret;
    }

protected:
    void initWithNode (QJsonValue jsonNode) {
        m_rootNode = jsonNode;
    }

private:
    QJsonValue m_rootNode;
};

#endif // QTJSONPATH

