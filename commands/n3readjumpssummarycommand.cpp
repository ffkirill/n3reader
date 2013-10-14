#include "n3readjumpssummarycommand.h"
#include "n3readerhelper.h"
#include <QString>
#include <QByteArray>
#include "n3readjumpssummarycommandresult.h"
#include <QSharedPointer>

N3ReadJumpsSummaryCommand::N3ReadJumpsSummaryCommand(QObject *parent) :
    N3ReadMemoryBlockCommand(0x0A, 0x1E, parent)
{
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

void N3ReadJumpsSummaryCommand::rawDataReady()
{
    QByteArray data = rawData.left(resultPresentationQuantum);
    emit userDataAvailable(
        QSharedPointer<N3ReadJumpsSummaryCommandResult>
            (new N3ReadJumpsSummaryCommandResult(data))
    );
}
