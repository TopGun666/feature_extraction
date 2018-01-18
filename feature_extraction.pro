TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += /usr/local/Cellar/opencv/3.4.0/include/

LIBS = /usr/local/lib/libopencv_core.3.4.0.dylib
LIBS = /usr/local/lib/libopencv_features2d.3.4.0.dylib
LIBS = /usr/local/lib/libopencv_highgui.3.4.0.dylib
