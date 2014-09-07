#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T03:42:39
#
#-------------------------------------------------

QT       += core gui webkitwidgets xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VKAudio
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
    model.cpp \
    vkaudio.cpp

HEADERS  += widget.h \
    observer.h \
    model.h \
    vkaudio.h

FORMS += \
    vkaudio.ui

