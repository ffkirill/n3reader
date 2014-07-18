#include "n3readerapplication.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QSharedPointer>
#include "n3readerhelper.h"
#include "n3readerresultconsoleview.h"
#include "commands/n3gettype0command.h"
#include "commands/n3readjumpssummarycommand.h"
#include "commands/n3readjumpsdetailscommand.h"
#include <QDebug>
#include <iostream>
#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QCommandLineParser>

N3ReaderApplication::N3ReaderApplication(int &argc, char **argv) :
    QCoreApplication(argc, argv),
    m_ioDevice(new QSerialPort(this)),
    m_reader(new N3ReaderHelper(this)),
    m_view(new N3ReaderResultConsoleView(this)),
    m_timer(new QTimer(this))
{
    //Common app settings
    QCoreApplication::setApplicationName("N3Reader");
    QCoreApplication::setApplicationVersion("0.0.1b");

    //Register commands
    registerCommand(new N3GetType0Command(m_reader));
    registerCommand(new N3ReadJumpsSummaryCommand(m_reader));
    registerCommand(new N3ReadJumpsDetailsCommand(m_reader));

    //Set up in and out data handlers
    QObject::connect(m_reader,&N3ReaderHelper::dataToSend, [this](const QByteArray &data){
        //qDebug()<<"OUT:"<<data.toHex();
        m_ioDevice->write(data);
        m_ioDevice->flush();
    });

    QObject::connect(m_ioDevice, &QSerialPort::readyRead, [this]{
        QByteArray data = m_ioDevice->readAll();
        //qDebug()<<"IN:"<<data.toHex();
        m_reader->processData(data);
    } );

    QTimer::singleShot(0, this, SLOT(runTask()));

}

void N3ReaderApplication::openPort(N3ReaderBaseCommand *initialCommand)
{
    std::cerr << "Opening port" << std::endl;
    m_ioDevice->setPortName(m_portName);

    if (!m_ioDevice->open(QIODevice::ReadWrite)){
        std::cerr << "Error:";
        std::cerr << m_ioDevice->errorString().toLocal8Bit().data();
        quit();
        return;
    }

    m_ioDevice->setBaudRate(QSerialPort::Baud57600);
    m_ioDevice->setDataBits(QSerialPort::Data8);
    m_ioDevice->setParity(QSerialPort::NoParity);
    m_ioDevice->setStopBits(QSerialPort::OneStop);
    m_ioDevice->setFlowControl(QSerialPort::HardwareControl);
    m_ioDevice->setReadBufferSize(0);

    m_ioDevice->setDataTerminalReady(true);
    std::cerr << "Waiting 10 sec.." << std::endl;


    m_timer->setSingleShot(true);
    disconnect(m_timer);

    connect(m_timer, &QTimer::timeout, [this, initialCommand](){

        std::cerr << "Working.." << std::endl;

        m_reader->executeCommand(initialCommand);

    });

    m_timer->start(10000);
}

inline QStringList serialPortNames()
{
    QStringList result;
    for (const QSerialPortInfo &port:QSerialPortInfo::availablePorts()){
        result.append(port.portName());
    }

    return result;
}

QStringList N3ReaderApplication::getCommandNames()
{
    QStringList result;
    for (auto key:m_commands.keys()) {
        result.append(
            QString("%1 - %2").arg(key).arg(m_commands.value(key)->description())
        );
    }

    return result;
}

inline bool checkPortName(const QString &portName)
{
    return serialPortNames().contains(portName);
}

void N3ReaderApplication::parseCommandLine()
{
    QCommandLineParser parser;
    parser.setApplicationDescription("N3Reader");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(
        "command",
        QString("Command, one of:\n%1").arg(this->getCommandNames().join("\n"))
    );

    QCommandLineOption portNameOption(
        QStringList() << "p" << "port",
        QString("Port name [%1]").arg(serialPortNames().join(", ")),
        "port"
    );

    parser.addOption(portNameOption);
    parser.process(*this);

    //Check if arguments correct
    if (parser.positionalArguments().size() != 1) {
        parser.showHelp(1);
    }
    m_portName = parser.value(portNameOption);

    if (!checkPortName(m_portName)) {
        parser.showHelp(1);
    }

    m_commandName = parser.positionalArguments().at(0);
    if (!m_commands.contains(m_commandName)) {
        parser.showHelp(1);
    }
}

void N3ReaderApplication::runTask()
{
    //check command line params
    parseCommandLine();

    N3ReaderBaseCommand * initialCommand = new N3GetType0Command(m_reader);
    m_command = m_commands[m_commandName];

    //for GetType0 command in command line we need to display result
    if (!strcmp(m_command->metaObject()->className(),
                initialCommand->metaObject()->className())){
        setCommandFinishedHandlers();
        initialCommand = m_command;
    }

    openPort(initialCommand);
    if (m_command == initialCommand) {
        //quit();
        return;
    }
    //start command on initial command finished
    connect(m_reader, &N3ReaderHelper::stateChanged, [this](const N3ReaderHelper::ReaderState & state){
        if (state == N3ReaderHelper::ReaderState::ready){
            std::cerr << "Device ready" << std::endl;
            setCommandFinishedHandlers();
            m_reader->executeCommand(m_command);
        }
    });
}

void N3ReaderApplication::setCommandFinishedHandlers()
{
    connect(m_command,
        SIGNAL(userDataAvailable(QSharedPointer<N3ReaderBaseResult>)),
        m_view,
        SLOT(displayResult(QSharedPointer<N3ReaderBaseResult>))
    );
    connect(m_command, &N3ReaderBaseCommand::commandFinished, [this](){
        disconnect(m_reader);
        disconnect(m_command);
        quit();
    }
    );
}

void N3ReaderApplication::registerCommand(N3ReaderBaseCommand *command, char *cmdline_name)
{
    if (cmdline_name) {
        m_commands[cmdline_name] = command;
    } else {
        m_commands[command->commandName()]=command;
    }
}
