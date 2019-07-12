QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

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


INCLUDEPATH += $$(TENSORFLOW_PATH)
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow3/external/eigen_archive
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-tensorflow3/external/protobuf_archive/src
INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-genfiles

INCLUDEPATH += $$(ABSEIL_PATH)

unix:!macx: LIBS += -L $$(TENSORFLOW_PATH)/bazel-bin/tensorflow/ -ltensorflow_cc

INCLUDEPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow
DEPENDPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow

unix:!macx: LIBS += -L $$(TENSORFLOW_PATH)/bazel-bin/tensorflow/ -ltensorflow_framework

INCLUDEPATH += $$(TENSORFLOW_PATH)
DEPENDPATH += $$(TENSORFLOW_PATH)/bazel-bin/tensorflow

HEADERS += \
    ../../Core/Interface/label_image.h \
    ../../Core/Interface/ttfsession.h

#including opencv
OPENCV_LIBS_LIST = -lopencv_core \
 -lopencv_highgui \
 -lopencv_imgproc \
 -lopencv_videoio \
 -lopencv_imgcodecs \
 -lopencv_video

 LIBS += -L$$(OPENCV3_PATH)/lib $$OPENCV_LIBS_LIST
 INCLUDEPATH += $$(OPENCV3_PATH)/include

