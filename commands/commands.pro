#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T14:17:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = commands
TEMPLATE = lib
CONFIG += staticlib


HEADERS += n3commands.h \
    commands/n3readjumpssummarycommandresult.h \
    commands/n3readjumpssummarycommand.h \
    commands/n3readerbaseresult.h \
    commands/n3readerbasecommand.h \
    commands/n3gettype0commandresult.h \
    commands/n3gettype0command.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

SOURCES += \
    commands/n3readjumpssummarycommandresult.cpp \
    commands/n3readjumpssummarycommand.cpp \
    commands/n3readerbaseresult.cpp \
    commands/n3readerbasecommand.cpp \
    commands/n3gettype0commandresult.cpp \
    commands/n3gettype0command.cpp
