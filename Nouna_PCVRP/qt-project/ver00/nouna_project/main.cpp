#include <iostream>

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(0));

    QProcess::execute("clear");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
