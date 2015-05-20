#include <iostream>

#include "mainwindow.h"
#include "hrd/const.h"
#include <QApplication>

// !!!!
unsigned int constant::nInterviewers = 20;
// !!!!

int unsigned constant::shortITime = 10; // 10 min
int unsigned constant::longITime = 25;  // 25 min

int unsigned constant::P = 3;           // 3 periods
int unsigned constant::nweeks = 16;     // each period contains 16 weeks

unsigned int constant::nLongInterviews = 0;   // number of Households with the type 1
unsigned int constant::nShortInterviews = 0;  // number of Households with the type 0
double constant::summaryLongITime = 0.0;        // summary long interview times in year

double constant::maxTimeDistDry = 0.0;        // maximal travel time in dry season
double constant::maxTimeDistRain = 0.0;        // maximal travel time in rain season

int main(int argc, char *argv[])
{
    srand(time(0));

    QProcess::execute("clear");

    QApplication a(argc, argv);
    MainWindow w;

    w.showMinimized();



    return a.exec();
}
