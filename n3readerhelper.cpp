#include "n3readerhelper.h"
#include "commands/n3readerbasecommand.h"
#include <QSerialPort>
#include <QDataStream>

N3ReaderHelper::N3ReaderHelper(QObject *parent) :
    QObject(parent)
{

}

inline uchar getByteOrAddition(const QByteArray & packet,
                        const uint &index,
                        const uint &packet_size)
{
    return (index<packet_size ? packet[index] : 0);
}

uint32_t N3ReaderHelper::bytesToUInt(const uchar &byte0,
                     const uchar &byte1,
                     const uchar &byte2,
                     const uchar &byte3)
{
    QByteArray data(4,0);
    data[0] = byte0;
    data[1] = byte1;
    data[2] = byte2;
    data[3] = byte3;
    QDataStream st(data);
    st.setByteOrder(QDataStream::LittleEndian);
    quint32 res;
    st >> res;
    return res;
}

QByteArray N3ReaderHelper::UIntToBytes(const quint32 &dword)
{
    QByteArray result;
    QDataStream st(&result, QIODevice::ReadWrite);
    st.setByteOrder(QDataStream::LittleEndian);
    st << dword;
    return result;
}

uchar N3ReaderHelper::calculateChecksum(const QByteArray &packet)
{
    uchar length = packet[0];
    uchar checksum = 0;
    for (uchar i=1; i<length+1; ++i) {
        checksum+=packet[i];
    }
    return checksum;
}

void N3ReaderHelper::encrypt(uint32_t &U, uint32_t &U1) const
{
    uint32_t U2 = 0;
    for (int i = 16; i > 0; i--)
    {
        U += (((U1 << 4) ^ (U1 >> 5)) + U1) ^ (U2 + m_key[U2 & 3]);
        U2 += 0x9E3779B9;
        U1 += (((U << 4) ^ (U >> 5)) + U) ^ (U2 + m_key[(U2 >> 11) & 3]);
    }
}

void N3ReaderHelper::decrypt(uint32_t & U, uint32_t & U1) const
{
  uint32_t U2 = 0xE3779B90;
  for (int i = 16; i > 0; i--)
  {
      U1 -= (((U << 4) ^ (U >> 5)) + U) ^ (U2 + m_key[(U2 >> 11) & 3]); U2 -= 0x9E3779B9;
      U -= (((U1 << 4) ^ (U1 >> 5)) + U1) ^ (U2 + m_key[U2 & 3]);
  }
}

QByteArray N3ReaderHelper::encryptPacket(const QByteArray & packet) const
{
    uint addition_length = 0;
    uint packet_size = packet.size();
    QByteArray result;

    // Calculate addition to 2 dwords length
    if (packet_size < N3_ENCODED_PACKET_SIZE) {
        addition_length = N3_ENCODED_PACKET_SIZE - packet_size;
    } else if (packet_size % N3_ENCODED_PACKET_SIZE) {
        addition_length = N3_ENCODED_PACKET_SIZE - (packet_size % N3_ENCODED_PACKET_SIZE);
    }

    // Send block of 2 dwords to encrypt
    for (uint i=0; i<packet_size+addition_length; ++i) {

        if ((i>=7) && ((i+1)%8==0)) {

            uint32_t dword0 = bytesToUInt(
                        getByteOrAddition(packet, i-7, packet_size),
                        getByteOrAddition(packet, i-6, packet_size),
                        getByteOrAddition(packet, i-5, packet_size),
                        getByteOrAddition(packet, i-4, packet_size)
            );

            uint32_t dword1 = bytesToUInt(
                        getByteOrAddition(packet, i-3, packet_size),
                        getByteOrAddition(packet, i-2, packet_size),
                        getByteOrAddition(packet, i-1, packet_size),
                        getByteOrAddition(packet, i,   packet_size)
            );

            encrypt(dword0, dword1);

            result.append(UIntToBytes(dword0));
            result.append(UIntToBytes(dword1));

        }
    }

    return result;

}

QByteArray N3ReaderHelper::decryptPacket(const QByteArray &packet) const
{
    uint addition_length = 0;
    uint packet_size = packet.size();
    QByteArray result;

    // Calculate addition to 2 dwords length
    if (packet_size < N3_ENCODED_PACKET_SIZE) {
        addition_length = N3_ENCODED_PACKET_SIZE - packet_size;
    } else if (packet_size % N3_ENCODED_PACKET_SIZE) {
        addition_length = N3_ENCODED_PACKET_SIZE - (packet_size % N3_ENCODED_PACKET_SIZE);
    }

    // Send block of 2 dwords to decrypt
    for (uint i=0; i<packet_size+addition_length; ++i) {

        if ((i>=7) && ((i+1)%8==0)) {

            uint32_t dword0 = bytesToUInt(
                        getByteOrAddition(packet, i-7, packet_size),
                        getByteOrAddition(packet, i-6, packet_size),
                        getByteOrAddition(packet, i-5, packet_size),
                        getByteOrAddition(packet, i-4, packet_size)
            );

            uint32_t dword1 = bytesToUInt(
                        getByteOrAddition(packet, i-3, packet_size),
                        getByteOrAddition(packet, i-2, packet_size),
                        getByteOrAddition(packet, i-1, packet_size),
                        getByteOrAddition(packet, i,   packet_size)
            );

            decrypt(dword0, dword1);

            result.append(UIntToBytes(dword0));
            result.append(UIntToBytes(dword1));

        }
    }

    return result;

}

void N3ReaderHelper::executeCommand(N3ReaderBaseCommand *command)
{
    m_command = command;
    setDeviceState(N3ReaderHelper::ReaderState::processing);
    connect(m_command, SIGNAL(sendPacket(QByteArray)), this, SLOT(sendData(QByteArray)));
    connect(m_command, SIGNAL(commandFinished()), this, SLOT(commandFinished()));
    m_command->execute();
}

void N3ReaderHelper::setKey(const uint32_t key[])
{
    m_key[0] = key[0];
    m_key[1] = key[1];
    m_key[2] = key[2];
    m_key[3] = key[3];
}

void N3ReaderHelper::setDeviceState(const N3ReaderHelper::ReaderState &state)
{
    m_state = state;
    emit stateChanged(m_state);
}

void N3ReaderHelper::commandFinished()
{
    disconnect(m_command, 0, this, 0);
    m_command = 0;
    setDeviceState(N3ReaderHelper::ReaderState::ready);
}

void N3ReaderHelper::sendData(const QByteArray &data)
{
    emit dataToSend(data);
}

void N3ReaderHelper::processData(const QByteArray &data)
{
    if (m_command) {
        m_command->processPacket(data);
    }
}

void N3ReaderHelper::processData()
{
    QSerialPort *ioDevice = qobject_cast<QSerialPort *>(sender());
    QByteArray data = ioDevice->readAll();
    processData(data);
}
