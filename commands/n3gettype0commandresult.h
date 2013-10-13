#ifndef N3GETTYPE0COMMANDRESULT_H
#define N3GETTYPE0COMMANDRESULT_H
#include "n3readerbaseresult.h"

class N3GetType0CommandResult : public N3ReaderBaseResult
{
    Q_OBJECT
private:
    const QByteArray m_data;
    const QString deviceType[7];
public:
    explicit N3GetType0CommandResult(QByteArray &type0packet);
    virtual QString toString() const;
    QString getProductType() const;
    QString getSerialNumber() const;
    quint8 getSoftwareVersion() const;
    quint8 getSoftwareRevision() const;
signals:
    
public slots:
    
};

#endif // N3GETTYPE0COMMANDRESULT_H
