#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T12:31:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = triton
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

macx {
    QMAKE_CXXFLAGS += -stdlib=libc++
}

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
