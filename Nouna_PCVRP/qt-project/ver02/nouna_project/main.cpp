#include <iostream>

#include "mainwindow.h"
#include "hrd/const.h"
#include <QApplication>

int unsigned constant::shortITime = 10; // 10 min
int unsigned constant::longITime = 25;  // 25 min

int unsigned constant::P = 3;           // 3 periods
int unsigned constant::nweeks = 16;     // each period contains 16 weeks

int main(int argc, char *argv[])
{
    srand(time(0));

    QProcess::execute("clear");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
