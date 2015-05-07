
#include <QCoreApplication>
#include <QDebug>

#include <QtJsonPath>

int main (int argc, char * argv []) {
    Q_UNUSED (argc)
    Q_UNUSED (argv)

    QByteArray json ("{ \"store\": {"
                     "    \"book\": [ "
                     "      { \"category\": \"reference\","
                     "        \"author\": \"Nigel Rees\","
                     "        \"title\": \"Sayings of the Century\","
                     "        \"price\": 8.95"
                     "      },"
                     "      { \"category\": \"fiction\","
                     "        \"author\": \"Evelyn Waugh\","
                     "        \"title\": \"Sword of Honour\","
                     "        \"price\": 12.99"
                     "      },"
                     "      { \"category\": \"fiction\","
                     "        \"author\": \"Herman Melville\","
                     "        \"title\": \"Moby Dick\","
                     "        \"isbn\": \"0-553-21311-3\","
                     "        \"price\": 8.99"
                     "      },"
                     "      { \"category\": \"fiction\","
                     "        \"author\": \"J. R. R. Tolkien\","
                     "        \"title\": \"The Lord of the Rings\","
                     "        \"isbn\": \"0-395-19395-8\","
                     "        \"price\": 22.99"
                     "      }"
                     "    ],"
                     "    \"bicycle\": {"
                     "      \"color\": \"red\","
                     "      \"price\": 19.95"
                     "    }"
                     "  }"
                     "}");

    QJsonDocument jsonDoc = QJsonDocument::fromJson (json);

    QtJsonPath jsonPath (jsonDoc);

    qDebug () << "/store/book/1"        << jsonPath.getValue ("/store/book/1");
    qDebug () << "/store/book/2/author" << jsonPath.getValue ("/store/book/2/author");
    qDebug () << "/store/bicycle"       << jsonPath.getValue ("/store/bicycle");
    qDebug () << "/store/toto"          << jsonPath.getValue ("/store/toto", "N/A");

    return 0;
}

