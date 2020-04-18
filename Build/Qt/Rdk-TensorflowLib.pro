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

DEFINES += LIBRDK_LIBRARY_EXPORT
DEFINES += RDK_UNICODE_RUN
DEFINES += RDK_QT

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += NOMINMAX

VERSION = $$system(hg parents --template '{rev}')
DEFINES += RDK_LIB_CVS_REVISION=$$VERSION

INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-genfiles
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/eigen_archive
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/protobuf_archive/src
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/com_google_protobuf/src
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/com_google_absl
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow
DEPENDPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow
INCLUDEPATH += $$(TENSORFLOW_PATH)


unix {
    DESTDIR = $$PWD/../../../../Bin/Platform/Linux/Lib.Qt
    LIBS += -L$$(TENSORFLOW_PATH)/bazel-bin/tensorflow -ltensorflow_cc -ltensorflow_framework
}

windows {
    DESTDIR = $$PWD/../../../../Bin/Platform/Win/Lib.Qt
    LIBS += -L$$(TENSORFLOW_PATH)/bazel-bin/tensorflow -llibtensorflow_framework.dll -llibtensorflow.dll -llibtensorflow_cc.dll
}

INCLUDEPATH += $$(BOOST_PATH)/include

 LIBS += -L$$(BOOST_PATH)/lib -lboost_system \
  -lboost_system \
  -lboost_chrono \
  -lboost_thread \
  -lboost_program_options \
  -lboost_filesystem \
  -lboost_date_time \
  -lboost_timer

HEADERS += \
    ../../Core/Lib.h \
    ../../Core/Interface/ttfsession.h \
    ../../Core/Interface/ttfsessionSqDet.h \
    ../../Core/UTfComponent.h  \
    ../../Core/UTfClassifier.h  \
    ../../Core/UTfDetector.h  \
    ../../Core/UTfSqDetector.h  \
    ../../Core/UTrajectoriesSaver.h


SOURCES += \
    ../../Core/Lib.cpp \
    ../../Core/Interface/ttfsession.cpp \
    ../../Core/Interface/ttfsessionSqDet.cpp \
    ../../Core/UTfComponent.cpp \
    ../../Core/UTfClassifier.cpp \
    ../../Core/UTfDetector.cpp  \
    ../../Core/UTfSqDetector.cpp  \
    ../../Core/UTrajectoriesSaver.cpp

#including opencv
OPENCV_LIBS_LIST = -lopencv_core \
 -lopencv_highgui \
 -lopencv_imgproc \
 -lopencv_videoio \
 -lopencv_imgcodecs \
 -lopencv_video

windows {
 OPENCV_LIBS_VERSION = 345

contains(DEFINES, RDK_USE_CUDA) {
    OPENCV_COMPILED_VERSION_64 = vc15cuda
    OPENCV_COMPILED_VERSION_86 = vc15
} else {
    OPENCV_COMPILED_VERSION_64 = vc15
    OPENCV_COMPILED_VERSION_86 = vc15
}

 # функция добавляет постфикс(второй параметр) ко всем элементам первого входного параметра
 defineReplace(addPostfix) {
  libList = $$1
  for(lib, libList) {
   returnValue += $${lib}$${2}
  }
  return($$returnValue)
 }

 INCLUDEPATH += $$(OPENCV3_PATH)/build/include

 !contains(QMAKE_TARGET.arch, x86_64) {
 CONFIG(debug){
  LIBS += -L$$(OPENCV3_PATH)/build/x86/$${OPENCV_COMPILED_VERSION_86}/lib/Debug $$addPostfix($$OPENCV_LIBS_LIST, $${OPENCV_LIBS_VERSION}d)
 }
 CONFIG(release) {
  LIBS += -L$$(OPENCV3_PATH)/build/x86/$${OPENCV_COMPILED_VERSION_86}/lib/Release $$addPostfix($$OPENCV_LIBS_LIST, $${OPENCV_LIBS_VERSION})
 }
} else {
 CONFIG(debug){
  LIBS += -L$$(OPENCV3_PATH)/build/x64/$${OPENCV_COMPILED_VERSION_64}/lib/Debug $$addPostfix($$OPENCV_LIBS_LIST, $${OPENCV_LIBS_VERSION}d)
 }
 CONFIG(release) {
  LIBS += -L$$(OPENCV3_PATH)/build/x64/$${OPENCV_COMPILED_VERSION_64}/lib/Release $$addPostfix($$OPENCV_LIBS_LIST, $${OPENCV_LIBS_VERSION})
 }
}
} else:unix {
 LIBS += -L$$(OPENCV3_PATH)/lib $$OPENCV_LIBS_LIST
}

