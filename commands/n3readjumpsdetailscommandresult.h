#ifndef N3READJUMPSDETAILSCOMMANDRESULT_H
#define N3READJUMPSDETAILSCOMMANDRESULT_H
#include "n3readerbaseresult.h"
#include <QDateTime>

class N3ReadJumpsDetailsCommandResult : public N3ReaderBaseResult
{
    Q_OBJECT
public:
    explicit N3ReadJumpsDetailsCommandResult(const QByteArray &data, QObject *parent = 0);
    const quint16 jumpNumber;
    const bool deletionMark;
    const QDateTime jumpDate;
    const quint16 freeFallTime;
    const quint16 canopyPilotingTime;
    const quint16 exitAltitude;
    const quint16 deployAltitude;
    const quint16 speed3K;
    const quint16 speed6K;
    const quint16 speed9K;
    const quint16 speed12K;
    virtual QString toString() const;
private:
    inline quint16 getJumpNumber(const QByteArray &data);
    inline bool getDeletionMark(const QByteArray &data);
    inline QDateTime getJumpDate(const QByteArray &data);
    inline quint16 getFreeFallTime(const QByteArray &data);
    inline quint16 getCanopyPilotingTime(const QByteArray &data);
    inline quint16 getExitAltitude(const QByteArray &data);
    inline quint16 getDeployAltitude(const QByteArray &data);
    inline quint16 getSpeed3K(const QByteArray &data);
    inline quint16 getSpeed6K(const QByteArray &data);
    inline quint16 getSpeed9K(const QByteArray &data);
    inline quint16 getSpeed12K(const QByteArray &data);
    inline quint16 getInt16(const QByteArray &data, int pos);
signals:
    
public slots:
    
};

#endif // N3READJUMPSDETAILSCOMMANDRESULT_H
