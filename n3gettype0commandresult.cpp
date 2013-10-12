#include "n3gettype0commandresult.h"

N3GetType0CommandResult::N3GetType0CommandResult(QByteArray &type0packet) :
    N3ReaderBaseResult(0),
    m_data(type0packet.mid(3,12)),
    deviceType{"Unknown",
               "Neptune",
               "Wave",
               "Tracker",
               "Data logger",
               "Neptune 3",
               "Neptune 3 Audio"}
{
}

QString N3GetType0CommandResult::toString() const
{
    return
    QString("%1 s/n %2 \n"
            "s/w version %3 rev %4\n")
            .arg(getProductType())
            .arg(getSerialNumber())
            .arg(QString::number(getSoftwareVersion()))
            .arg(QString::number(getSoftwareRevision()));
}

QString N3GetType0CommandResult::getProductType() const
{
    uchar pType = m_data[11];
    if (pType >0 && pType<6) {
        return deviceType[pType];
    } else {
        return "Unknown";
    }
}

QString N3GetType0CommandResult::getSerialNumber() const
{
    QString result;
    result.append(m_data.mid(3,9));
    return result;
}

quint8 N3GetType0CommandResult::getSoftwareVersion() const
{
    return m_data[1];
}

quint8 N3GetType0CommandResult::getSoftwareRevision() const
{
    return m_data[2];
}
