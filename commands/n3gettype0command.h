#ifndef N3GETTYPE0COMMAND_H
#define N3GETTYPE0COMMAND_H
#include "n3readerbasecommand.h"
#include "n3gettype0commandresult.h"

class N3GetType0Command : public N3ReaderBaseCommand
{
    Q_OBJECT
private:
    const quint8 type0Size=97;
    QByteArray inBuffer;
    bool verifyType0Record() const;
    void setEncryptionKey();
public:
    explicit N3GetType0Command(QObject *parent);
    virtual void execute();
    virtual void processPacket(const QByteArray &data);
    virtual const QString &commandName() const;
    virtual const QString &description() const;
signals:
    
public slots:
    
};

#endif // N3GETTYPE0COMMAND_H
