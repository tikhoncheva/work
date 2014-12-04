#ifndef PLOT_H
#define PLOT_H

#include <QColor>
#include <iostream>
#include <vector>
#include <qcustomplot/qcustomplot.h>
#include <hrd/village.h>
#include <hrd/road.h>


void plot_villages(QCustomPlot*, std::vector<stVillage>);

void plot_labelsVillages(QCustomPlot*, std::vector<stVillage>);

void plot_roads(QCustomPlot*, std::vector<stVillage>, std::vector<stRoad>, std::vector<std::vector<unsigned int> >, bool);

void delete_labelsAndRoads(QCustomPlot*);

#endif // PLOT_H
