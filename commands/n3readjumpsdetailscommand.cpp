#include "n3readjumpsdetailscommand.h"
#include "n3readjumpsdetailscommandresult.h"
#include "n3readerhelper.h"
#include <QDataStream>

N3ReadJumpsDetailsCommand::N3ReadJumpsDetailsCommand(QObject *parent) :
    N3ReadMemoryBlockCommand(0x0520, 22, 22, parent),
    m_readBlockSizeCommand(new N3ReadMemoryBlockCommand(14, 2, 2,parent))
{
    connect(m_readBlockSizeCommand, SIGNAL(rawDataAvailable(QByteArray)), this, SLOT(memoryBlockSizeDataReady(QByteArray)));
}

const QString &N3ReadJumpsDetailsCommand::commandName() const
{
    static const QString name("read_jumps_details");
    return name;
}

const QString &N3ReadJumpsDetailsCommand::description() const
{
    static const QString name("Reads jumps details data");
    return name;
}

QList<N3ReaderBaseCommand *> N3ReadJumpsDetailsCommand::requrements()
{
    QList<N3ReaderBaseCommand *> result;
    result << m_readBlockSizeCommand;
    return result;
}

quint16 getInt16(const QByteArray &m_data, int pos)
{
    quint16 result;
    QDataStream st(m_data.mid(pos,2));
    st.setByteOrder(QDataStream::LittleEndian);
    st >> result;
    return result;
}

void N3ReadJumpsDetailsCommand::rawDataReady()
{
    QByteArray data = rawData.left(resultPresentationQuantum);

    emit userDataAvailable(
        QSharedPointer<N3ReadJumpsDetailsCommandResult>
            (new N3ReadJumpsDetailsCommandResult(data))
        );
}

void N3ReadJumpsDetailsCommand::memoryBlockSizeDataReady(const QByteArray &data)
{
    setMemoryBlockLength(N3ReaderHelper::bytesToUInt(data[0], data[1], 0, 0) * resultPresentationQuantum);
    disconnect(m_readBlockSizeCommand, 0, this, 0);
}
