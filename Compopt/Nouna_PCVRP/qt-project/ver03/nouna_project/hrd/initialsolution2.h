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

//sort villages according to the angle they do have relative to the capital
std::vector<unsigned int> sort_villages_angles(std::vector<unsigned int> villageIDs,
                                               const std::vector<stVillage> villages);

// group households according to villages they belong to
std::vector<std::vector<unsigned int> > groupHH(std::vector<unsigned int> , //villagesID
                                                std::vector<unsigned int> ,  //householdsID
                                                const std::vector<stHousehold>);

// group households according to villages they belong to
std::vector<std::vector<unsigned int> > groupHH2(std::vector<unsigned int> , //villagesID
                                                std::vector<unsigned int> ,  //householdsID
                                                const std::vector<stHousehold>,
                                                std::vector <std::pair<unsigned int, double> >);   // TimePlan_week

// sort Villages (and households) in increasing order of the angle they make with the capital
std::vector<stHousehold> households_presorting(const std::vector<stVillage> ,
                                               const std::vector<stHousehold> ,
                                               std::vector<std::vector<unsigned int> > );

//------------------------------------------------------------------------------------------------//

// To each household assign a week of visiting and interview time
// Result : table week vs <hhIDs, itime> (TimePlan_week)
std::vector<std::vector<std::pair<unsigned int, double> > > planInterviews_weekly
                                                            (std::vector<stHousehold> , // households
                                                             timeStatistic );           // information about mean values

// Create day plans from week plans
// Result : for each week daily routes (ITimePlan_daily)
std::vector<std::vector<stRoute> > createDailyPlansFromWeeklyPlans(unsigned int nI, // number of interviews
                                   std::vector<stHousehold> ,  // _households
                                   std::vector<std::vector<std::pair<unsigned int, double> > >&, // TimePlan_week
                                   const std::vector<std::vector<double> > ,  // distmatrix
                                   const std::vector<std::vector<double> > ); // distmatrix

// Assign created daily plant to available interviewers
void assignDailyPlansToInterviewers(std::vector<stInterviewer>& ,  // interviewers
                                    const std::vector<std::vector<stRoute> > ); // ITimePlan_daily

// Make week routes for each interviewer by collecting information from daily routes
void makeWeekRoutes(std::vector<stInterviewer>& );      // interviewers

// allow to stay over a night
void stay_over_night(std::vector<stInterviewer>& _interviewer,           // interviewers
                     std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                     std::vector<std::vector<double> >  _distmatrixRain); // distmatrix

// Initial Solution
int initialsolution2(std::vector<stVillage> ,           // villages
                     std::vector<stHousehold> ,         // _households
                     std::vector<stInterviewer>& ,      // _interviewer
                     std::vector<std::vector<double> > , // distmatrix
                     std::vector<std::vector<double> > , // distmatrix
                     std::vector<std::vector<unsigned int> > ,  // grouped households
                     timeStatistic,
                     std::vector<std::vector<std::pair<unsigned int, double> > > &); // weekly interview plan for hh

#endif // INITIALSOLUTION2_H
