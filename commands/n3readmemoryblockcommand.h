#ifndef N3READMEMORYBLOCKCOMMAND_H
#define N3READMEMORYBLOCKCOMMAND_H
#include "n3readerbasecommand.h"

class QTimer;
class N3ReadMemoryBlockCommand : public N3ReaderBaseCommand
{
    Q_OBJECT
public:
    explicit N3ReadMemoryBlockCommand(quint32 address,
                                      qint16 length,
                                      qint16 resultLength,
                                      QObject *parent);
    virtual void execute();
    virtual void processPacket(const QByteArray &data);
    virtual const QString &commandName() const;
    virtual const QString &description() const;
protected:
    QByteArray rawData;
    const qint16 resultPresentationQuantum;
    void setMemoryBlockAddress(const quint32 &address);
    void setMemoryBlockLength(const quint32 &length);
    quint32 getMemoryBlockAddress() const;
    quint32 getMemoryBlockLength() const;
private:
    enum class CommandState {
        initial,
        awaitingAcknowledgementStart,
        receivingAcknowledgement,
        badAcknowledgement,
        receivingData
    };
    CommandState m_state = CommandState::initial;
    quint16 bytesReceived=0;
    static const int msByteSendingDelay=10;
    qint32 memoryBlockAddress;
    qint16 memoryBlockLength;
    const qint16 memoryBlockReadQuantum=32;
    static const QByteArray acknowledgementString;
    QByteArray outBuffer;
    QByteArray inBuffer;
    QTimer *outDataSendTimer;
    int outBufferPosition=0;
    virtual void rawDataReady();
private slots:
    void onOutDataSendTimerShot();
signals:
    void rawDataAvailable(QByteArray data);
public slots:

};

#endif // N3READMEMORYBLOCKCOMMAND_H
