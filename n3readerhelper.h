#ifndef N3READERHELPER_H
#define N3READERHELPER_H

#include <QObject>
#include <QSharedPointer>
class N3ReaderBaseCommand;

class N3ReaderHelper : public QObject
{
    Q_OBJECT
public:
    explicit N3ReaderHelper(QObject *parent = 0);
    enum class ReaderState {
        ready,
        processing,
        closed
    };
    static uint32_t bytesToUInt(const uchar &byte0,
                                const uchar &byte1,
                                const uchar &byte2,
                                const uchar &byte3);
    static QByteArray UIntToBytes(const quint32 &dword);
    static uchar calculateChecksum(const QByteArray &packet);
    void encrypt(uint32_t &U, uint32_t &U1) const;
    void decrypt(uint32_t &U, uint32_t &U1) const;
    QByteArray encryptPacket(const QByteArray &packet) const;
    QByteArray decryptPacket(const QByteArray &packet) const;
    void executeCommand(N3ReaderBaseCommand *command);
    void setKey(const uint32_t key[]);
private:
    const size_t N3_ENCODED_PACKET_SIZE = 32;
    N3ReaderBaseCommand *m_command;
    uint32_t m_key[4];
    ReaderState m_state=ReaderState::closed;
    void setDeviceState(const ReaderState &state);
private slots:
    void commandFinished();
    void sendData(const QByteArray &data);
signals:
    void dataToSend(const QByteArray &data);
    void stateChanged(const ReaderState &state);
public slots:
    void processData(const QByteArray &data);
    void processData();
};
#endif // N3READERHELPER_H
