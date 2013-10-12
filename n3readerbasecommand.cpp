#include "n3readerbasecommand.h"
#include "n3readerhelper.h"

N3ReaderBaseCommand::N3ReaderBaseCommand(QObject *parent) :
    QObject(parent)
{
}

N3ReaderHelper *N3ReaderBaseCommand::reader()
{
    return qobject_cast<N3ReaderHelper *>(parent());
}
