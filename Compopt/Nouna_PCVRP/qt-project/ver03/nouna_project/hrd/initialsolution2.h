#ifndef INITIALSOLUTION2_H
#define INITIALSOLUTION2_H

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

// returns position on the minimum positive value in the container
template <class ForwardIterator>
ForwardIterator find_posmin( ForwardIterator, ForwardIterator);


// sort villages according to their distance to capital
std::vector<unsigned int> sort_villages_dist(std::vector<unsigned int> ,    // villages
                                             const std::vector<std::vector<double> > ); // distance

// group households according to villages they belong to
std::vector<std::vector<unsigned int> > groupHH(std::vector<unsigned int> , //villagesID
                                                std::vector<unsigned int> ,  //householdsID
                                                const std::vector<stHousehold>);

// sort Villages (and households) in increasing order of the angle they make with the capital
std::vector<stHousehold> households_presorting(const std::vector<stVillage> ,
                                               const std::vector<stHousehold> ,
                                               std::vector<std::vector<unsigned int> > );

//------------------------------------------------------------------------------------------------

// Plan interview times of hhs in the year: day |->   hhIDs to visit
std::vector<std::vector<std::pair<unsigned int, double> > > planInterviews_weekly(std::vector<stHousehold> ,
                                                                                   // number of interviewers
                                                        timeStatistic );           // information about mean values

// Given a year plan of household, assign interviewer to each hh
void assignInterviewersToHH(const std::vector<std::vector<std::pair<unsigned int, double> > > , // year plan of itimes
                            std::vector<stHousehold> ,         // _households
                            std::vector<stInterviewer>& ,      // interviewer
                            const std::vector<std::vector<double> >  , // distmatrix
                            const std::vector<std::vector<double> >  );// distmatrix


// Make week plans from the given day plans
void make_week_plans(std::vector<stInterviewer>&);      // interviewer

// Initial Solution
void initialsolution2(std::vector<stVillage> ,           // villages
                     std::vector<stHousehold> ,         // _households
                     std::vector<stInterviewer>& ,      // _interviewer
                     std::vector<std::vector<double> > , // distmatrix
                     std::vector<std::vector<double> > , // distmatrix
                     std::vector<std::vector<unsigned int> > ,  // grouped households
                     timeStatistic,
                     std::vector<std::vector<std::pair<unsigned int, double> > > &); // weekly interview plan for hh

void planForADay (unsigned int,                      // day
                  std::vector<std::pair<int, bool> >& ToVis,
                  std::vector<std::vector<double> >,       // distmatrix
                  std::vector<stInterviewer>             // Interviewer
                  );
#endif // INITIALSOLUTION2_H
