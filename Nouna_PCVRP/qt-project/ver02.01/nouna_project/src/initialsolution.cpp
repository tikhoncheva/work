#include "hrd/initialsolution.h"


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


std::vector<std::vector<unsigned int> > sort_v_with_hh(std::vector<unsigned int> villagesID,
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


// sort Villages (and households) in increasing order of the angle they make with the capital
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

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

/*
 * Year plan of the interviews:
 * day |->   hhIDs to visit
 *
 */
std::vector<std::vector<std::pair<unsigned int, double> > > planInterviewInYear(std::vector<stHousehold> _households_sorted, // _households
                                                            unsigned int K,            // number of interviewers
                                                            timeStatistic _TimeInfo)
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
    unsigned int d = 0;  // day

    std::vector<std::pair<int, double> >::iterator hhNode;
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


    double sum1 = 0., sum2 = 0., sum3 = 0.;
    for (unsigned int i=0; i<nLongInterviews; ++i)
    {
        sum1 += planLongITimes[0][i].second;
        sum2 += planLongITimes[1][i].second;
        sum3 += planLongITimes[2][i].second;
    }

    std::cout << "Period \t LongInterview Times" << std::endl;
    std::cout << "Period 1 \t" << sum1 << std::endl;
    std::cout << "Period 2 \t" << sum2 << std::endl;
    std::cout << "Period 3 \t" << sum3 << std::endl;
    std::cout << std::endl;
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
     * Step 3: assign a day to each interview according to the week assignments from the
     * last step
     */

    std::cout << std::endl;
    std::cout << "Step 3" << std::endl;


    unsigned int nDays = constant::P * constant::nweeks * 5;
    std::vector<std::vector<std::pair<unsigned int, double> > > dayInterviews(nDays);


    // maximal worktime pro day (in min) - time to get to the farthest village
    const double tmax = K * 8*60.;// - 5* constant::maxDistDry  *(1 - rainingSeason)
//                                - 5* constant::maxDistRain *     rainingSeason;
    std::cout << "maximal free time weekly " << tmax << std::endl;

    std::vector<double> remaining_time (nDays, tmax);

    std::vector<unsigned int>  days(constant::nweeks, 0);
    // we plan only one period, because all iterviews should be done on the same days in each
    // of the three periods

    // plan long interviews first
    double sum = 0.0;
    for (unsigned int i=0; i<nLongInterviews; ++i)
    {
        w = weeks[i];
        hhID  = planLongITimes[0][i].first;
        itime = planLongITimes[0][i].second;

        d = days[w];
        days[w] = (days[w]+1)%5;
        d = std::min(int(d),4);

        dayInterviews [w*5+d].push_back(std::make_pair(hhID, itime));
        remaining_time[w*5+d] -= itime;

        dayInterviews [constant::nweeks*5 + w*5 + d].push_back(std::make_pair(hhID, planLongITimes[1][i].second));
        remaining_time[constant::nweeks*5 + w*5 + d] -= planLongITimes[1][i].second;

        dayInterviews [2*constant::nweeks*5 + w*5 + d].push_back(std::make_pair(hhID, planLongITimes[2][i].second));
        remaining_time[2*constant::nweeks*5 + w*5 + d] -= planLongITimes[2][i].second;

        sum += itime + planLongITimes[1][i].second + planLongITimes[2][i].second;
    }

    //plan short interviews
    d = 0; // here d \in [0, nweeks-1]
    for (unsigned int i=0; i<_TimeInfo.nShortInterviews; ++i)
    {
        hhID  = _households_sorted[indShortInterviews[i]].ID - 10001;
        itime = _households_sorted[indShortInterviews[i]].itime;

        dayInterviews [d].push_back(std::make_pair(hhID, itime));
        remaining_time[d] -= itime;

        dayInterviews [constant::nweeks*5 + d].push_back(std::make_pair(hhID, itime));
        remaining_time[constant::nweeks*5 + d] -= itime;

        dayInterviews [2*constant::nweeks*5 + d].push_back(std::make_pair(hhID, itime));
        remaining_time[2*constant::nweeks*5 + d] -= itime;

        d = (d + 1) % (constant::nweeks*5);

        sum += 3*itime;
    }

    std::cout << "day\t" << "households \t" << "free time" << std::endl;
    for (unsigned int d=0; d< constant::nweeks * 5 ; ++d)
    {
        std::cout << d+1 << "\t";
//        for (unsigned int j=0; j<dayInterviews[d].size(); ++j)
//            std::cout << dayInterviews[d][j] + 10001 << " ";
        std::cout << "\t" << remaining_time[d]
                  << "\t" << remaining_time[d + constant::nweeks * 5]
                  << "\t" << remaining_time[d + constant::nweeks * 5 * 2] << std::endl;
    }

    std::cout << "summary work time= " << sum << std::endl;
    //-------------------------------------------------------------------------------------------

    return dayInterviews;
}

void assignInterviewersToHH(const std::vector<std::vector<std::pair<unsigned int, double> > > _dayplan_interviews,
                             std::vector<stHousehold> _households,         // _households
                            std::vector<stInterviewer>& _interviewer,
                            const std::vector<std::vector<double> >  _distmatrixDry,// distmatrix
                            const std::vector<std::vector<double> >  _distmatrixRain)
{
    unsigned int nI = _interviewer.size();
    unsigned int nDays = _dayplan_interviews.size();


    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int predV;                // predecessor of the current hh
    unsigned int nextV;                // next village to go

    unsigned int hhID; // hhID

    std::vector<std::pair<unsigned int, double> > hhToVis;
    std::vector<unsigned int> vToVis;

    const double tmax = 8*60.;    // maximal worktime pro day (in min)
    double ti;                    // interview time
    double t_changev;             // time to move to other village
    double t_home;                // time to come back at the start


    std::vector<std::vector<unsigned int> > visitedV(nI); // visited villages assigned to interviewer
    std::vector<std::vector<unsigned int> > visitedHh(nI);// visited _households assigned to interviewer
    std::vector<double> remaining_time(nI, tmax); // remaining work time of each interviewer on the current day
    std::vector<double> remaining_time_new(nI, tmax); // remaining work time of each interviewer on the current day
    stRoute tmpRoute;

    bool rainingSeason = 0;         // raining Season

    // plan one day after ahother

    for (unsigned int d = 0; d<nDays; ++d)
    {   // d is the current day

        // check if it is raining season or not
        if (d>=20*5 && d <=40*5) rainingSeason = 1;
            else rainingSeason = 0;

        // delete entries from the previous iteration
        for (unsigned int i=0; i<nI; ++i)
        {
            visitedV[i].erase(visitedV[i].begin(), visitedV[i].end());
            visitedV[i].push_back(home);
            visitedHh[i].erase(visitedHh[i].begin(), visitedHh[i].end());
            remaining_time[i] = tmax;
        }

        // households to visit on the current day
        hhToVis = _dayplan_interviews[d];
        // corresponding villages
        for (unsigned int h=0; h<hhToVis.size(); ++h)
        {
            hhID = hhToVis[h].first;
            vToVis.push_back(_households[hhID].villageID - 101);
        }

        /*
         * sort households in ascending order of distance to the village they belong to
         */
//        if (rainingSeason) vToVis = sort_villages_dist(vToVis,_distmatrixRain);
//            else vToVis = sort_villages_dist(vToVis,_distmatrixDry);

//        std::vector<std::vector<unsigned int> > village_household_Matrix;
//        village_household_Matrix = sort_v_with_hh(vToVis, hhToVis, _households);

        predV = home;
        for (unsigned int h=0; h<hhToVis.size(); ++h)
        {
            hhID = hhToVis[h].first;
            nextV = vToVis[h];

            ti = hhToVis[h].second;

            for (unsigned int i=0; i<nI; ++i)
            {
                predV = visitedV[i].back();
                t_home    =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                          + (1-rainingSeason)*_distmatrixDry[nextV][home];

                t_changev =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
                          + (1-rainingSeason)*_distmatrixDry[predV][nextV];
                remaining_time_new[i] = remaining_time[i] - (t_changev + ti + t_home);
            }

            unsigned int selInterviewer = find_posmin(remaining_time_new.begin(), remaining_time_new.end())
                                        - remaining_time_new.begin();
            selInterviewer = std::min(selInterviewer, nI-1); // if interview does not pass anybody, the last interviewer should take

            visitedV[selInterviewer].push_back(nextV);
            visitedHh[selInterviewer].push_back(hhID);
            remaining_time[selInterviewer] -= t_changev + ti;
        }// all hh on the day are visited

        //save all routes
        for (unsigned int i=0; i<nI; ++i)
        {
            predV = visitedV[i].back();
            visitedV[i].push_back(home);
            t_home    =    rainingSeason *_distmatrixRain[predV][home] // cam back
                      + (1-rainingSeason)*_distmatrixDry [predV][home];

            tmpRoute.villages = visitedV[i];
            tmpRoute.households = visitedHh[i];
            tmpRoute.time = tmax - remaining_time[i];
            _interviewer[i].routes_days.push_back(tmpRoute);
        }

    }// end day

}

void make_week_plans(std::vector<stInterviewer>& _interviewer)
{
    unsigned int nI = _interviewer.size();
    unsigned int nWeeks = constant::P * constant::nweeks;

    std::vector<unsigned int> visitedV(nI); // visited villages assigned to interviewer
    std::vector<unsigned int> visitedHh(nI);// visited _households assigned to interviewer
    double worktime = 0.;
    stRoute tmpRoute;

    for (unsigned int i=0; i<nI; ++i)
    {
        for (unsigned int w=0; w<nWeeks; ++w)
        {
            visitedV.erase(visitedV.begin(), visitedV.end());
            visitedHh.erase(visitedHh.begin(), visitedHh.end());
            worktime = 0.;

            for (unsigned int d=0; d<5; ++d)
            {
                if (!_interviewer[i].routes_days[w*5+d].households.empty())
                {
                    for (unsigned int j=0; j< _interviewer[i].routes_days[w*5+d].villages.size(); ++j)
                        visitedV.push_back(_interviewer[i].routes_days[w*5+d].villages[j]);

                    for (unsigned int j=0; j< _interviewer[i].routes_days[w*5+d].households.size(); ++j)
                        visitedHh.push_back(_interviewer[i].routes_days[w*5+d].households[j]);
                    worktime += _interviewer[i].routes_days[w*5+d].time;
                }
            }

            tmpRoute.villages = visitedV;
            tmpRoute.households = visitedHh;
            tmpRoute.time = worktime;
            _interviewer[i].routes_weeks.push_back(tmpRoute);
        }
    }
}


/*
 * Initial Solution
 */
void initialsolution(std::vector<stVillage> _villages,           // villages
                     std::vector<stHousehold> _households,         // _households
                     std::vector<stInterviewer>& _interviewer,      // _interviewer
                     std::vector<std::vector<double> >  _distmatrix,// distmatrix
                     std::vector<std::vector<double> >  _distmatrixRain,// distmatrix
                     std::vector<std::vector<unsigned int> > _village_household,
                     timeStatistic _TimeInfo
                     )
{

    std::cout << "Construct initial solution" << std::endl << std::endl;

    unsigned int nI = _interviewer.size();
    std::vector<stHousehold> households_sorted;

    // erase previous tours
    for (unsigned int k=0; k < nI; ++k)
    {
        _interviewer[k].nRoutes = 0;
        _interviewer[k].routes_weeks.erase(_interviewer[k].routes_weeks.begin(),
                                           _interviewer[k].routes_weeks.end());
        _interviewer[k].routes_days.erase(_interviewer[k].routes_days.begin(),
                                          _interviewer[k].routes_days.end());
    }

    /*
     * Preprocessing
     */
    households_sorted = households_presorting(_villages,_households,_village_household);

    /*
     * Associate interviews with days
     */
    std::vector<std::vector<std::pair<unsigned int, double> > > dayplan_interviews;
    dayplan_interviews = planInterviewInYear(households_sorted, nI, _TimeInfo);

    /*
     *  Assign Interviewers to the interview
     */
    assignInterviewersToHH(dayplan_interviews, _households,_interviewer,
                           _distmatrix, _distmatrixRain);

    /*
     * collect data to week plan
     */
    make_week_plans(_interviewer);

}
