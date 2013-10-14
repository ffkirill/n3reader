#include "n3readerresultconsoleview.h"
#include "commands/n3readerbaseresult.h"
#include <iostream>
#include <QSharedPointer>

N3ReaderResultConsoleView::N3ReaderResultConsoleView(QObject *parent) :
    QObject(parent)
{
}

void N3ReaderResultConsoleView::displayResult(const N3ReaderBaseResult &result)
{
    std::cout << result.toString().toLocal8Bit().data();
}

void N3ReaderResultConsoleView::displayResult(QSharedPointer<N3ReaderBaseResult> result)
{
    displayResult(*result.data());
}

