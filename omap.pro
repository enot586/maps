QT += core
QT -= gui

TARGET = omap
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

HEADERS += \
    test.hpp \
    map3.hpp \
    map1.hpp \


