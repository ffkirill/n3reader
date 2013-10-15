#ifndef N3READJUMPSDETAILSCOMMAND_H
#define N3READJUMPSDETAILSCOMMAND_H
#include "n3readmemoryblockcommand.h"

class N3ReadJumpsDetailsCommand : public N3ReadMemoryBlockCommand
{
    Q_OBJECT
public:
    explicit N3ReadJumpsDetailsCommand(QObject *parent = 0);
    virtual const QString &commandName() const;
    virtual const QString &description() const;
private:
    virtual void rawDataReady();
private slots:

signals:

public slots:

};

#endif // N3READJUMPSDETAILSCOMMAND_H
