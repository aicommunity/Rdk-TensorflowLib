#-------------------------------------------------
#
# Project created by QtCreator 2012-11-11T15:17:55
#
#-------------------------------------------------

QT       -= gui

TARGET = Rdk-TensorflowLib.qt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11

include($$PWD/../../../../Rdk/Build/Lib/Qt/RdkDefines.pri)

DEFINES += LIBRDK_LIBRARY_EXPORT
DEFINES += RDK_UNICODE_RUN
DEFINES += RDK_QT

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += NOMINMAX

VERSION = $$system(hg parents --template '{rev}')
DEFINES += RDK_LIB_CVS_REVISION=$$VERSION

include($$PWD/../../../../Rdk/Build/Lib/Qt/RdkDefines.pri)

unix {
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
}


HEADERS += \
    ../../Core/Lib.h \
    ../../Core/Interface/ttfsession.h \
    ../../Core/Interface/ttfsessionSqDet.h \
    ../../Core/UTfComponent.h  \
    ../../Core/UTfClassifier.h  \
    ../../Core/UTfDetector.h  \
    ../../Core/UTfSqDetector.h


SOURCES += \
    ../../Core/Lib.cpp \
    ../../Core/Interface/ttfsession.cpp \
    ../../Core/Interface/ttfsessionSqDet.cpp \
    ../../Core/UTfComponent.cpp \
    ../../Core/UTfClassifier.cpp \
    ../../Core/UTfDetector.cpp  \
    ../../Core/UTfSqDetector.cpp

