#ifndef N3READERAPPLICATION_H
#define N3READERAPPLICATION_H
#include <QCoreApplication>
#include <QSharedPointer>
#include <QMap>


class QSerialPort;
class N3ReaderHelper;
class N3ReaderResultConsoleView;
class N3ReaderBaseCommand;
class QTimer;


class N3ReaderApplication : public QCoreApplication
{
    Q_OBJECT
public:
    void registerCommand(N3ReaderBaseCommand *command,
                         char *cmdline_name=0);
    explicit N3ReaderApplication(int &argc, char **argv);
private:
    QSerialPort *m_ioDevice;
    N3ReaderHelper *m_reader;
    N3ReaderResultConsoleView *m_view;
    QTimer *m_timer;
    QString m_portName;
    QString m_commandName;
    N3ReaderBaseCommand *m_command;
    QMap<QString, N3ReaderBaseCommand*> m_commands;
    void openPort(N3ReaderBaseCommand *initialCommand);
    void setCommandFinishedHandlers();
    bool parseCommandLine();
    bool usage();
signals:
    
public slots:
    void runTask();
    
};

#endif // N3READERAPPLICATION_H
