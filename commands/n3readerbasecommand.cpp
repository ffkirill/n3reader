#include "n3readerbasecommand.h"
#include "n3readerhelper.h"
#include <QList>
N3ReaderBaseCommand::N3ReaderBaseCommand(QObject *parent) :
    QObject(parent)
{
}

N3ReaderHelper *N3ReaderBaseCommand::reader()
{
    return qobject_cast<N3ReaderHelper *>(parent());
}

QList<N3ReaderBaseCommand *> N3ReaderBaseCommand::requrements()
{
    return QList<N3ReaderBaseCommand *>();
}
