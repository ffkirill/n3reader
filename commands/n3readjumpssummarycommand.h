#ifndef N3READJUMPSSUMMARYCOMMAND_H
#define N3READJUMPSSUMMARYCOMMAND_H
#include "n3readmemoryblockcommand.h"

class N3ReadJumpsSummaryCommand : public N3ReadMemoryBlockCommand
{
    Q_OBJECT
public:
    explicit N3ReadJumpsSummaryCommand(QObject *parent = 0);
    virtual const QString &commandName() const;
    virtual const QString &description() const;
private:
    virtual void rawDataReady();
private slots:

signals:
    
public slots:
    
};

#endif // N3READJUMPSSUMMARYCOMMAND_H
