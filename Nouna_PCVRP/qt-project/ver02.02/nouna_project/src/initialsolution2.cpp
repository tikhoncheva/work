#include "hrd/initialsolution2.h"


/*
 * returns position on the minimum positive value in the container
 */
template <class ForwardIterator>
ForwardIterator find_posmin( ForwardIterator first, ForwardIterator last )
{
    if (first==last)
        return last;

    ForwardIterator smallest = first;

    while (++first!=last)
    {
        if (*first>=0 && (*first<*smallest || *smallest<0))
        {
            smallest=first;
        }
    }

    if (*smallest<0)
        return last;
    else
        return smallest;
}

//------------------------------------------------------------------------------------------------
/*
 * sort villages according to their distance to capital
 */
/*
std::vector<unsigned int> sort_villages_dist(std::vector<unsigned int> villages,
                                             const std::vector<std::vector<double> > dist)
{
    std::vector<unsigned int> villages_sorted;
    const unsigned int home = 142 - 101;
    const unsigned int N = villages.size();
    std::vector<std::pair<double, unsigned int>> distToHome;

    for (unsigned int i=0; i<N; ++i)
        distToHome.push_back(std::make_pair(dist[home][villages[i]],villages[i]));

    std::sort(distToHome.begin(), distToHome.end(), []( const std::pair<double, int>& a, const std::pair<double, int>& b)
                                                { return a.first > b.first;});
    for (unsigned int i=0; i<N; ++i)
       villages_sorted.push_back(distToHome[i].second);

    return villages_sorted;

}
*/

/*
 * sort households in increasing order of the angle they make with the capital
 */
std::vector<unsigned int> sort_villages_angles(std::vector<unsigned int> villageIDs,
                                               const std::vector<stVillage> villages)
{
    const unsigned int N = villageIDs.size();
    unsigned int vID;

    const unsigned int Nouna = 142-100-1; // Capital (Nouna)
    double x0 = villages[Nouna].coord.first;
    double y0 = villages[Nouna].coord.second;

    std::vector<unsigned int> villages_sorted;
    std::vector<std::pair<double, int> > angles(N, std::make_pair(0.,0));

    for (unsigned int i=0; i<N; ++i)
    {
        vID = villageIDs[i];
        double x = villages[vID].coord.first - x0;
        double y = villages[vID].coord.second - y0;
        angles[i] = std::make_pair(atan2(y,x), vID);
    }
    std::sort(angles.begin(), angles.end(), []( const std::pair<double, int>& a, const std::pair<double, int>& b)
    { return a.first < b.first;});

    for (unsigned int i=0; i<N; ++i)
        villages_sorted.push_back(angles[i].second);

    return villages_sorted;

}
//------------------------------------------------------------------------------------------------
/*
 * group households according to villages they belong to
 * return matrix instead of two arrays
 */
/*
std::vector<std::vector<unsigned int> > groupHH(std::vector<unsigned int> villagesID,
                                                       std::vector<unsigned int> householdsID,
                                                       const std::vector<stHousehold> _households)
{
    unsigned int V = villagesID.size();
    unsigned int H = householdsID.size();
    std::vector<std::vector<unsigned int> > _village_household(V);

    unsigned int vID;   // village's ID

    // for each village find it's hh
    for (unsigned int i=0; i<V; ++i)    // for each village
    {
        vID =  villagesID[i] + 101;

        for (unsigned int j=0; j< H; ++j)
        {
            // find household in this village i ;
            if (_households[householdsID[j]].villageID==vID)
                _village_household[i].push_back(householdsID[j]);
        }
    }

    return _village_household;
}
*/
//------------------------------------------------------------------------------------------------
/*
 * sort Villages (and households) in increasing order of the angle they make with the capital
 */
/*
std::vector<stHousehold> households_presorting(const std::vector<stVillage> villages,
                                                const std::vector<stHousehold> households,
                                                std::vector<std::vector<unsigned int> > _village_household)
{
    unsigned int V = villages.size();
    const unsigned int Nouna = 142-100-1; // Capital (Nouna)
    unsigned int vID;
    // set Nouna to the map center
    double x0 = villages[Nouna].coord.first;
    double y0 = villages[Nouna].coord.second;

    std::vector<std::pair<double,int> > angles(V, std::make_pair(0.,0));
    std::vector<stHousehold> households_sorted;
    unsigned int hhID;


    for (unsigned int i=0; i<V; ++i)
    {
        double x = villages[i].coord.first - x0;
        double y = villages[i].coord.second - y0;
        angles[i] = std::make_pair(atan2(y,x),i);
    }
    std::sort(angles.begin(), angles.end(), []( const std::pair<double, int>& a, const std::pair<double, int>& b)
                                                { return a.first < b.first;});

    // sort housesholds in the order of villages
    for (unsigned int i=0; i<V; ++i)    // for each village
    {
        vID = angles[i].second;
        for (unsigned int j=0; j < _village_household[vID].size(); ++j)
        {
            hhID = _village_household[vID][j];
            households_sorted.insert(households_sorted.begin(), households[hhID]);
        }
    }

    return households_sorted;
}
*/
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------


/*
 * Plan intervie times of each hh for each week of an year: week |->   hhIDs to visit
 * Each week should contain roughly same amount (in min) of long interviews
 */
std::vector<std::vector<std::pair<unsigned int, double> > > planInterviews_weekly(
        std::vector<stHousehold> _households_sorted, // _households
        unsigned int K,                              // number of interviewers
        timeStatistic _TimeInfo)                     // information about mean values
{
    /*
     * Step 1: set interview times for long interviewes in each period, so that
     * summary interview time of each period is roughly equivalent
     */

    std::cout << "Step 1" << std::endl;

    unsigned int nLongInterviews;

    double pmeanLongITime = _TimeInfo.sLongITime1 / double(constant::P);
    double wmeanLongITime = _TimeInfo.sLongITime1 / double(constant::P * constant::nweeks);
    double sumITimePeriod = 0.;

    unsigned int p = 0;  // period
    unsigned int w = 0;  // week

    unsigned int nWeeks = constant::nweeks * constant::P;

    double itime;
    unsigned int hhID;

    //    std::vector<bool> ToVis(N, true);
    std::vector<std::vector <std::pair<int, double> > > planLongITimes(3); // interview time of long interviews in each period
    std::vector<unsigned int > indShortInterviews;
    std::cout << "Mean long IT in period " << pmeanLongITime << std::endl;

    for (unsigned int i=0; i<_households_sorted.size(); ++i)
    {
        if (_households_sorted[i].type == 1)    // only long interviews
        {
            hhID = _households_sorted[i].ID - 10001;// rand()%(N-1);

            //        while (!ToVis[hhID])
            //            hhID= (hhID+1)%N;

            planLongITimes[0].push_back(std::make_pair(hhID, constant::shortITime));
            planLongITimes[1].push_back(std::make_pair(hhID, constant::shortITime));
            planLongITimes[2].push_back(std::make_pair(hhID, constant::shortITime));

            if (sumITimePeriod + _households_sorted[i].itime > pmeanLongITime)
            {
                if (p != std::min(p+1, unsigned(2)))
                    sumITimePeriod = 0.;
                p = std::min(int(p+1), 2);
            }
            planLongITimes[p].back() = std::make_pair(hhID, _households_sorted[i].itime);
            sumITimePeriod += _households_sorted[i].itime;
        }
        else
            indShortInterviews.push_back(i);
        //        ToVis[hhID] = false;
    }

    nLongInterviews = _TimeInfo.nLongInterviews;                        // correct number of long interviews
    assert(planLongITimes[0].size() == nLongInterviews);
    assert(planLongITimes[1].size() == nLongInterviews);
    assert(planLongITimes[2].size() == nLongInterviews);

    assert(indShortInterviews.size() == _TimeInfo.nShortInterviews);    // correct number of short interviews

    //-------------------------------------------------------------------------------------------

    /*
     * Step 2: Set for each long interviews a week, it will be made at
     *         Each week should contain roughly same amount (in min) of long interviews
     */

    std::cout << "Step 2" << std::endl;
    std::vector<unsigned int> weeks(nLongInterviews);
    // week plan should be the same in each of three periods

    std::vector< double > remainingTime (constant::nweeks, wmeanLongITime);
    std::vector< double > remainingTime_new(constant::nweeks, 0.);


    std::vector<double> worktime_week1 (constant::nweeks, 0.);
    std::vector<double> worktime_week2 (constant::nweeks, 0.);
    std::vector<double> worktime_week3 (constant::nweeks, 0.);

    //----------------------------------------------------------------------------------
    // assign long interview from the first period
    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        hhID = planLongITimes[0][i].first;// rand()%(N-1);
        //        while (!ToVis[hhID])
        //            hhID= (hhID+1)%N;
        itime = planLongITimes[0][i].second;

        if (itime > constant::shortITime /*10*/)
        {
            // for each week calculate the remaining time, if hh will be assign to this week
            for (unsigned int week=0; week<constant::nweeks; ++week)
                remainingTime_new[week] = remainingTime[week] - itime;

            // find min remaining time over all positive values
            w = find_posmin(remainingTime_new.begin(), remainingTime_new.end())
                    - remainingTime_new.begin();

            w = std::min(w, constant::nweeks-1); // it can happend that last week in period
            // is a little bit overloaded with long interviews
            weeks[i] = w;    // assign interviewer to the week
            remainingTime[w] = remainingTime_new[w];
        }
        //ToVis[hhID] = false;
    }
    std::fill(remainingTime.begin(), remainingTime.end(), wmeanLongITime);

    //----------------------------------------------------------------------------------
    // assign long interview from the second period
    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        hhID = planLongITimes[1][i].first;// rand()%(N-1);
        //        while (!ToVis[hhID])
        //            hhID= (hhID+1)%N;
        itime = planLongITimes[1][i].second;

        if (itime > constant::shortITime /*10*/)
        {
            // for each week calculate the remaining time, if hh will be assign to this week
            for (unsigned int week=0; week<constant::nweeks; ++week)
                remainingTime_new[week] = remainingTime[week] - itime;

            // find min remaining time over all positive values
            w = find_posmin(remainingTime_new.begin(), remainingTime_new.end())
                    - remainingTime_new.begin();

            w = std::min(w, constant::nweeks-1); // it can happend that last week in period
            // is a little bit overloaded with long interviews
            weeks[i] = w;    // assign interviewer to the week
            remainingTime[w] = remainingTime_new[w];
        }
        //ToVis[hhID] = false;
    }
    std::fill(remainingTime.begin(), remainingTime.end(), wmeanLongITime);

    //----------------------------------------------------------------------------------
    // assign long interview from the third period
    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        hhID = planLongITimes[2][i].first;// rand()%(N-1);
        //        while (!ToVis[hhID])
        //            hhID= (hhID+1)%N;
        itime = planLongITimes[2][i].second;

        if (itime > constant::shortITime /*10*/)
        {

            // for each week calculate the remaining time, if hh will be assign to this week
            for (unsigned int week=0; week<constant::nweeks; ++week)
                remainingTime_new[week] = remainingTime[week] - itime;

            // find min remaining time over all positive values
            w = find_posmin(remainingTime_new.begin(), remainingTime_new.end())
                    - remainingTime_new.begin();

            w = std::min(w, constant::nweeks-1); // it can happend that last week in period
            // is a little bit overloaded with long interviews
            weeks[i] = w;    // assign interviewer to the week
            remainingTime[w] = remainingTime_new[w];
        }
        //ToVis[hhID] = false;
    }

    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        w = weeks[i];
        worktime_week1[w] += planLongITimes[0][i].second;
        worktime_week2[w] += planLongITimes[1][i].second;
        worktime_week3[w] += planLongITimes[2][i].second;
    }

    std::cout << "mean long it in week " << wmeanLongITime << std::endl;
    std::cout << "week \t period1 \t period2 \t period3" << std::endl;
    for (unsigned int i= 0; i< constant::nweeks; ++i)
    {
        std::cout << i+1 << "\t" << worktime_week1[i] << "\t"
                  << worktime_week2[i]<< "\t"
                  << worktime_week3[i]<< std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;

    //-------------------------------------------------------------------------------------------

    /*
    * Step 3: Combine results from two previous steps in one matrix of the form: week| HH/itime
    */


    std::cout << "Step 3" << std::endl;
    const double tmax = K * 5* 8*60.;
    std::cout << "Max week work time " << tmax << std::endl;
    std::vector<double> remaining_time (nWeeks, tmax);

    std::vector<std::vector<std::pair<unsigned int, double>>> interviews_weekly;
    interviews_weekly.resize(nWeeks);

    double sum = 0;

    // first go through long interviews
    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        w = weeks[i];

        assert(planLongITimes[0][i].first == planLongITimes[1][i].first);
        assert(planLongITimes[0][i].first == planLongITimes[2][i].first);

        // first period
        interviews_weekly[w].push_back(std::make_pair(planLongITimes[0][i].first,
                                       planLongITimes[0][i].second));
        remaining_time[w] -=  planLongITimes[0][i].second;

        // second period
        interviews_weekly[constant::nweeks + w].push_back(std::make_pair(planLongITimes[1][i].first,
                                                          planLongITimes[1][i].second));
        remaining_time[constant::nweeks + w] -=  planLongITimes[1][i].second;

        // third period
        interviews_weekly[2*constant::nweeks + w].push_back(std::make_pair(planLongITimes[2][i].first,
                                                            planLongITimes[2][i].second));
        remaining_time[2*constant::nweeks + w] -=  planLongITimes[2][i].second;

        sum += planLongITimes[0][i].second + planLongITimes[1][i].second + planLongITimes[2][i].second;
    }

    // at the end plan short interviews
    w = 0;
    for (unsigned int i=0; i<_TimeInfo.nShortInterviews; ++i)
    {
        hhID  = _households_sorted[indShortInterviews[i]].ID - 10001;
        itime = _households_sorted[indShortInterviews[i]].itime;

        //        if (remaining_time[w] - itime < 0)
        //            w = std::min(w+1 , constant::nweeks);

        // first period
        interviews_weekly[w].push_back(std::make_pair(hhID, itime));
        remaining_time[w] -=  itime;

        // second period
        interviews_weekly[constant::nweeks + w].push_back(std::make_pair(hhID, itime));
        remaining_time[constant::nweeks + w] -= itime;

        // third period
        interviews_weekly[2*constant::nweeks + w].push_back(std::make_pair(hhID, itime));
        remaining_time[2*constant::nweeks + w] -=  itime;

        w = (w+1) % constant::nweeks;

        sum += 3*itime;
    }

    std::cout << "week \t period1 \t period2 \t period3" << std::endl;
    for (unsigned int w=0; w< constant::nweeks ; ++w)
    {
        std::cout << w+1;
        std::cout << "\t" << remaining_time[w]
                     << "\t" << remaining_time[w + constant::nweeks]
                     << "\t" << remaining_time[w + constant::nweeks * 2] << std::endl;
    }

    std::cout << "summary work time= " << sum << std::endl;

    return interviews_weekly;
}


/*
 * ------------------------------------------------------------------------------------
 */


/*
 * Week plan for each interviewer
 *
 * calculate week plans for each interviewer according to plan of interview times
 *
 */

void assignInterviewersToHH_weekly(const std::vector<std::vector<std::pair<unsigned int, double> > > _hhITimePlan_weekly,
                                   std::vector<stHousehold> _households,         // _households
                                   std::vector<stInterviewer>& _interviewer,
                                   const std::vector<std::vector<double> >  _distmatrixDry,// distmatrix
                                   const std::vector<std::vector<double> >  _distmatrixRain)
{
    unsigned int nI = _interviewer.size();
    unsigned int nWeeks = _hhITimePlan_weekly.size();

    const unsigned int home = 142-101; // Capital (Nouna)

    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    std::vector<std::pair<unsigned int, double> > hhToVisit;
    std::vector<std::vector<unsigned int> > visitedV(nI); // visited villages
    std::vector<std::vector<unsigned int> > visitedHh(nI);// visited _households
    stRoute tmpRoute;

    // maximal worktime pro week (in min) - time to get to the farthest village
    double tmax = 5*8*60.;

    std::vector<double> remaining_time (nI, tmax);
    std::vector<double> remaining_time_new (nI, tmax);


    double ti;                     // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start

    bool rainingSeason = 0;         // raining Season

    /*bool allFull = false*/;

    for (unsigned int w = 0; w<nWeeks; ++w)
    {
        if (w>=20 && w<=40) rainingSeason = 1;
        else rainingSeason = 0;

        tmax = 5*8*60. - 5 * constant::maxDistDry  *(1 - rainingSeason)
                - 5 * constant::maxDistRain *     rainingSeason;

        // delete entries from the previous iteration
        for (unsigned int i=0; i<nI; ++i)
        {
            visitedV[i].erase(visitedV[i].begin(), visitedV[i].end());
            //            visitedV[i].push_back(home);
            visitedHh[i].erase(visitedHh[i].begin(), visitedHh[i].end());

            remaining_time[i] = tmax;
        }

        // hh to visit on the current week
        hhToVisit = _hhITimePlan_weekly[w];

        for (unsigned int i=0; i<hhToVisit.size(); ++i)
        {

            hhID = hhToVisit[i].first;    // next household to visite
            ti = hhToVisit[i].second;     // Interview time

            nextV = _households[hhID].villageID - 101;   // village of the next household

            t_home = 0.;
            //            t_home =    rainingSeason *_distmatrixRain[nextV][home] // time to return home
            //                   + (1-rainingSeason)*_distmatrixDry[nextV][home];

            //        predV= home;
            // for each Interviewer calculate the remaining time, if he takes the hh
            for (unsigned int k=0; k<nI; ++k)
            {
                //                predV = visitedV[k].back(); // previous village in tour k
                t_changev = 0;
                //                t_changev =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
                //                          + (1-rainingSeason)*_distmatrixDry[predV][nextV];
                remaining_time_new[k] = remaining_time[k] - (t_changev + ti + t_home);
            }

            // find min remaining time over all positive values
            unsigned int  k_min = find_posmin(remaining_time_new.begin(), remaining_time_new.end())
                    - remaining_time_new.begin();

            k_min = std::min(k_min, nI-1);

            if (visitedV[k_min].size()>0)
                predV = visitedV[k_min].back();
            else
                predV = 99999;
            //            t_changev =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
            //                      + (1-rainingSeason)*_distmatrixDry[predV][nextV];
            //            t_home =    rainingSeason *_distmatrixRain[nextV][home] // time to return home
            //                   + (1-rainingSeason)*_distmatrixDry[nextV][home];

            //?????????????????????????????????????????????????????????????????????????????
            remaining_time[k_min] = remaining_time_new[k_min];// + t_home; // we dont want to come back immediately
            //?????????????????????????????????????????????????????????????????????????????

            visitedHh[k_min].push_back(hhID);

            if (predV !=nextV)
                visitedV[k_min].push_back(nextV);

        }

        // at the end of the week w close all routes:
        // add time to go home
        // assign routes to Interviewer
        for (unsigned int k=0; k<nI; ++k)
        {
            //            predV = visitedV[k].back();    // last village in the route

            //remaining_time[k] -= _distmatrix[predV][home];
            //            visitedV[k].push_back(home);

            tmpRoute.villages = visitedV[k];
            tmpRoute.households = visitedHh[k];
            tmpRoute.time = tmax - remaining_time[k];

            _interviewer[k].routes_weeks.push_back(tmpRoute);
        }

    } // end week w

}


/*
 * Day Schedules:
 * split week schedules in days
 */

void make_day_plans (stInterviewer& interviewer,
                     const std::vector<stVillage> _villages,
                     const std::vector<stHousehold> _households,         // _households
                     std::vector<std::vector <std::pair<unsigned int, double> > >  _hhITimePlan_weekly, // <hh, itime>
                     const std::vector<std::vector<double> > _distmatrixDry,        // distmatrix
                     const std::vector<std::vector<double> > _distmatrixRain        // distmatrix
                     )
{
    unsigned int nWeeks = constant::nweeks * constant::P;
    unsigned int nDays = 5;

    unsigned int N;                    // number of hh to visit on the current week
    std::vector<unsigned int> hhToVis; // households to visit on the current week

    unsigned int V;     // number of villages to visit on the current week
    std::vector<unsigned int> vToVis; // villages to visit on the current week

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int firstV, lastV; // last villaege of previous day, first village of current day

    unsigned int hhID; // hhID

    stRoute tmpRoute;
    std::vector<std::pair<unsigned int, double> >::iterator _hhITimePlan_it;

    const double tmax = 8*60.;    // maximal worktime pro day (in min)
    double ti;                    // interview time
    double ti_rest;
    double t_changev;             // time to move in other village
    double t_home;                // time to come back at the start

    std::vector<std::vector<unsigned int> > visitedV(nDays); // visited villages
    std::vector<std::vector<unsigned int> > visitedHh(nDays);// visited _households
    std::vector<double> remaining_time(nDays, tmax); // work time at each day

    unsigned int d, day, newday;         // current week day

    bool rainingSeason = 0;         // raining Season



    // plan each week one after another
    for (unsigned int w=0; w<nWeeks; ++w)
    {
        if (w>=20 && w <=40) rainingSeason = 1;
        else rainingSeason = 0;

        // delete entries from the previous iteration
        for (day=0; day<nDays; ++day)
        {
            visitedV[day].erase(visitedV[day].begin(), visitedV[day].end());
            visitedHh[day].erase(visitedHh[day].begin(), visitedHh[day].end());
            remaining_time[day] = tmax;
        }

        // villages and households to visit on the current week
        hhToVis = interviewer.routes_weeks[w].households;
        N = interviewer.routes_weeks[w].households.size();

        vToVis = interviewer.routes_weeks[w].villages;
        V = vToVis.size();
//        std::cout << "\n\nweek " << w << "  #villages to visit " << V
//                  << "  #hh to visit " << N << std::endl;

        // if there is nothing to be done on the current week
        if (hhToVis.empty()){
            // add 5 empty entries in the schedule
            for (day=0; day<nDays; ++day)
            {
                tmpRoute.villages = visitedV[day];
                tmpRoute.households = visitedHh[day];
                tmpRoute.time = tmax - remaining_time[day];

                interviewer.routes_days.push_back(tmpRoute);
            }
            continue;
        }

        /*
         * sort households in ascending order of distance to the village they belong to
         */
        //        vToVis = sort_villages_dist(vToVis,_distmatrixDry);
        /*
         * sort households in increasing order of the angle they make with the capital
         */

        vToVis = sort_villages_angles(vToVis, _villages);

        std::vector<std::vector<unsigned int> > village_household_Matrix;
        village_household_Matrix = groupHH(vToVis, hhToVis, _households);

        // each week starts at capital (home)
        predV = home;
        visitedV[0].push_back(home);
        day = 0; // fill one day afte another


        for (unsigned int v=0; v<V; ++v)    // for each village
        {
            nextV = vToVis[v];   // village of the next household

            while (!village_household_Matrix[v].empty())
            {
                hhID = village_household_Matrix[v].back(); // next household to visite
                village_household_Matrix[v].pop_back();

                // interview time
                _hhITimePlan_it = std::find_if(_hhITimePlan_weekly[w].begin(), _hhITimePlan_weekly[w].end(),
                                               [hhID] (std::pair<unsigned int, double> const& element)
                                                    { return element.first == hhID;});
                ti = (*_hhITimePlan_it).second;


                t_home    =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                          + (1-rainingSeason)*_distmatrixDry[nextV][home];

                // try to add into already started day
                for (d=0; d<=day; ++d)
                {
                    predV = visitedV[d].back();
                    t_changev =    rainingSeason *_distmatrixRain[predV][nextV] // time to change village
                              + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    if (remaining_time[d] - (t_changev + ti + t_home) >= 0 )
                    {
                        if (predV != nextV)
                            visitedV[d].push_back(nextV);
                        visitedHh[d].push_back(hhID);
                        remaining_time[d] -= t_changev + ti;
                        break;
                    }

                }

                // start new day
                if (d>day)
                {
                    newday = std::min(int(day+1), 4);
                    if (newday!=day)
                    {
                        day = newday;        // switch to the next day
                        predV = home;        // start from home
                        visitedV[day].push_back(home);
                    }
                    else
                        predV = visitedV[day].back(); // continue the last day of the week

                    t_changev =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
                              + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    remaining_time[day] -= t_changev + ti;
                    if (predV != nextV)
                        visitedV[day].push_back(nextV);
                    visitedHh[day].push_back(hhID);
                }

            } // end while
        }   // end for v

        // return home at the end of the day
        for (d=0; d<=day; ++d)
        {
            predV = visitedV[d].back();
            t_home =    rainingSeason *_distmatrixRain[predV][home]
                   + (1-rainingSeason)*_distmatrixDry [predV][home];

            remaining_time[d] -= t_home;
            visitedV[d].push_back(home);
        }


//        // stay for a night, if the next day beginns in the same village
//        for (day=0; day<4; ++day)
//        {
//            if(visitedV[day].size()>=2 && visitedV[day+1].size()>=2)
//            {
//                lastV = visitedV[day][visitedV[day].size()-2];
//                firstV = visitedV[day+1][1];
//                if (lastV == firstV)
//                {
//                    visitedV[day].pop_back();
//                    t_changev =    rainingSeason *_distmatrixRain[lastV][home] // travel time between previous village and new village
//                            + (1-rainingSeason)*_distmatrixDry [lastV][home];
//                    remaining_time[day] += t_changev;

//                    visitedV[day+1].erase(visitedV[day+1].begin());
//                    t_changev =    rainingSeason *_distmatrixRain[home][firstV] // travel time between previous village and new village
//                            + (1-rainingSeason)*_distmatrixDry [home][firstV];
//                    remaining_time[day+1] += t_changev;
//                }
//            }
//        }


        // save all routes
        for (day=0; day<nDays; ++day)
        {
            tmpRoute.villages = visitedV[day];
//            std::cout << "\nday " << day << " #hh " << visitedHh[day].size() << " ";
            tmpRoute.households = visitedHh[day];
            tmpRoute.time = tmax - remaining_time[day];
            interviewer.routes_days.push_back(tmpRoute);
        }
    }   // end week

}

// allow interviewer to stay over the night, if the next day begins in the same village
// at the end of the week all interviewer should come at Nouna
void stay_over_night(std::vector<stInterviewer>& _interviewer,
                     std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                     std::vector<std::vector<double> >  _distmatrixRain) // distmatrix)
{
    unsigned int nWeeks = constant::nweeks * constant::P;
    unsigned int nDays = 5;

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int firstV, lastV; // last villaege of previous day, first village of current day

    double t_changev;             // time to move in other village

    bool rainingSeason = 0;         // raining Season



    for (unsigned int k=0; k<_interviewer.size(); ++k)
    {
        // plan each week one after another
        for (unsigned int w=0; w<nWeeks; ++w)
        {
            if (w>=20 && w <=40) rainingSeason = 1;
            else rainingSeason = 0;

            // stay for a night, if the next day beginns in the same village
            for (unsigned int day=0; day<nDays-1; ++day)
            {
                unsigned int len = _interviewer[k].routes_days[w*5 + day].villages.size();
                unsigned int len1 = _interviewer[k].routes_days[w*5 + day + 1].villages.size();
                if ( len >= 2 && len1 >= 2)
                {
                    lastV =  _interviewer[k].routes_days[w*5 + day].villages[len - 2];
                    firstV = _interviewer[k].routes_days[w*5 + day + 1].villages[1];
                    if (lastV == firstV)
                    {
                        _interviewer[k].routes_days[w*5 + day].villages.pop_back();
                        t_changev =    rainingSeason *_distmatrixRain[lastV][home] // travel time between previous village and new village
                                  + (1-rainingSeason)*_distmatrixDry [lastV][home];
                        _interviewer[k].routes_days[w*5 + day].time -= t_changev;

                        _interviewer[k].routes_days[w*5 + day + 1].villages.erase(
                                    _interviewer[k].routes_days[w*5 + day + 1].villages.begin());
                        t_changev =    rainingSeason *_distmatrixRain[home][firstV] // travel time between previous village and new village
                                  + (1-rainingSeason)*_distmatrixDry [home][firstV];
                        _interviewer[k].routes_days[w*5 + day + 1].time -= t_changev;
                    }
                }
            }
        }   // end week
    } // end _interviewer[k]
}


// split too long interviews

void spleet_longinterviews(std::vector<stInterviewer>& _interviewer,
                     std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                     std::vector<std::vector<double> >  _distmatrixRain) // distmatrix)
{

}

/*
 * Initial Solution
 */
void initialsolution2(std::vector<stVillage> _villages,           // villages
                      std::vector<stHousehold> _households,         // _households
                      std::vector<stInterviewer>& _interviewer,      // _interviewer
                      std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                      std::vector<std::vector<double> >  _distmatrixRain, // distmatrix
                      std::vector<std::vector<unsigned int> > _village_household,
                      timeStatistic _TimeInfo,
                      std::vector<std::vector<std::pair<unsigned int, double> > >& _hhITimePlan_weekly
                      )
{

    std::cout << "Construct initial solution" << std::endl << std::endl;

    unsigned int nI = _interviewer.size();

    std::vector<stHousehold> _households_sorted;

    // erase previous tours
    for (unsigned int k=0; k < _interviewer.size(); ++k)
    {
        _interviewer[k].nRoutes = 0;
        _interviewer[k].routes_weeks.erase(_interviewer[k].routes_weeks.begin(),
                                           _interviewer[k].routes_weeks.end());
        _interviewer[k].routes_days.erase(_interviewer[k].routes_days.begin(),
                                          _interviewer[k].routes_days.end());
    }

    /*
     * Preprocessing
     * sort Villages (and households) in increasing order of the angle they make with the capital
     */
    _households_sorted = households_presorting(_villages,_households,_village_household);

    /*
     * Define interview times of each hh in each of three visits and assign weeks of visits
     */
    _hhITimePlan_weekly = planInterviews_weekly(_households_sorted, nI, _TimeInfo);

    /*
     *  Assign Interviewers to the interview
     */
    assignInterviewersToHH_weekly(_hhITimePlan_weekly, _households,_interviewer,
                                  _distmatrixDry, _distmatrixRain);

    /*
     * split week plans into day plans
     */
    for (unsigned int k=0; k<_interviewer.size(); ++k)
        make_day_plans(_interviewer[k], _villages, _households,_hhITimePlan_weekly, _distmatrixDry, _distmatrixRain);

    // allow to stay in some villages over night
    stay_over_night(_interviewer, _distmatrixDry, _distmatrixRain);

}
