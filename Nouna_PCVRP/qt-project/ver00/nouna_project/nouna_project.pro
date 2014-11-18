#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T10:42:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = nouna_project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    readdata.cpp \
    add_func.cpp \
    qcustomplot/qcustomplot.cpp \
    distmatrix.cpp \
    plot.cpp

HEADERS  += mainwindow.h \
    readdata.h \
    datadef.h \
    add_func.h \
    village.h \
    route.h \
    household.h \
    qcustomplot/qcustomplot.h \
    distmatrix.h \
    plot.h

FORMS    += mainwindow.ui
