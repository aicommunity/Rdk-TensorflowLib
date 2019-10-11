QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += NOMINMAX

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        ../../Core/Interface/ttfsession.cpp

# Default rules for deployment.
target.path = $$PWD/../../Bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-genfiles
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/eigen_archive
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/protobuf_archive/src
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/com_google_protobuf/src
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow/external/com_google_absl

unix:!macx: LIBS += -L$$(TENSORFLOW_PATH)/bazel-bin/tensorflow -ltensorflow_cc

INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow
DEPENDPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow

unix:!macx: LIBS += -L$$(TENSORFLOW_PATH)/bazel-bin/tensorflow -ltensorflow_framework

windows {
LIBS += -L$$(TENSORFLOW_PATH)/bazel-bin/tensorflow -llibtensorflow_framework.dll -llibtensorflow.dll -llibtensorflow_cc.dll
}

INCLUDEPATH += $$(TENSORFLOW_PATH)
DEPENDPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow

HEADERS += \
    ../../Core/Interface/ttfsession.h \
    ../../Core/Interface/label_image.h

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

