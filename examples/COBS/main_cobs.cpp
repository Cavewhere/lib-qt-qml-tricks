
#include <QCoreApplication>
#include <QDebug>

#include <QtCOBS>

int main (int argc, char * argv []) {
    QCoreApplication (argc, argv);

    const char _raw1 [1] = {0x00};
    const QByteArray raw1 = QByteArray::fromRawData (_raw1, 1);
    const char _cob1 [2] = {0x01, 0x01};
    const QByteArray cob1 = QByteArray::fromRawData (_cob1, 2);
    qDebug () << "TST1 :"
              << "RAW=" << raw1.toHex ()
              << "COBS=" << cob1.toHex ()
              << "ENCODE=" << (QtCOBS::encode (raw1) == cob1)
              << "DECODE=" << (QtCOBS::decode (cob1) == raw1);

    const char _raw2 [4] = {0x11, 0x22, 0x00, 0x33};
    const QByteArray raw2 = QByteArray::fromRawData (_raw2, 4);
    const char _cob2 [5] = {0x03, 0x11, 0x22, 0x02, 0x33};
    const QByteArray cob2 = QByteArray::fromRawData (_cob2, 5);
    qDebug () << "TST2 :"
              << "RAW=" << raw2.toHex ()
              << "COBS=" << cob2.toHex ()
              << "ENCODE=" << (QtCOBS::encode (raw2) == cob2)
              << "DECODE=" << (QtCOBS::decode (cob2) == raw2);

    const char _raw3 [4] = {0x11, 0x00, 0x00, 0x00};
    const QByteArray raw3 = QByteArray::fromRawData (_raw3, 4);
    const char _cob3 [5] = {0x02, 0x11, 0x01, 0x01, 0x01};
    const QByteArray cob3 = QByteArray::fromRawData (_cob3, 5);
    qDebug () << "TST3 :"
              << "RAW=" << raw3.toHex ()
              << "COBS=" << cob3.toHex ()
              << "ENCODE=" << (QtCOBS::encode (raw3) == cob3)
              << "DECODE=" << (QtCOBS::decode (cob3) == raw3);

    return 0;
}
