#ifndef QTBITSTREAM_H
#define QTBITSTREAM_H

#include <QByteArray>
#include <QtGlobal>
#include <QDebug>

// TODO : handle little/big endian

// TODO : handle sign bit moves

// NOTE : using template partial specialization + SFINAE to ensure compile-time type checking and no build error

typedef unsigned int  UInt;
typedef unsigned char Byte;

template<typename T> class CheckInt {
public:
    static const bool isInt = false;
    static bool testBit (T, UInt) { return false; }
};

#define CheckInt_SPECIALIZE(TYPE) \
    template<> class CheckInt<TYPE> { \
    public: static const bool isInt = true; \
    public: static bool testBit (TYPE src, UInt bit) { return ((src >> bit) & 0x1); } }

CheckInt_SPECIALIZE (bool);
CheckInt_SPECIALIZE (qint8);
CheckInt_SPECIALIZE (qint16);
CheckInt_SPECIALIZE (qint32);
CheckInt_SPECIALIZE (qint64);
CheckInt_SPECIALIZE (quint8);
CheckInt_SPECIALIZE (quint16);
CheckInt_SPECIALIZE (quint32);
CheckInt_SPECIALIZE (quint64);

class QtBitStream {
public:
    explicit QtBitStream (QByteArray data)
        : m_len  (data.size () * 8)
        , m_pos  (0)
        , m_data (data) {
        if (!m_len) {
            qWarning () << "QtBitStream with an empty data buffer is useless, nothing can be done on it !";
        }
    }

    QByteArray getData (void) const {
        return m_data;
    }

    UInt getPosition (void) const {
        return m_pos;
    }

    QString toBinary (bool blocks = true) const {
        QString ret;
        for (UInt bit = 0; bit < m_len; bit++) {
            if (blocks && bit % 8 == 0) {
                ret.append (' ');
            }
            ret.append (testBit (bit) ? '1' : '0');
        }
        return ret;
    }

    bool testBit (UInt bit) const {
        Byte byte (m_data [bit / 8]);
        return ((byte >> (bit % 8)) & 0x1);
    }

    void setPosition (UInt pos) {
        m_pos = pos;
    }

    void setBit (UInt bit, bool value) {
        Byte byte = m_data [bit / 8];
        if (value) {
            byte |= (1 << (bit % 8));
        }
        else {
            byte &= ~(1 << (bit % 8));
        }
        m_data [bit / 8] = byte;
    }

    template<typename IntegerType> IntegerType readBits (UInt count, bool * ok = NULL) {
        IntegerType ret = 0;
        bool result = false;
        if (CheckInt<IntegerType>::isInt) {
            if (m_pos + count < m_len && count <= (sizeof (ret) * 8)) {
                for (UInt bit = 0; bit < count; bit++, m_pos++) {
                    if (testBit (m_pos)) {
                        ret |= (IntegerType)(1 << bit);
                    }
                    else {
                        ret &= (IntegerType)~(1 << bit);
                    }
                }
                result = true;
            }
        }
        else {
            qWarning () << "QtBitStream::readBits MUST be used with bool / int8 / int16 / int32 / int64 types only !";
        }
        if (ok != NULL) {
            *ok = result;
        }
        return ret;
    }

    template<typename IntegerType> void writeBits (IntegerType src, UInt count, bool * ok = NULL) {
        bool result = false;
        if (CheckInt<IntegerType>::isInt) {
            if (m_pos + count < m_len && count <= (sizeof (src) * 8)) {
                for (UInt bit = 0; bit < count; bit++, m_pos++) {
                    setBit (m_pos, CheckInt<IntegerType>::testBit (src, bit));
                }
                result = true;
            }
        }
        else {
            qWarning () << "QtBitStream::writeBits MUST be used with bool / int8 / int16 / int32 / int64 types only !";
        }
        if (ok != NULL) {
            *ok = result;
        }
    }

private:
    UInt m_len, m_pos;
    QByteArray m_data;
};

#endif // QTBITSTREAM_H
