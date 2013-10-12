#ifndef N3READJUMPSSUMMARYCOMMANDRESULT_H
#define N3READJUMPSSUMMARYCOMMANDRESULT_H
#include "n3readerbaseresult.h"

class N3ReadJumpsSummaryCommandResult : public N3ReaderBaseResult
{
    Q_OBJECT
private:
    const QByteArray m_data;
    quint16 getInt16(int pos) const;
    quint32 getInt32(int pos) const;
public:
    explicit N3ReadJumpsSummaryCommandResult(QByteArray &rawData);
    virtual QString toString() const;
    quint16 getTotalJumpsSinceOdometerReset() const;
    quint16 getTotalJumpsStoredIncDeleted() const;
    quint16 getTotalJumpsStored() const;
    quint32 getFreefallTime() const;
    quint32 getCanopyPilotingTime() const;
    quint16 getNextJumpNumber() const;
    quint16 getTopJumpNumber() const;
    quint16 getCurrentDZIdx() const;
    quint16 getCurrentACIdx() const;
signals:
    
public slots:
    
};

#endif // N3READJUMPSSUMMARYCOMMANDRESULT_H
