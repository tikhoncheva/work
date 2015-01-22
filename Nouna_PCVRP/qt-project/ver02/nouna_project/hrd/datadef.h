#ifndef DATADEF_H
#define DATADEF_H

#include <iostream>
#include <vector>
#include <utility>

#include "village.h"
#include "road.h"
#include "household.h"
#include "interviewer.h"
#include <utility>      // std::pair

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
std::vector<stInterviewer> Cleaner;

std::vector<std::vector<uDist> > distmatrix;    // weighted adjacent matrix

std::vector<std::vector<double> > timematrixDry;    // length of the shortest ways
std::vector<std::vector<double> > timematrixRain;   // between all villages

uintMatrix2D predecessorsDry;    // shortest ways
uintMatrix2D predecessorsRain;   // between all villages

std::vector<std::vector<std::pair<double, unsigned int> > > village_household;    // list of the households in a village



struct timeStatistic
{
    unsigned int nLongInterviews;   // number of Households with the type 1
    unsigned int nShortInterviews;  // number of Households with the type 0

    double workTimeYear;            // summary working time in year
    double sShortITime;             // summary short interview times in year
    double sLongITime1;              // summary long interview times in year
    double sLongITime23;              // summary long interview times in year
} TimeInfo;

#endif // DATADEF_H
