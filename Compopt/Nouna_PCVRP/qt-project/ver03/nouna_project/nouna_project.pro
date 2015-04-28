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
    src/distmatrix.cpp \
    src/plot.cpp \
    src/readdata.cpp \
    src/initialsolution2.cpp \
    src/report.cpp \
    reportwindow.cpp \
    src/test.cpp

HEADERS  += mainwindow.h \
    qcustomplot/qcustomplot.h \
    hrd/datadef.h \
    hrd/distmatrix.h \
    hrd/household.h \
    hrd/plot.h \
    hrd/readdata.h \
    hrd/village.h \
    hrd/initialsolution2.h \
    hrd/interviewer.h \
    hrd/road.h \
    hrd/dijkstra.h \
    hrd/collectdata.h \
    hrd/statistic.h \
    hrd/const.h \
    hrd/report.h \
    reportwindow.h \
    hrd/test.h

FORMS    += mainwindow.ui \
    reportwindow.ui

CONFIG += c++11
