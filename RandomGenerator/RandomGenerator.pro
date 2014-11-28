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

INCLUDEPATH +=  ../../boost_1_57_0 ../Common

LIBS += -L../bin -lCommon

