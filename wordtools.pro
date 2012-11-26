#-------------------------------------------------
#
# Project created by QtCreator 2012-11-23T22:43:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = wordtools
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp dawg.cpp \
    reverseanagrammer.cpp \
    utilities.cpp
HEADERS += dawg.h \
    reverseanagrammer.h \
    utilities.h
