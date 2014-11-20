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
    qcustomplot/qcustomplot.cpp \
    src/add_func.cpp \
    src/distmatrix.cpp \
    src/plot.cpp \
    src/readdata.cpp

HEADERS  += mainwindow.h \
    readdata.h \
    datadef.h \
    add_func.h \
    village.h \
    route.h \
    household.h \
    qcustomplot/qcustomplot.h \
    distmatrix.h \
    plot.h \
    hrd/add_func.h \
    hrd/datadef.h \
    hrd/distmatrix.h \
    hrd/household.h \
    hrd/plot.h \
    hrd/readdata.h \
    hrd/route.h \
    hrd/village.h

FORMS    += mainwindow.ui
