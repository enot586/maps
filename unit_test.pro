TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += /usr/include/boost
INCLUDEPATH += /usr/include/boost/test/

LIBS += -lboost_test_exec_monitor

SOURCES += \
    unit_test.cpp \

