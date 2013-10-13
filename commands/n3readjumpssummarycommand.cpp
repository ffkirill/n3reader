#include "n3readjumpssummarycommand.h"
#include "n3readerhelper.h"
#include <QString>
#include <QByteArray>
#include <QTimer>
#include "n3readjumpssummarycommandresult.h"
#include <QSharedPointer>

const QByteArray N3ReadJumpsSummaryCommand::acknowledgementString = QByteArray::fromHex("31 35");

N3ReadJumpsSummaryCommand::N3ReadJumpsSummaryCommand(QObject *parent) :
    N3ReaderBaseCommand(parent),
    outDataSendTimer(new QTimer(this))
{
    inBuffer.reserve(memoryBlockLength);

    outDataSendTimer->setInterval(N3ReadJumpsSummaryCommand::msByteSendingDelay);
    connect(outDataSendTimer, SIGNAL(timeout()), this, SLOT(onOutDataSendTimerShot()));
}

void N3ReadJumpsSummaryCommand::execute()
{
    QByteArray packet(9, 0);
    //length
    packet[0] = 7;
    //cmd code
    packet[1] = 0xA0;
    //memory address (0x0A);
    packet[2] = 0x0A;
    packet[3] = 0;
    packet[4] = 0;
    packet[5] = 0;
    //memory block length
    packet[6] = 0x1E;
    packet[7] = 0;
    packet[8] = N3ReaderHelper::calculateChecksum(packet);

    outBuffer = reader()->encryptPacket(packet);
    outBufferPosition = 0;
    m_state = CommandState::awaitingAcknowledgementStart;
    inBuffer.clear();
    rawData.clear();
    rawData.reserve(30);
    outDataSendTimer->start();
}

inline void appendBufferOnFirstCall(QByteArray &buffer, const QByteArray &data, bool &doAppend)
{
    if (doAppend) {
        buffer.append(data);
        doAppend = false;
    }
}

void N3ReadJumpsSummaryCommand::processPacket(const QByteArray &data)
{
    int index = 0;
    bool appendInBuffer = true;

    //first, need to receive acknowledgement
    if (m_state==CommandState::awaitingAcknowledgementStart
        && (index=data.indexOf(acknowledgementString[0]) != -1)) {
            m_state = CommandState::receivingAcknowledgement;
            inBuffer = data.mid(index-1);
            appendInBuffer = false;
    }

    if (m_state==CommandState::receivingAcknowledgement) {
        appendBufferOnFirstCall(inBuffer, data, appendInBuffer);
        if (inBuffer.size() >= acknowledgementString.size()) {
            if (inBuffer.startsWith(acknowledgementString)) {
                m_state = CommandState::receivingData;
                inBuffer = inBuffer.mid(acknowledgementString.size());
            } else {
                m_state = CommandState::badAcknowledgement;
                return;
            }
        }
    }

    //accumulate data in buffer after acknowledgement received
    if (m_state==CommandState::receivingData) {
        appendBufferOnFirstCall(inBuffer, data, appendInBuffer);
        if (inBuffer.size()>=memoryBlockLength) {
            emit sendPacket(QByteArray::fromHex("31"));
            QByteArray dataReceived = reader()->decryptPacket(inBuffer);
            if (rawData.size()) {
                rawData.append(dataReceived);
            } else {
                rawData.append(dataReceived.mid(3));
            }
            if (rawData.size() >= 30) {
                emit userDataAvailable(
                    QSharedPointer<N3ReadJumpsSummaryCommandResult>
                        (new N3ReadJumpsSummaryCommandResult(rawData))
                );
                emit commandFinished();
                rawData.clear();
            }
        }

    }
}

const QString &N3ReadJumpsSummaryCommand::commandName() const
{
    static const QString name("read_jumps_summary");
    return name;
}

const QString &N3ReadJumpsSummaryCommand::description() const
{
    static const QString name("Reads jumps summary data");
    return name;
}

void N3ReadJumpsSummaryCommand::onOutDataSendTimerShot()
{
    if (outBufferPosition<outBuffer.size()){
        emit sendPacket(outBuffer.mid(outBufferPosition, 1));
        ++outBufferPosition;
    } else {
        outDataSendTimer->stop();
        outBufferPosition=0;
        outBuffer.clear();
    }
}
