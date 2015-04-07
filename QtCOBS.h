#ifndef QTCOBS_H
#define QTCOBS_H

#include <QByteArray>

class QtCOBS {
public:
    static QByteArray encode (const QByteArray & rawData) {
        QByteArray encodedData;
        quint8  code = 0x01;
        int readIndex  = 0;
        int writeIndex = 1;
        int codeIndex  = 0;
        const int len = rawData.size ();
        while (readIndex < len) {
            if (rawData [readIndex] == 0x00) {
                encodedData [codeIndex] = code;
                code = 0x01;
                codeIndex = writeIndex++;
                readIndex++;
            }
            else {
                encodedData [writeIndex++] = rawData [readIndex++];
                code++;
                if (code == 0xFF) {
                    encodedData [codeIndex] = code;
                    code = 0x01;
                    codeIndex = writeIndex++;
                }
            }
        }
        encodedData [codeIndex] = code;
        return encodedData;
    }
    static QByteArray decode (const QByteArray & cobsData) {
        QByteArray decodedData;
        quint8 code;
        int readIndex  = 0;
        int writeIndex = 0;
        const int len = cobsData.size ();
        while (readIndex < len) {
            code = cobsData [readIndex];
            if (readIndex + code > len && code != 0x01) {
                decodedData.clear ();
                break;
            }
            readIndex++;
            for (quint8 pos = 1; pos < code; pos++) {
                decodedData [writeIndex++] = cobsData [readIndex++];
            }
            if (code != 0xFF && readIndex != len) {
                decodedData [writeIndex++] = 0x00;
            }
        }
        return decodedData;
    }
};

#endif // QTCOBS_H
