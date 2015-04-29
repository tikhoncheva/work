#ifndef INITIALSOLUTION1_H
#define INITIALSOLUTION1_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>      // std::pair
#include <math.h>
#include <time.h>
#include <assert.h>

//#include "hrd/datadef.h"
#include "hrd/village.h"
#include "hrd/interviewer.h"
#include "hrd/road.h"
#include "hrd/household.h"
//#include "hrd/statistic.h"
#include "hrd/const.h"

namespace approach1
{
    // returns position on the minimum positive value in the container
    template <class ForwardIterator>
    ForwardIterator find_positivemin( ForwardIterator, ForwardIterator);


    //// sort villages according to their distance to capital
    //std::vector<unsigned int> sort_villages_dist(std::vector<unsigned int> ,    // villages
    //                                             const std::vector<std::vector<double> > ); // distance

    ////sort villages according to the angle they do have relative to the capital
    //std::vector<unsigned int> sort_villages_angles(std::vector<unsigned int> villageIDs,
    //                                               const std::vector<stVillage> villages);

    //// group households according to villages they belong to
    //std::vector<std::vector<unsigned int> > groupHH2(std::vector<unsigned int> , //villagesID
    //                                                std::vector<unsigned int> ,  //householdsID
    //                                                const std::vector<stHousehold>,
    //                                                std::vector <std::pair<unsigned int, double> >);   // TimePlan_week

    // sort Villages (and households) in increasing order of the angle they make with the capital
    std::vector<stHousehold> households_presorting(const std::vector<stVillage> ,
                                                   const std::vector<stHousehold> ,
                                                   std::vector<std::vector<unsigned int> > );

    //------------------------------------------------------------------------------------------------//

    // To each household assign a week of visiting and interview time
    // Result : table week vs <hhIDs, itime> (TimePlan_week)
    std::vector<std::vector<std::pair<unsigned int, double> > > assign_hh_to_weeks
                                                                (std::vector<stHousehold> ); // households

    // Create day plans from week plans
    // Result : for each week daily routes (ITimePlan_daily)
    std::vector<std::vector<stRoute> > create_daily_routes(unsigned int nI, // number of interviews
                                       std::vector<stHousehold> ,  // _households
                                       std::vector<std::vector<std::pair<unsigned int, double> > >&, // TimePlan_week
                                       const std::vector<std::vector<double> > ,  // timematrix
                                       const std::vector<std::vector<double> > ); // timematrix

    // Assign created daily plant to available interviewers
    void assign_dailyRoutes_to_interviewer(std::vector<stInterviewer>& ,  // interviewers
                                        const std::vector<std::vector<stRoute> > ); // ITimePlan_daily

    // Make week routes for each interviewer by collecting information from daily routes
    void make_weekRoutes(std::vector<stInterviewer>& );      // interviewers

    // allow to stay over a night
    void stay_over_night(std::vector<stInterviewer>& _interviewer,           // interviewers
                         std::vector<std::vector<double> >  _timematrixDry,  // timematrix
                         std::vector<std::vector<double> >  _timematrixRain); // timematrix

    // Initial Solution
    int initialsolution1(std::vector<stVillage> ,           // villages
                         std::vector<stHousehold> ,         // _households
                         std::vector<stInterviewer>& ,      // _interviewer
                         std::vector<std::vector<double> > , // timematrix
                         std::vector<std::vector<double> > , // timematrix
                         std::vector<std::vector<unsigned int> > ,  // grouped households
                         std::vector<std::vector<std::pair<unsigned int, double> > > &); // weekly interview plan for hh

} //end namespace approach1
#endif // INITIALSOLUTION2_H
