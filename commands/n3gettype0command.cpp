#include "n3gettype0command.h"
#include "n3gettype0commandresult.h"
#include "n3readerhelper.h"

N3GetType0Command::N3GetType0Command(QObject *parent) :
    N3ReaderBaseCommand(parent)
{
}

bool N3GetType0Command::verifyType0Record() const
{
    if (inBuffer[1] != 0) {
        return false;
    }

    const uchar length = inBuffer[0];
    uint checksum = 0;
    for (char i = 1; i<length+1; ++i) {
        checksum += inBuffer[i];
    }

    checksum %= 256;

    uint packet_checksum = (uint)inBuffer[length+1] & 0xFF;
    if (checksum != packet_checksum) {
        return false;
    }

    return true;
}

void N3GetType0Command::setEncryptionKey()
{
    uint32_t key[4];
    key[0] = N3ReaderHelper::
             bytesToUInt(78,
                         inBuffer[8],
                         inBuffer[26],
                         inBuffer[24]);

    key[1] = N3ReaderHelper::
            bytesToUInt(inBuffer[6],
                         inBuffer[25],
                         inBuffer[23],
                         inBuffer[13]);

    key[2] = N3ReaderHelper::
             bytesToUInt(inBuffer[10],
                         117,
                         inBuffer[7],
                         inBuffer[22]);

    key[3] = N3ReaderHelper::
             bytesToUInt(inBuffer[9],
                         inBuffer[11],
                         126,
                         inBuffer[21]);

    reader()->setKey(key);
}

void N3GetType0Command::execute()
{
    emit sendPacket(QByteArray("018080"));
}


void N3GetType0Command::processPacket(const QByteArray &packet)
{
    inBuffer.append(packet);

    if (inBuffer.size() >= type0Size) {

        //Verify the packet received
        inBuffer = QByteArray::fromHex(inBuffer);

        if (verifyType0Record()) {
            setEncryptionKey();
            emit userDataAvailable(QSharedPointer<N3GetType0CommandResult>(new N3GetType0CommandResult(inBuffer)));
            emit commandFinished();
        }
    }
}

const QString &N3GetType0Command::commandName() const
{
    static const QString name("get_device_info");
    return name;

}

const QString &N3GetType0Command::description() const
{
    static const QString desc("Read device model, s/n, s/w and h/w versions");
    return desc;
}
