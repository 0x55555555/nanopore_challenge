QT       -= core gui
TARGET = Common
TEMPLATE = lib
DESTDIR = ../bin

QMAKE_CXXFLAGS += -std=c++11 -stdlib=libc++

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
}

SOURCES += \
    SharedBuffer.cpp

HEADERS += \
    Generator.h \
    SharedBuffer.h

INCLUDEPATH += ../../boost_1_57_0