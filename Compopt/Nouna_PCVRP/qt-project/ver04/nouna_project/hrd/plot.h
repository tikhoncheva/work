#ifndef PLOT_H
#define PLOT_H

#include <QColor>
#include <QVector>

#include <iostream>
#include <vector>
#include <assert.h>

#include <qcustomplot/qcustomplot.h>
#include <hrd/village.h>
#include <hrd/road.h>
#include <hrd/interviewer.h>


extern QVector<double> highlightx;
extern QVector<double> highlighty;

void plot_villages(QCustomPlot*, std::vector<stVillage>);

void plot_labelsVillages(QCustomPlot*, std::vector<stVillage>);
void plot_IDsVillages(QCustomPlot*, std::vector<stVillage>);

void plot_roads(QCustomPlot*, std::vector<stVillage>, std::vector<stRoad>, std::vector<std::vector<uDist> >);

void plot_route_week(QCustomPlot*, const std::vector<stVillage>, stInterviewer, unsigned int,
                const std::vector<std::vector<unsigned int> >);
void plot_route_day(QCustomPlot*, const std::vector<stVillage>, stInterviewer, unsigned int,
                const std::vector<std::vector<unsigned int> >);
void plot_routes(QCustomPlot*, std::vector<stVillage>, stInterviewer,
                const std::vector<std::vector<unsigned int> > );

void delete_labelsAndRoads(QCustomPlot*);

#endif // PLOT_H
