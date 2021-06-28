QT += core
QT += gui

CONFIG += c++11

TARGET = TCPServer
CONFIG -= console
CONFIG -= app_bundle
LIBS += -lboost_system -lboost_thread

TEMPLATE = app

SOURCES += main.cpp \
    tcpserver.cpp \
    tcpclient.cpp \
    circular_buffer.cpp

HEADERS += \
    tcpserver.h \
    tcpclient.h \
    circular_buffer.h \
    buffer_base.h \


