#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Rdk-TensorflowLib.qt
TEMPLATE = lib
CONFIG += staticlib

DEFINES += LIBRDK_LIBRARY_EXPORT
DEFINES += RDK_UNICODE_RUN
DEFINES += RDK_QT

VERSION = $$system(hg parents --template '{rev}')
DEFINES += RDK_LIB_CVS_REVISION=$$VERSION

unix {
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}

HEADERS += \


SOURCES += \

