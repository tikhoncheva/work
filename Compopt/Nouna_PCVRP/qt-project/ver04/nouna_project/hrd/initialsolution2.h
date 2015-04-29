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


//// sort villages according to their distance to capital
//std::vector<unsigned int> sort_villages_dist(std::vector<unsigned int> ,    // villages
//                                             const std::vector<std::vector<double> > ); // distance


// sort households in increasing order of the angle they make with the capital
std::vector<unsigned int> sort_villages_angles(std::vector<unsigned int> ,      // village IDs
                                               const std::vector<stVillage>); // villages

// group households according to villages they belong to
std::vector<std::vector<unsigned int> > groupHH2(std::vector<unsigned int> , //villagesID
                                                std::vector<unsigned int> ,  //householdsID
                                                const std::vector<stHousehold>,
                                                std::vector <std::pair<unsigned int, double> >);   // TimePlan_week

// sort Villages (and households) in increasing order of the angle they make with the capital
std::vector<stHousehold> households_presorting(const std::vector<stVillage> ,
                                               const std::vector<stHousehold> ,
                                               std::vector<std::vector<unsigned int> > );

//------------------------------------------------------------------------------------------------

// Plan interview times of hhs in the year: day |->   hhIDs to visit
std::vector<std::vector<std::pair<unsigned int, double> > > planInterviews_weekly(
                                                        std::vector<stHousehold> , //  sorted households
                                                        unsigned int );             // number of interviewers

// Given a year plan of household, assign interviewer to each hh
void assignInterviewersToHH(const std::vector<std::vector<std::pair<unsigned int, double> > > , // year plan of itimes
                            std::vector<stHousehold> ,         // _households
                            std::vector<stInterviewer>& ,      // interviewer
                            const std::vector<std::vector<double> >  , // timematrix
                            const std::vector<std::vector<double> >  );// timematrix


// Make week plans from the given day plans
void make_week_plans(std::vector<stInterviewer>&);      // interviewer

void planForADay (unsigned int,                      // day
                  std::vector<std::pair<int, bool> >& ToVis,
                  std::vector<std::vector<double> >,       // timematrix
                  std::vector<stInterviewer>             // Interviewer
                  );

// allow to stay over a night
void stay_over_night(std::vector<stInterviewer>& _interviewer,           // interviewers
                     std::vector<std::vector<double> >  _timematrixDry,  // timematrix
                     std::vector<std::vector<double> >  _timematrixRain); // timematrix


// Initial Solution
int initialsolution2(std::vector<stVillage> ,           // villages
                     std::vector<stHousehold> ,         // _households
                     std::vector<stInterviewer>& ,      // _interviewer
                     std::vector<std::vector<double> > , // timematrix
                     std::vector<std::vector<double> > , // timematrix
                     std::vector<std::vector<unsigned int> > ,  // grouped households
                     std::vector<std::vector<std::pair<unsigned int, double> > > &); // weekly interview plan for hh


#endif // INITIALSOLUTION2_H
