#ifndef INITIALSOLUTION_H
#define INITIALSOLUTION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>      // std::pair
#include <math.h>
#include <time.h>
#include <assert.h>

#include "hrd/village.h"
#include "hrd/interviewer.h"
#include "hrd/road.h"
#include "hrd/household.h"
#include "hrd/statistic.h"
#include "hrd/const.h"


void setNInterviwer();

void initialsolution(std::vector<stVillage>,                // villages
                     std::vector<stHousehold> ,               // households
                     std::vector<stInterviewer>& ,             // interviewers
                     std::vector<std::vector<double> >,       // distmatrix
                     std::vector<std::vector<double> >,       // distmatrix (raining season)
                     std::vector<std::vector<std::pair<unsigned int, double> > >,
                     timeStatistic);

void planForADay (unsigned int,                      // day
                  std::vector<std::pair<int, bool> >& ToVis,
                  std::vector<std::vector<double> >,       // distmatrix
                  std::vector<stInterviewer>             // Interviewer
                  );
#endif // INITIALSOLUTION_H
