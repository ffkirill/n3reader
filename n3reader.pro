#-------------------------------------------------
#
# Project created by QtCreator 2013-09-15T21:58:13
#
#-------------------------------------------------

QT       += core serialport

QT       -= gui

TARGET = n3reader
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    n3readerhelper.cpp \
    n3readerbasecommand.cpp \
    n3readerbaseresult.cpp \
    n3gettype0command.cpp \
    n3gettype0commandresult.cpp \
    n3readerresultconsoleview.cpp \
    n3readerapplication.cpp \
    n3readjumpssummarycommand.cpp \
    n3readjumpssummarycommandresult.cpp

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    n3readerhelper.h \
    n3readerbasecommand.h \
    n3readerbaseresult.h \
    n3gettype0command.h \
    n3gettype0commandresult.h \
    n3readerresultconsoleview.h \
    n3readerapplication.h \
    n3readjumpssummarycommand.h \
    n3readjumpssummarycommandresult.h
