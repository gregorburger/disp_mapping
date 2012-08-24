#-------------------------------------------------
#
# Project created by QtCreator 2012-08-24T10:07:10
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = disp_mapping
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    viewer.cpp

HEADERS  += mainwindow.h \
    viewer.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -lQGLViewer -lGLU -lGLEW

OTHER_FILES += \
    displacement.vsh
