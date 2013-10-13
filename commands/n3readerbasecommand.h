#ifndef N3READERBASECOMMAND_H
#define N3READERBASECOMMAND_H

#include <QObject>

class N3ReaderBaseResult;
class N3ReaderHelper;
class N3ReaderBaseCommand : public QObject
{
    Q_OBJECT
public:
    explicit N3ReaderBaseCommand(QObject *parent);
    N3ReaderHelper *reader();
    virtual void execute()=0;
    virtual void processPacket(const QByteArray &data)=0;
    virtual const QString &commandName() const = 0;
    virtual const QString &description() const = 0 ;
signals:
    void sendPacket(const QByteArray &data);
    void commandFinished();
    void userDataAvailable(QSharedPointer<N3ReaderBaseResult>);
public slots:
    
};

#endif // N3READERBASECOMMAND_H
