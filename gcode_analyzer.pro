QT += core
QT -= gui

CONFIG += c++11

TARGET = gcode_analyzer
CONFIG += console
CONFIG -= app_bundle

LIBS += -L/opt/poky/1.7.3/sysroots/core2-32-poky-linux/usr/lib
TEMPLATE = app

SOURCES += main.cpp \
    gcode_info.cpp

HEADERS += \
    gcode_info.h
