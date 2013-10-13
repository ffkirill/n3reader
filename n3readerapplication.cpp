#include "n3readerapplication.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QSharedPointer>
#include "n3readerhelper.h"
#include "n3readerresultconsoleview.h"
#include "commands/n3gettype0command.h"
#include "commands/n3readjumpssummarycommand.h"
#include <QDebug>
#include <iostream>
#include <QRegExp>
#include <QStringList>
#include <QString>

N3ReaderApplication::N3ReaderApplication(int &argc, char **argv) :
    QCoreApplication(argc, argv),
    m_ioDevice(new QSerialPort(this)),
    m_reader(new N3ReaderHelper(this)),
    m_view(new N3ReaderResultConsoleView(this)),
    m_timer(new QTimer(this))
{
    //Register commands
    registerCommand(new N3GetType0Command(m_reader));
    registerCommand(new N3ReadJumpsSummaryCommand(m_reader));

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
    std::cout << "Opening port" << std::endl;
    m_ioDevice->setPortName(m_portName);

    if (!m_ioDevice->open(QIODevice::ReadWrite)){
        std::cout << "Error:";
        std::cout << m_ioDevice->errorString().toLocal8Bit().data();
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
    std::cout << "Waiting 10 sec.." << std::endl;


    m_timer->setSingleShot(true);
    disconnect(m_timer);

    connect(m_timer, &QTimer::timeout, [this, initialCommand](){

        std::cout << "Working.." << std::endl;

        m_reader->executeCommand(initialCommand);

    });

    m_timer->start(10000);
}

bool usage()
{
    std::cout << "Usage:";
    std::cout << "n3reader --port=<port_name> <command>" << std:: endl;
    return false;
}

inline QStringList serialPortNames()
{
    QStringList result;
    for (const QSerialPortInfo &port:QSerialPortInfo::availablePorts()){
        result.append(port.portName());
    }

    return result;
}

inline bool checkPortName(const QString &portName)
{
    QStringList ports = serialPortNames();
    if (ports.indexOf(portName)==-1) {
        std::cout << "Invalid port name. ";
        std::cout << "Port names are:" << std::endl <<" ";
        for (const QString &name:ports){
            std::cout <<name.toLocal8Bit().data() << " ";
        }
        std::cout << std::endl;
        return false;
    }

    return true;
}

inline void displayCommandNames(const QMap<QString, N3ReaderBaseCommand*> &m_commands)
{
    std::cout << "Invalid command. Commands are:" <<std::endl;
    for (auto key:m_commands.keys()) {
        std::cout << key.toLocal8Bit().data();
        std::cout << " - " << m_commands.value(key)->description()
                     .toLocal8Bit().data() << std::endl;
    }
}

bool N3ReaderApplication::parseCommandLine()
{
    QStringList args = arguments();
    QRegExp portRx("--port=(\\S+)");
    QRegExp cmdRx("(\\w+)");
    int portParamIdx=-1;
    if (args.length()<3 || (portParamIdx=args.indexOf(portRx))==-1) {
        return usage();
    }
    portRx.indexIn(args[portParamIdx]);
    m_portName = portRx.capturedTexts()[1];

    if (!checkPortName(m_portName))
        return false;

    int cmdIdx = args.indexOf(cmdRx);

    if (cmdIdx == -1) {
        return usage();
    }

    cmdRx.indexIn(args[cmdIdx]);
    m_commandName = cmdRx.capturedTexts()[1];
    if (!m_commands.contains(m_commandName)) {
        displayCommandNames(m_commands);
        return false;
    }
    return true;
}

void N3ReaderApplication::runTask()
{
    //check command line params
    if (!parseCommandLine()) {
        quit();
        return;
    }

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
            std::cout << "Device ready" << std::endl;
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
