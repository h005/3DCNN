#-------------------------------------------------
#
# Project created by QtCreator 2017-05-03T14:19:30
#
#-------------------------------------------------

QT       += core gui widgets


#CONFIG += console
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Model2Video_vcglib
TEMPLATE = app

#QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11

INCLUDEPATH += ../../libs/vcglib-1.0.1 \
            ../../libs/vcglib-1.0.1/eigenlib


OTHER_FILES += shader/*.vert shader/*.frag


SOURCES += main.cpp\
#        mainwindow.cpp \
    meshmodel.cpp \
    meshmodelhandler.cpp \
    modellistmanager.cpp \
    render.cc \
    imagemanager.cpp \
    shader.cc \
    test.cpp


HEADERS  += meshmodel.h \
#    mainwindow.h \
    meshmodelhandler.h \
    modellistmanager.h \
    render.hh \
    imagemanager.h \
    shader.hh \
    test.h

# glew
LIBS += -lGLEW -lGLU -lGL

# glm
INCLUDEPATH += /usr/include/glm

# opencv
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2 \

LIBS += /usr/local/lib/libopencv_aruco.so \
        /usr/local/lib/libopencv_bgsegm.so \
        /usr/local/lib/libopencv_bioinspired.so \
        /usr/local/lib/libopencv_calib3d.so \
        /usr/local/lib/libopencv_ccalib.so \
        /usr/local/lib/libopencv_core.so \
        /usr/local/lib/libopencv_datasets.so \
        /usr/local/lib/libopencv_dnn.so \
        /usr/local/lib/libopencv_dpm.so \
        /usr/local/lib/libopencv_face.so \
        /usr/local/lib/libopencv_features2d.so \
        /usr/local/lib/libopencv_flann.so \
        /usr/local/lib/libopencv_fuzzy.so \
        /usr/local/lib/libopencv_hdf.so \
        /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_line_descriptor.so \
        /usr/local/lib/libopencv_ml.so \
        /usr/local/lib/libopencv_objdetect.so \
        /usr/local/lib/libopencv_optflow.so \
        /usr/local/lib/libopencv_photo.so \
        /usr/local/lib/libopencv_plot.so \
        /usr/local/lib/libopencv_reg.so \
        /usr/local/lib/libopencv_rgbd.so \
        /usr/local/lib/libopencv_saliency.so \
        /usr/local/lib/libopencv_sfm.so \
        /usr/local/lib/libopencv_shape.so \
        /usr/local/lib/libopencv_stereo.so \
        /usr/local/lib/libopencv_stitching.so \
        /usr/local/lib/libopencv_structured_light.so \
        /usr/local/lib/libopencv_superres.so \
        /usr/local/lib/libopencv_surface_matching.so \
        /usr/local/lib/libopencv_text.so \
        /usr/local/lib/libopencv_tracking.so \
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_video.so \
        /usr/local/lib/libopencv_videostab.so \
        /usr/local/lib/libopencv_viz.so \
        /usr/local/lib/libopencv_xfeatures2d.so \
        /usr/local/lib/libopencv_ximgproc.so \
        /usr/local/lib/libopencv_xobjdetect.so \
        /usr/local/lib/libopencv_xphoto.so

