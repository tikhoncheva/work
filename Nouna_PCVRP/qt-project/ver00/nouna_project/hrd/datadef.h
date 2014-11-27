#ifndef DATADEF_H
#define DATADEF_H

#include <iostream>
#include <vector>
#include <utility>

#include "village.h"
#include "road.h"
#include "household.h"
#include "interviewer.h"



unsigned int nH;		// number of households
unsigned int nV;     // number of villages
unsigned int nR;     // number of routes
unsigned int K;		// number of interviewer

std::vector<stVillage> Village;
std::vector<stRoad> Road;
std::vector<stHousehold> Household;
std::vector<stInterviewer> Interviewer;

std::vector<std::vector<unsigned int> > adjmatrix;
std::vector<double> distmatrix;

#endif // DATADEF_H
