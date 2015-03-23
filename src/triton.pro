#-------------------------------------------------
#
# Project created by QtCreator 2015-03-23T12:31:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Triton
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

macx {
    QMAKE_CXXFLAGS += -stdlib=libc++
}

SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cpp \
    iocontroller.cpp \
    spline.cpp \
    splinegroup.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    iocontroller.h \
    spline.h \
    splinegroup.h \
    canvas_actions.h

FORMS    += mainwindow.ui
