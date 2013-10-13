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
    n3readerresultconsoleview.cpp \
    n3readerapplication.cpp

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    n3readerhelper.h \
    n3readerresultconsoleview.h \
    n3readerapplication.h

include(commands/commands.pro)
