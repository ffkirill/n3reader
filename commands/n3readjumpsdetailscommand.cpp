#include "n3readjumpsdetailscommand.h"
#include <QDebug>

N3ReadJumpsDetailsCommand::N3ReadJumpsDetailsCommand(QObject *parent) :
    N3ReadMemoryBlockCommand(0x0520,22*3, 22, parent)
{
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

void N3ReadJumpsDetailsCommand::rawDataReady()
{
    QByteArray data = rawData.left(resultPresentationQuantum);
    qDebug() << data.toHex();
    qDebug() <<resultPresentationQuantum;
    qDebug() << data.size();
//    emit userDataAvailable(
//        QSharedPointer<N3ReadJumpsSummaryCommandResult>
//            (new N3ReadJumpsSummaryCommandResult(data))
//    );
}
