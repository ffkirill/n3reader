#ifndef N3READERRESULTCONSOLEVIEW_H
#define N3READERRESULTCONSOLEVIEW_H

#include <QObject>

class N3ReaderBaseResult;
class N3GetType0CommandResult;
class N3ReaderResultConsoleView : public QObject
{
    Q_OBJECT
public:
    explicit N3ReaderResultConsoleView(QObject *parent = 0);
    
signals:
    
public slots:
    void displayResult(const N3ReaderBaseResult &result);
    void displayResult(QSharedPointer<N3ReaderBaseResult> result);
    
};

#endif // N3READERRESULTCONSOLEVIEW_H
