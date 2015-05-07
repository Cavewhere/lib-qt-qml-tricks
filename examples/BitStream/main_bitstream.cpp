
#include <QCoreApplication>
#include <QDebug>

#include <QtBitStream>

int main (int argc, char * argv []) {
    Q_UNUSED (argc)
    Q_UNUSED (argv)

    const unsigned char data [] = { 0x35, 0x02 , 0x26, 0x7F, 0x65, 0x4F, 0xAB, 0x03, 0x85, 0x7B };
    const unsigned int  size = (sizeof (data) / sizeof (data [0]));

    QtBitStream streamOut (QByteArray::fromRawData ((const char *) data, size));

    qDebug () << "streamOut=" << streamOut.toBinary ();

    bool ok = false;

    for (int bit = 0; bit < 8; bit++) {
        qDebug () << "bit as bool :" << streamOut.getPosition () << streamOut.readBits<bool> (1, &ok);
    }
    qDebug () << "bits 8-24 in quint16 :" << streamOut.getPosition () << streamOut.readBits<quint16> (16);

    QtBitStream streamIn (QByteArray (4, 0x00));
    quint8 test1 = 0x7;
    streamIn.writeBits<quint8> (test1, 3, &ok);
    qDebug () << "streamIn after test1=" << streamIn.toBinary () << ok;
    qreal test2 = 0x7;
    streamIn.writeBits<qreal> (test2, 5, &ok);
    qDebug () << "streamIn after test2=" << streamIn.toBinary () << ok;
    QByteArray test3 = "toto";
    streamIn.writeBits<QByteArray> (test3, 23, &ok);
    qDebug () << "streamIn after test3=" << streamIn.toBinary () << ok;
    streamIn.writeBits<quint16> (5651, 12, &ok);
    qDebug () << "streamIn after test3=" << streamIn.toBinary () << ok;

    return 0;
}

