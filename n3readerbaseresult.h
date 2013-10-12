#ifndef N3READERBASERESULT_H
#define N3READERBASERESULT_H

#include <QObject>

class N3ReaderBaseResult : public QObject
{
    Q_OBJECT
public:
    explicit N3ReaderBaseResult(QObject *parent = 0);
    virtual QString toString() const = 0;
signals:
    
public slots:
    
};

#endif // N3READERBASERESULT_H
