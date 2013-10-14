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
                                      QObject *parent);
    virtual void execute();
    virtual void processPacket(const QByteArray &data);
protected:
    QByteArray rawData;
    const qint16 resultPresentationQuantum=0;
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
    const qint32 memoryBlockAddress;
    const qint16 memoryBlockLength;
    const qint16 memoryBlockReadQuantum=32;
    static const QByteArray acknowledgementString;
    QByteArray outBuffer;
    QByteArray inBuffer;
    QTimer *outDataSendTimer;
    int outBufferPosition=0;
    virtual void rawDataReady()=0;
private slots:
    void onOutDataSendTimerShot();
signals:

public slots:

};

#endif // N3READMEMORYBLOCKCOMMAND_H
