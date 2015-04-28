#ifndef DATADEF_H
#define DATADEF_H

#include <iostream>
#include <vector>
#include <utility>      // std::pair

#include "village.h"
#include "road.h"
#include "household.h"
#include "interviewer.h"
#include "statistic.h"


typedef std::vector<std::vector<unsigned int> > uintMatrix2D ;
typedef std::vector<uintMatrix2D> uintMatrix3D;

unsigned int nH;		// number of households
unsigned int nV;     // number of villages
unsigned int nR;     // number of routes
unsigned int nK;		// number of interviewer


std::vector<stVillage> Village;
std::vector<stRoad> Road;
std::vector<stHousehold> Household;
std::vector<stInterviewer> Interviewer;

std::vector<std::vector<uDist> > distmatrix;    // weighted adjacent matrix

std::vector<std::vector<double> > timematrixDry;    // length of the shortest ways
std::vector<std::vector<double> > timematrixRain;   // between all villages

uintMatrix2D predecessorsDry;    // shortest ways
uintMatrix2D predecessorsRain;   // between all villages

std::vector<std::vector<unsigned int> > village_households;    // list of the households in a village

std::vector<std::vector<std::pair<unsigned int, double> > > hhITimePlan_day;
std::vector<std::vector<std::pair<unsigned int, double> > > hhITimePlan_week;

timeStatistic TimeInfo;

#endif // DATADEF_H
