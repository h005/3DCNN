QT += core
QT += gui
QT += widgets

CONFIG += c++11

TARGET = Model2Video
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += _USE_MATH_DEFINES GLM_FORCE_RADIANS

OTHER_FILES += shader/*.vert shader/*.frag

SOURCES += main.cpp \
    handler.cpp \
    reverseface.cpp \
    render.cc \
    shader.cc \
    trackball.cpp \
    imagemanager.cpp \
    modellistmanager.cpp

HEADERS += \
    handler.h \
    externalimporter.hh \
    common.hh \
    reverseface.h \
    render.hh \
    meshglhelper.hh \
    shader.hh \
    trackball.hh \
    gausscurvature.hh \
    meancurvature.hh \
    abstractfeature.hh \
    colormap.hh \
    curvature.hh \
    imagemanager.h \
    modellistmanager.h

# OpenMesh
INCLUDEPATH += /usr/local/include
LIBS += -lOpenMeshCore

# assimp
INCLUDEPATH += /usr/local/include/assimp
LIBS += -L/usr/local/lib/ -lassimp

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


