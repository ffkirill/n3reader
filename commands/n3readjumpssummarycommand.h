#ifndef N3READJUMPSSUMMARYCOMMAND_H
#define N3READJUMPSSUMMARYCOMMAND_H
#include "n3readerbasecommand.h"

class QTimer;
class N3ReadJumpsSummaryCommand : public N3ReaderBaseCommand
{
    Q_OBJECT
public:
    explicit N3ReadJumpsSummaryCommand(QObject *parent = 0);
    virtual void execute();
    virtual void processPacket(const QByteArray &data);
    virtual const QString &commandName() const;
    virtual const QString &description() const;
    QByteArray getOutBuffer() const;
    void setOutBuffer(const QByteArray &value);

private:
    enum class CommandState {
        initial,
        awaitingAcknowledgementStart,
        receivingAcknowledgement,
        badAcknowledgement,
        receivingData
    };
    CommandState m_state = CommandState::initial;
    static const int msByteSendingDelay=10;
    static const int memoryBlockLength=32;
    static const QByteArray acknowledgementString;
    QByteArray outBuffer;
    QByteArray inBuffer;
    QByteArray rawData;
    QTimer *outDataSendTimer;
    int outBufferPosition=0;
private slots:
    void onOutDataSendTimerShot();
signals:
    
public slots:
    
};

#endif // N3READJUMPSSUMMARYCOMMAND_H
