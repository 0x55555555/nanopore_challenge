QT       -= core gui

TARGET = RandomGenerator
CONFIG   += console
CONFIG   -= app_bundle
DESTDIR = ../bin

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../../hdf5/include ../../boost_1_57_0 ../Common

LIBS += -L../../hdf5/lib -lhdf5_hl_cpp -lhdf5_cpp -lhdf5_hl -lhdf5 -L../bin -lCommon

