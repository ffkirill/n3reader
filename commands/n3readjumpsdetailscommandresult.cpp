#include "n3readjumpsdetailscommandresult.h"
#include <QByteArray>
#include <QDataStream>
#include <QDate>
#include <QTime>
#include <cmath>

N3ReadJumpsDetailsCommandResult::N3ReadJumpsDetailsCommandResult(const QByteArray &data, QObject *parent) :
    N3ReaderBaseResult(parent),
    jumpNumber(getJumpNumber(data)),
    deletionMark(getDeletionMark(data)),
    jumpDate(getJumpDate(data)),
    freeFallTime(getFreeFallTime(data)),
    canopyPilotingTime(getCanopyPilotingTime(data)),
    exitAltitude(getExitAltitude(data)),
    deployAltitude(getDeployAltitude(data)),
    speed3K(getSpeed3K(data)),
    speed6K(getSpeed6K(data)),
    speed9K(getSpeed9K(data)),
    speed12K(getSpeed12K(data))
{
}

QString N3ReadJumpsDetailsCommandResult::toString() const
{
    return QString("Jump number: %1\n"
                   "Date and time: %2\n"
                   "Exit altitude: %3 meters\n"
                   "Deploy altitude: %4 meters\n"
                   "Freefall time: %5 sec.\n"
                   "Canopy piloting time: %6 sec\n"
                   "3.5K meters speed: %7 km\\h\n"
                   "2.5K meters speed: %8 km\\h\n"
                   "1.9K meters speed: %9 km\\h\n"
                   "0.9K meters speed: %10 km\\h\n"
                   )
            .arg(jumpNumber)
            .arg(jumpDate.toString())
            .arg(exitAltitude)
            .arg(deployAltitude)
            .arg(freeFallTime)
            .arg(canopyPilotingTime)
            .arg(speed12K)
            .arg(speed9K)
            .arg(speed6K)
            .arg(speed3K);
}

quint16 N3ReadJumpsDetailsCommandResult::getJumpNumber(const QByteArray &data)
{
    return getInt16(data, 0);
}

bool N3ReadJumpsDetailsCommandResult::getDeletionMark(const QByteArray &data)
{
    return (data[2] >> 7) & 1;
}

QDateTime N3ReadJumpsDetailsCommandResult::getJumpDate(const QByteArray &data)
{
    QDate date(2007 + ((data[2] & 0x7F) - 1) / 12,
            (data[2] & 0x7F) % 12,
            (data[13] >> 2) & 0b11111);
    QTime time((getInt16(data, 6) >> 6) & 0b11111,
               getInt16(data, 6) & 0x3F);

    return QDateTime(date, time);
}

quint16 N3ReadJumpsDetailsCommandResult::getFreeFallTime(const QByteArray &data)
{
    return getInt16(data, 4) & 0x03FF;
}

quint16 N3ReadJumpsDetailsCommandResult::getCanopyPilotingTime(const QByteArray &data)
{
    return getInt16(data, 16) & 0x0FFF;
}

quint16 N3ReadJumpsDetailsCommandResult::getExitAltitude(const QByteArray &data)
{
    return (getInt16(data, 12) & 0x03ff) * 16;
}

quint16 N3ReadJumpsDetailsCommandResult::getDeployAltitude(const QByteArray &data)
{
    return (getInt16(data, 14) & 0x03ff) * 16;
}

quint16 N3ReadJumpsDetailsCommandResult::getSpeed3K(const QByteArray &data)
{
    return floor((data[8]&0x7F) * 3.6 + 0.5);
}

quint16 N3ReadJumpsDetailsCommandResult::getSpeed6K(const QByteArray &data)
{
    return floor(((getInt16(data, 8) >> 7) & 0x7F) * 3.6 + 0.5);
}

quint16 N3ReadJumpsDetailsCommandResult::getSpeed9K(const QByteArray &data)
{
    return floor(((getInt16(data, 9) >> 6) & 0x7F) * 3.6 + 0.5);
}

quint16 N3ReadJumpsDetailsCommandResult::getSpeed12K(const QByteArray &data)
{
    return floor(((getInt16(data, 10) >>5) & 0x7F) * 3.6 + 0.5);
}

quint16 N3ReadJumpsDetailsCommandResult::getInt16(const QByteArray &data, int pos)
{
    quint16 result;
    QDataStream st(data.mid(pos,2));
    st.setByteOrder(QDataStream::LittleEndian);
    st >> result;
    return result;
}
