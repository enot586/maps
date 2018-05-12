QT += core
QT -= gui

TARGET = omap
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14

SOURCES += main.cpp

HEADERS += \
    \usr\include\boost\
    test.hpp \
    map3.hpp \
    map2.hpp \
    map1.hpp


