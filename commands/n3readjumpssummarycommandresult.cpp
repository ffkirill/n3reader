#include "n3readjumpssummarycommandresult.h"
#include <QByteArray>
#include <QDataStream>

N3ReadJumpsSummaryCommandResult::N3ReadJumpsSummaryCommandResult(QByteArray &rawData) :
    N3ReaderBaseResult(0),
    m_data(rawData)
{
}

QString N3ReadJumpsSummaryCommandResult::toString() const
{
    return
    QString("Number of jumps since last odometer reset: %1\n"
            "Total jumps stored (inc. deleted): %2\n"
            "Total jumps stored: %3\n"
            "Freefall time: %4 sec.\n"
            "Undercanopy time: %5 sec.\n"
            "Next jump number: %6\n"
            "Top jump number: %7\n"
            "Current Dropzone index: %8\n"
            "Current aircraft index: %9\n")
            .arg(getTotalJumpsSinceOdometerReset())
            .arg(getTotalJumpsStoredIncDeleted())
            .arg(getTotalJumpsStored())
            .arg(getFreefallTime())
            .arg(getCanopyPilotingTime())
            .arg(getNextJumpNumber())
            .arg(getTopJumpNumber())
            .arg(getCurrentDZIdx())
            .arg(getCurrentACIdx());
}

quint16 N3ReadJumpsSummaryCommandResult::getTotalJumpsSinceOdometerReset() const
{
    return getInt16(2);
}

quint16 N3ReadJumpsSummaryCommandResult::getTotalJumpsStoredIncDeleted() const
{
    return getInt16(4);
}

quint16 N3ReadJumpsSummaryCommandResult::getTotalJumpsStored() const
{
    return getInt16(6);
}

quint32 N3ReadJumpsSummaryCommandResult::getFreefallTime() const
{
    return getInt32(8);
}

quint32 N3ReadJumpsSummaryCommandResult::getCanopyPilotingTime() const
{
    return getInt32(12);
}

quint16 N3ReadJumpsSummaryCommandResult::getNextJumpNumber() const
{
    return getInt16(16);
}

quint16 N3ReadJumpsSummaryCommandResult::getTopJumpNumber() const
{
    return getInt16(18);
}

quint16 N3ReadJumpsSummaryCommandResult::getCurrentDZIdx() const
{
    return getInt16(24);
}

quint16 N3ReadJumpsSummaryCommandResult::getCurrentACIdx() const
{
    return getInt16(26);
}

quint16 N3ReadJumpsSummaryCommandResult::getInt16(int pos) const
{
    quint16 result;
    QDataStream st(m_data.mid(pos,2));
    st.setByteOrder(QDataStream::LittleEndian);
    st >> result;
    return result;
}

quint32 N3ReadJumpsSummaryCommandResult::getInt32(int pos) const
{
    quint32 result;
    QDataStream st (m_data.mid(pos,4));
    st.setByteOrder(QDataStream::LittleEndian);
    st >> result;
    return result;
}
