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
 *
 * the hh in the same village are sorted in the descending order of their interview times
 */

std::vector<std::vector<unsigned int> > groupHH2(std::vector<unsigned int> villagesID,
                                                 std::vector<unsigned int> householdsID,
                                                 const std::vector<stHousehold> _households,
                                                 std::vector <std::pair<unsigned int, double> >  _hhITimePlan_week)
{
    unsigned int V = villagesID.size();
    unsigned int H = householdsID.size();
    std::vector<std::vector<unsigned int> > _village_household(V);

    std::vector<std::pair<double, unsigned int> > tmp;
    std::vector<std::pair<unsigned int, double> >::iterator _hhITimePlan_it;

    unsigned int vID;   // village's ID
    unsigned int hhID;  // household ID
    double ti;

    // for each village find it's hh
    for (unsigned int i=0; i<V; ++i)    // for each village
    {
        vID =  villagesID[i] + 101;

        tmp.erase(tmp.begin(),tmp.end());

        for (unsigned int j=0; j< H; ++j)
        {
            // find household in the village i ;
            if (_households[householdsID[j]].villageID==vID)
            {
                hhID = householdsID[j];

                _hhITimePlan_it = std::find_if(_hhITimePlan_week.begin(), _hhITimePlan_week.end(),
                                               [hhID] (std::pair<unsigned int, double> const& element)
                { return element.first == hhID;});
                ti = (*_hhITimePlan_it).second;
                tmp.push_back(std::make_pair(ti, hhID));
            }

        }

        // sort household in the descending order of their interview times
        std::sort(tmp.begin(), tmp.end(), []( const std::pair<double, int>& a, const std::pair<double, int>& b)
        { return a.first > b.first;});
        for (unsigned int j=0; j<tmp.size(); ++j)
            _village_household[i].push_back(tmp[j].second);

    }

    return _village_household;
}

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
 * -------------------------------------------------------------------------------------------------
 * Assign a week of visiting to each hh
 * -------------------------------------------------------------------------------------------------
 * For each week in an year (48 weeks) plan households, that should be visited on that week.
 * As result return table week/hhIDs
 *
 * Each week should contain roughly same amount (in min) of long interviews
 * The short interviews should be spread uniformly in the whole year
 */
std::vector<std::vector<std::pair<unsigned int, double> > > planInterviews_weekly(
        std::vector<stHousehold> _households_sorted, // _households
        timeStatistic _TimeInfo)                     // information about mean values
{
    /*
     * Step 1: set interview times for long interviewes in each period, so that
     * summary interview time in each period is roughly equivalent
     */

    std::cout << "Step 1: distribute long interview into three periods";

    unsigned int nLongInterviews;

    double pmeanLongITime = _TimeInfo.sLongITime1 / double(constant::P);
    double wmeanLongITime = _TimeInfo.sLongITime1 / double(constant::P * constant::nweeks);
    double sumITimePeriod = 0.;

    unsigned int p = 0;  // period
    unsigned int w = 0;  // week

    unsigned int nWeeks = constant::nweeks * constant::P;

    double itime;
    unsigned int hhID;

    std::vector<std::vector <std::pair<int, double> > > planLongITimes(3); // interview time of long interviews in each period
    std::vector<unsigned int > indShortInterviews;


    for (unsigned int i=0; i<_households_sorted.size(); ++i)
    {
        if (_households_sorted[i].type == 1)    // only long interviews
        {
            hhID = _households_sorted[i].ID - 10001;

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
    }

    nLongInterviews = _TimeInfo.nLongInterviews;                        // correct number of long interviews
    assert(planLongITimes[0].size() == nLongInterviews);
    assert(planLongITimes[1].size() == nLongInterviews);
    assert(planLongITimes[2].size() == nLongInterviews);

    assert(indShortInterviews.size() == _TimeInfo.nShortInterviews);    // correct number of short interviews

    std::cout << " ... finished" << std::endl;
    //-------------------------------------------------------------------------------------------

    /*
     * Step 2: Set for each long interviews a week, it will be made at
     *         Each week should contain roughly same amount (in min) of long interviews
     */

    std::cout << "Step 2: assign weeks to each long interview";

    std::vector<unsigned int> weeks(nLongInterviews);

    std::vector< double > remainingTime (constant::nweeks, wmeanLongITime);
    std::vector< double > remainingTime_new(constant::nweeks, 0.);

    //----------------------------------------------------------------------------------
    // assign long interview from the first period
    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        hhID = planLongITimes[0][i].first;
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
    }
    //----------------------------------------------------------------------------------
    // assign long interview from the second period

    std::fill(remainingTime.begin(), remainingTime.end(), wmeanLongITime);

    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        hhID = planLongITimes[1][i].first;
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
    }
    //----------------------------------------------------------------------------------
    // assign long interview from the third period

    std::fill(remainingTime.begin(), remainingTime.end(), wmeanLongITime);

    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        hhID = planLongITimes[2][i].first;
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
    }
    std::cout << " ... finished" <<std::endl;

    //-------------------------------------------------------------------------------------------

    /*
    * Step 3: Combine results from two previous steps in one matrix of the form: week| HH/itime
    */


    std::cout << "Step 3: combine results of the two previuos steps to get matrix: week vs hhID/time";

    std::vector<std::vector<std::pair<unsigned int, double>>> interviews_weekly;
    interviews_weekly.resize(nWeeks);

    // first go through long interviews

    for (unsigned int i=0; i< nLongInterviews; ++i)
    {
        w = weeks[i];

        assert(planLongITimes[0][i].first == planLongITimes[1][i].first);
        assert(planLongITimes[0][i].first == planLongITimes[2][i].first);

        // first period
        interviews_weekly[w].push_back(std::make_pair(planLongITimes[0][i].first,
                                       planLongITimes[0][i].second));

        // second period
        interviews_weekly[constant::nweeks + w].push_back(std::make_pair(planLongITimes[1][i].first,
                                                          planLongITimes[1][i].second));

        // third period
        interviews_weekly[2*constant::nweeks + w].push_back(std::make_pair(planLongITimes[2][i].first,
                                                            planLongITimes[2][i].second));
    }

    // at the end plan short interviews

    // we want to have almost the same number of short interviewes in each week.
    // So we divide number of short interviewes by number of weeks in one period (16 weeks)
    // this gives us the number of short interviewers in on week
    // We use this number to spread short interviewers in first period and than just accept the selected
    // week for each short interviewer in two further periods

    unsigned int nShortI_inOneWeek = _TimeInfo.nShortInterviews / constant::nweeks;
    unsigned int R = _TimeInfo.nShortInterviews % constant::nweeks; // remainder

    std::vector<unsigned int> nShortI_week(constant::nweeks, nShortI_inOneWeek);

    // If remainder R of the division is greater than 0, increase number of short interviews in the
    // first R weeks by one
    for(unsigned int r=0; r<R; ++r)
        ++nShortI_week[r];

    unsigned int h=0; // counter of short interviews
    for (w=0; w<constant::nweeks; ++w)
    {
        for (unsigned int i=0; i<nShortI_week[w]; ++i)
        {
            hhID  = _households_sorted[indShortInterviews[h]].ID - 10001;
            itime = _households_sorted[indShortInterviews[h]].itime;

            // period 1
            interviews_weekly[w].push_back(std::make_pair(hhID, itime));
            // period 2
            interviews_weekly[constant::nweeks + w].push_back(std::make_pair(hhID, itime));
            // period 3
            interviews_weekly[2*constant::nweeks + w].push_back(std::make_pair(hhID, itime));

            ++h; // go to the next interviewer
        }
    }

    std::cout << " ... finished" << std::endl;

    return interviews_weekly;
}




/*
 * -------------------------------------------------------------------------------------------------
 * Create day plans from week plans
 * -------------------------------------------------------------------------------------------------
 *
 * Use result from planInterviews_weekly (_hhITimePlan_weekly) and split each week plan into groups,
 * that represent time plans for one working day. Check, that number of such groups in each week can
 * not exceed number (#working_days_in_week x # Interviewers). Otherwise it is imposible to visit
 * all planed households.
 */

std::vector<std::vector<stRoute> > createDailyPlansFromWeeklyPlans(unsigned int nI, // number of interviews
                                   std::vector<stHousehold> _households,         // _households
                                   const std::vector<std::vector<std::pair<unsigned int, double> > > _hhITimePlan_weekly,
                                   const std::vector<std::vector<double> >  _distmatrixDry,// distmatrix
                                   const std::vector<std::vector<double> >  _distmatrixRain)
{
    unsigned int nWeeks = _hhITimePlan_weekly.size();

    unsigned int home = 142 - 101;
    unsigned int nextV, predV; // next village to go
    unsigned int hhID; // hhID

    std::vector<std::pair<unsigned int, double> > hhToVisit;

    std::vector<unsigned int> route_V; // visited villages
    std::vector<unsigned int> route_Hh;// visited households

    std::vector<std::vector<stRoute> > hhITimePlan_daily;

    stRoute tmpRoute;

    double tmax = 8*60.;            // maximal worktime pro day (in min)
    double ti, ti_new;              // interview time and first part of interview time, if interview
                                    // was splitted into two or more day

    double thome, thome_pred;       // time to return to Nouna
    double tmove;                   // time to change village

    double remaining_time;          // remaining day plan
    bool rainingSeason = 0;         // raining Season


    hhITimePlan_daily.resize(nWeeks);  // daily plans are grouped accoreding to week they will be visited at

    for (unsigned int w = 0; w<nWeeks; ++w)
//    unsigned int w=0;
    {
        // split households, that should be visited at current week, into groups with maximal summary
        // interview time equal (tmax - 2 x thome (from first and last hh in a group))
        // those groups are considered further as week plans for one person
        // After groups are formed, assign them to interviews

        if (w>=20 && w<=40) rainingSeason = 1;
        else rainingSeason = 0;

        // hh to visit on the current week
        hhToVisit = _hhITimePlan_weekly[w];

        // build groups
        predV = home;
        remaining_time = tmax;
        thome_pred = 0;

        route_V.push_back(home);

        for (unsigned int i=0; i<hhToVisit.size(); ++i)
        {

            hhID = hhToVisit[i].first;    // next household to visite
            ti = hhToVisit[i].second;     // Interview time

            nextV = _households[hhID].villageID - 101;   // village of the next household

            thome =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                  + (1-rainingSeason)*_distmatrixDry[nextV][home];
            tmove =    rainingSeason *_distmatrixRain[predV][nextV] // time to change village
                  + (1-rainingSeason)*_distmatrixDry[predV][nextV];


            if (remaining_time < tmove + ti + thome)
            {
                // split interview
                if (tmove + 10 <= remaining_time &&
                    remaining_time <= tmove + (ti-10))
                {
                    ti_new = remaining_time - tmove;
                    ti -= ti_new;
                    remaining_time = 0;
                    thome_pred = thome;

                    route_Hh.push_back(hhID);
                    if (std::find(route_V.begin(), route_V.end(), nextV) == route_V.end())
                        route_V.push_back(nextV);
                }

                // and current day and start a new one
                route_V.push_back(home);

                tmpRoute.villages = route_V;
                tmpRoute.households = route_Hh;
                tmpRoute.time = tmax - (remaining_time - thome_pred);

                // save day root into _hhITimePlan_daily[w]
                hhITimePlan_daily[w].push_back(tmpRoute);

                route_V.erase(route_V.begin(), route_V.end());
                route_Hh.erase(route_Hh.begin(), route_Hh.end());

                remaining_time = tmax;
                tmove = thome;
                predV = home;
                route_V.push_back(home);
            }

            // add interview to the current day
            remaining_time -= (tmove + ti);
            route_Hh.push_back(hhID);
            if (std::find(route_V.begin(), route_V.end(), nextV) == route_V.end())
                route_V.push_back(nextV);

            thome_pred = thome;
        }   // end build groups

        // ASSERT: number of groups in each week can not exceed number (#working_days_in_week x # Interviewers)
        if (hhITimePlan_daily[w].size()>5*nI)
            std::cout << "Error in week " << w+1 << " plans: " << hhITimePlan_daily[w].size() << " groups vs 5x"
                      << nI << " interviewers=" << nI * 5 << std::endl;

    } // end week w

    return hhITimePlan_daily;
}

/*
 * -------------------------------------------------------------------------------------------------
 * Assign created daily plant to available interviewers
 * -------------------------------------------------------------------------------------------------
 */

void assignDailyPlansToInterviewers(std::vector<stInterviewer>& _interviewer,
                                    const std::vector<std::vector<stRoute> > _hhITimePlan_daily)
{
    unsigned int nI = _interviewer.size();
    unsigned int nWeeks = _hhITimePlan_daily.size();
    unsigned int nDPlans;

    for (unsigned int w=0; w<nWeeks; ++w)
    {
        nDPlans = _hhITimePlan_daily[w].size();

        // ASSERT: number of groups in each week can not exceed number (#working_days_in_week x # Interviewers)
        assert(_hhITimePlan_daily[w].size()<=5*nI);  // we have already done this, but anyway

        for (unsigned int i=0; i<nDPlans; ++i)
        {
            _interviewer[i/5].routes_days.push_back(_hhITimePlan_daily[w][i]);
        }
    }

    for (unsigned int i=0; i<nI; ++i)
        _interviewer[i].routes_days.resize(nWeeks*5);
}

/*
 * Make week routes for each interviewer by collecting information from daily routes
 */
void makeWeekRoutes(std::vector<stInterviewer>& _interviewer)
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

                    for (unsigned int j=1; j< _interviewer[i].routes_days[w*5+d].villages.size()-1; ++j)
                        visitedV.push_back(_interviewer[i].routes_days[w*5+d].villages[j]);

                    for (unsigned int j=0; j< _interviewer[i].routes_days[w*5+d].households.size(); ++j)
                        visitedHh.push_back(_interviewer[i].routes_days[w*5+d].households[j]);

                    worktime += _interviewer[i].routes_days[w*5+d].time;
                }
            }

            std::vector<unsigned int>::iterator it;
            it = std::unique(visitedV.begin(), visitedV.end());
            visitedV.resize( std::distance(visitedV.begin(),it) );

            tmpRoute.villages = visitedV;
            tmpRoute.households = visitedHh;
            tmpRoute.time = worktime;
            _interviewer[i].routes_weeks.push_back(tmpRoute);
        }
    }

}


/*
 * -------------------------------------------------------------------------------------------------
 * Day Schedules:
 * -------------------------------------------------------------------------------------------------
 * split week schedules of each interviewer in days
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

    unsigned int N;
    std::vector<unsigned int> hhToVis; // households to visit on the current week

    unsigned int V;     // number of villages to visit on the current week
    std::vector<unsigned int> vToVis; // villages to visit on the current week

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    stRoute tmpRoute;
    std::vector<std::pair<unsigned int, double> >::iterator _hhITimePlan_it;

    const double tmax = 8*60.;    // maximal worktime pro day (in min)
    double ti;                    // interview time
    double tmove;             // time to move in other village
    double thome;                // time to come back at the start

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


        // sort households in ascending order of distance to the village they belong to
        //        vToVis = sort_villages_dist(vToVis,_distmatrixDry);

        // sort households in increasing order of the angle they make with the capital
        vToVis = sort_villages_angles(vToVis, _villages);

        std::vector<std::vector<unsigned int> > village_household_Matrix;
        village_household_Matrix = groupHH(vToVis, hhToVis, _households);

        //
        assert(village_household_Matrix.size() == V);
        unsigned int count = 0;
        for (unsigned int p=0; p<village_household_Matrix.size(); ++p)
            count += village_household_Matrix[p].size();
        assert(count == N);     // check that one hh wasn't classified into two or more villages
        //

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


                thome    =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                        + (1-rainingSeason)*_distmatrixDry[nextV][home];

                // try to add into already started day
                for (d=0; d<=day; ++d)
                {
                    predV = visitedV[d].back();
                    tmove =    rainingSeason *_distmatrixRain[predV][nextV] // time to change village
                            + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    if (remaining_time[d] - (tmove + ti + thome) >= 0 )
                    {
                        if (predV != nextV)
                            visitedV[d].push_back(nextV);
                        visitedHh[d].push_back(hhID);
                        remaining_time[d] -= tmove + ti;
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

                    tmove =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
                            + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    remaining_time[day] -= tmove + ti;
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
            thome =    rainingSeason *_distmatrixRain[predV][home]
                    + (1-rainingSeason)*_distmatrixDry [predV][home];

            remaining_time[d] -= thome;
            visitedV[d].push_back(home);
        }

        // save all routes
        for (day=0; day<nDays; ++day)
        {
            tmpRoute.villages = visitedV[day];
            tmpRoute.households = visitedHh[day];
            tmpRoute.time = tmax - remaining_time[day];
            interviewer.routes_days.push_back(tmpRoute);
        }
    }   // end week

}

void make_day_plans2 (stInterviewer& interviewer,
                      const std::vector<stVillage> _villages,
                      const std::vector<stHousehold> _households,         // _households
                      std::vector<std::vector <std::pair<unsigned int, double> > >  _hhITimePlan_weekly, // <hh, itime>
                      const std::vector<std::vector<double> > _distmatrixDry,        // distmatrix
                      const std::vector<std::vector<double> > _distmatrixRain        // distmatrix
                      )
{
    unsigned int nWeeks = constant::nweeks * constant::P;
    unsigned int nDays = 5;

    unsigned int N;
    std::vector<unsigned int> hhToVis; // households to visit on the current week

    unsigned int V;     // number of villages to visit on the current week
    std::vector<unsigned int> vToVis; // villages to visit on the current week

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    stRoute tmpRoute;
    std::vector<std::pair<unsigned int, double> >::iterator _hhITimePlan_it;

    const double tmax = 8*60.;    // maximal worktime pro day (in min)
    double ti;                    // interview time
    double tmove;             // time to move in other village
    double thome;                // time to come back at the start

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

        // sort households in increasing order of the angle they make with the capital
        vToVis = sort_villages_angles(vToVis, _villages);

        std::vector<std::vector<unsigned int> > village_household_Matrix;
        village_household_Matrix = groupHH2(vToVis, hhToVis, _households, _hhITimePlan_weekly[w]);

        //
        assert(village_household_Matrix.size() == V);
        unsigned int count = 0;
        for (unsigned int p=0; p<village_household_Matrix.size(); ++p)
            count += village_household_Matrix[p].size();
        assert(count == N);
        //

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


                thome    =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                        + (1-rainingSeason)*_distmatrixDry[nextV][home];

                // try to add into already started day
                for (d=0; d<=day; ++d)
                {
                    predV = visitedV[d].back();
                    tmove =    rainingSeason *_distmatrixRain[predV][nextV] // time to change village
                            + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    if (remaining_time[d] - (tmove + ti + thome) >= 0 )
                    {
                        if (predV != nextV)
                            visitedV[d].push_back(nextV);
                        visitedHh[d].push_back(hhID);
                        remaining_time[d] -= tmove + ti;
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

                    tmove =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
                            + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    remaining_time[day] -= tmove + ti;
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
            thome =    rainingSeason *_distmatrixRain[predV][home]
                    + (1-rainingSeason)*_distmatrixDry [predV][home];

            remaining_time[d] -= thome;
            visitedV[d].push_back(home);
        }

        // save all routes
        for (day=0; day<nDays; ++day)
        {
            tmpRoute.villages = visitedV[day];
            tmpRoute.households = visitedHh[day];
            tmpRoute.time = tmax - remaining_time[day];
            interviewer.routes_days.push_back(tmpRoute);
        }
    }   // end week

}


/*
 * -------------------------------------------------------------------------------------------------
 * Stay over the night
 * -------------------------------------------------------------------------------------------------
 * allow interviewer to stay over the night, if the next day begins in the same village
 * at the end of the week all interviewer should come at Nouna
 *
 */
void stay_over_night(std::vector<stInterviewer>& _interviewer,
                     std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                     std::vector<std::vector<double> >  _distmatrixRain) // distmatrix)
{
    unsigned int nWeeks = constant::nweeks * constant::P;
    unsigned int nDays = 5;

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int firstV, lastV; // last villaege of previous day, first village of current day

    double thome;             // time to move in other village

    bool rainingSeason = 0;         // raining Season



    for (unsigned int k=0; k<_interviewer.size(); ++k)
    {
        // look through all weeks
        for (unsigned int w=0; w<nWeeks; ++w)
        {
            if (w>=20 && w <=40) rainingSeason = 1;
            else rainingSeason = 0;

            // stay for a night, if the next day beginns in the same village
            for (unsigned int day=0; day<nDays-1; ++day)
            {
                unsigned int len  = _interviewer[k].routes_days[w*5 + day].villages.size();
                unsigned int len1 = _interviewer[k].routes_days[w*5 + day + 1].villages.size();
                if ( len >= 2 && len1 >= 2)
                {
                    lastV =  _interviewer[k].routes_days[w*5 + day].villages[len - 2];
                    firstV = _interviewer[k].routes_days[w*5 + day + 1].villages[1];
                    if (lastV == firstV)
                    {
                        _interviewer[k].routes_days[w*5 + day].villages.pop_back();
                        thome =    rainingSeason *_distmatrixRain[lastV][home] // travel time between previous village and new village
                                + (1-rainingSeason)*_distmatrixDry [lastV][home];
                        _interviewer[k].routes_days[w*5 + day].time -= thome;

                        _interviewer[k].routes_days[w*5 + day + 1].villages.erase(
                                    _interviewer[k].routes_days[w*5 + day + 1].villages.begin());
                        thome =    rainingSeason *_distmatrixRain[home][firstV] // travel time between previous village and new village
                                + (1-rainingSeason)*_distmatrixDry [home][firstV];
                        _interviewer[k].routes_days[w*5 + day + 1].time -= thome;
                    }
                }
            }
        }   // end week
    } // end _interviewer[k]
}

/*
 * -------------------------------------------------------------------------------------------------
 * Split too long interviews
 * -------------------------------------------------------------------------------------------------
 * all interviews, those time is longer than one working day, are only the one interviews to make
 * in the corresponding day
 */


// start from the end of the current day (day) of the week (week) and adapt sequentially working time on each day before
// moving sequentially to the left,
// Interviews, which takes too long, can be split and started on the day before
void propagateToTheLeft(unsigned int week, unsigned int day,
                        stInterviewer& _interviewer,
                        const std::vector<stHousehold> _households,         // _households
                        const std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                        const std::vector<std::vector<double> >  _distmatrixRain, // distmatrix)
                        std::vector<std::vector <std::pair<unsigned int, double> > >& _hhITimePlan_week)
{
    unsigned int d = 0;

    double tmax = 8 * 60.;
    double ti;      // interview time
    double ti_rest;
    double thome;//, thome_pred;
    double tmove;
    double twork;

    unsigned int predV;
    unsigned int V;


    std::vector<unsigned int> hhVecToReorder; // new vector of hh

    std::vector<unsigned int> newHhVec; // new vector of hh
    std::vector<unsigned int> newVVec;  // new vector of villages

    unsigned int home = 142 - 101;
    unsigned int hhID;
    std::vector<std::pair<unsigned int, double> >::iterator _hhITimePlan_it;

    bool rainingSeason = 0;
    bool addVillage;// = false;

    if (week>=20 && week<=40) rainingSeason = 1;
    else rainingSeason = 0;


    // fill new array of hh to be reordered
    for(d = week*5; d <= day; ++d)
    {
        for (unsigned int h = 0;h < _interviewer.routes_days[d].households.size();++h)
            hhVecToReorder.push_back(_interviewer.routes_days[d].households[h]);

        // delete old schedules
        _interviewer.routes_days[d].households.erase(
                    _interviewer.routes_days[d].households.begin(),
                    _interviewer.routes_days[d].households.end());
        _interviewer.routes_days[d].villages.erase(
                    _interviewer.routes_days[d].villages.begin(),
                    _interviewer.routes_days[d].villages.end());

        _interviewer.routes_days[d].time = 0.;
        _interviewer.routes_days[d].villages.push_back(home);
        _interviewer.routes_days[d].villages.push_back(home);
    }

    predV = home;
    thome =    rainingSeason *_distmatrixRain[predV][home]
            + (1-rainingSeason)*_distmatrixDry[predV][home];

    twork = 2 * thome;
    assert( twork == 0.);

    newVVec.push_back(home);

    // rearange array
    do {

        twork -= thome;
        hhID = hhVecToReorder.back();  hhVecToReorder.pop_back();
        V    = _households[hhID].villageID - 101;

        newHhVec.push_back(hhID);
        if (std::find(newVVec.begin(), newVVec.end(), V) == newVVec.end())
        {
            addVillage = true;
            newVVec.push_back(V);
        }

        _hhITimePlan_it = std::find_if(_hhITimePlan_week[week].begin(),
                                       _hhITimePlan_week[week].end(),
                                       [hhID] (std::pair<unsigned int, double> const& element)
        { return element.first == hhID;});
        ti = (*_hhITimePlan_it).second;  // it's interview time

        tmove =    rainingSeason *_distmatrixRain[predV][V]
                + (1-rainingSeason)*_distmatrixDry[predV][V];

        thome =    rainingSeason *_distmatrixRain[V][home]
                + (1-rainingSeason)*_distmatrixDry[V][home];

        twork += tmove + ti + thome;

        predV = V;

        if (twork - tmax - thome >= 10 && day>week*5) // ti_rest >= 10 min
        {
            ti_rest = (twork - tmax) - thome;
            if (ti_rest<ti) // split        // otherwise we cannot split (ti will be negative)
                // but it can lead here to the ptoblem, that
                // a day is too overloaded
            {
                ti -= ti_rest;
                // add to the time table, that interview time of hh hhID was splitted
                (*_hhITimePlan_it).second = ti;
                _hhITimePlan_week[week].push_back(std::make_pair(hhID, ti_rest));


                _interviewer.routes_days[day].households.erase(
                            _interviewer.routes_days[day].households.begin(),
                            _interviewer.routes_days[day].households.end());

                _interviewer.routes_days[day].villages.erase(
                            _interviewer.routes_days[day].villages.begin(),
                            _interviewer.routes_days[day].villages.end());

                newVVec.push_back(home);
                std::reverse(newHhVec.begin(), newHhVec.end());
                std::reverse(newVVec.begin(), newVVec.end());

                _interviewer.routes_days[day].time = tmax + thome;
                _interviewer.routes_days[day].households = newHhVec;
                _interviewer.routes_days[day].villages = newVVec;

                newHhVec.erase(newHhVec.begin(), newHhVec.end());
                newVVec.erase(newVVec.begin(), newVVec.end());

                newHhVec.push_back(hhID);
                twork = thome + ti_rest + thome; /// 2x thome??

                newVVec.push_back(home);
                newVVec.push_back(V);

                if (day!=0)
                    day = std::max(day - 1 , week*5);
            }
        } // ti_split >=10

        if (twork>tmax && day>week*5) // we cannot split (otherwise ti < 0)
        {   // move last interview on the next day

            /*
            newHhVec.erase(newHhVec.end()-1);
            if (addVillage)
                newVVec.erase(newVVec.end()-1);
            twork -= tmove + ti + thome;


            _interviewer.routes_days[day].households.erase(
                        _interviewer.routes_days[day].households.begin(),
                        _interviewer.routes_days[day].households.end());

            _interviewer.routes_days[day].villages.erase(
                        _interviewer.routes_days[day].villages.begin(),
                        _interviewer.routes_days[day].villages.end());

            thome_pred =    rainingSeason *_distmatrixRain[newVVec.back()][home]
                    + (1-rainingSeason)*_distmatrixDry [newVVec.back()][home];
            twork += thome_pred;
            newVVec.push_back(home);

            std::reverse(newHhVec.begin(), newHhVec.end());
            std::reverse(newVVec.begin(), newVVec.end());

            _interviewer.routes_days[day].time += twork;
            _interviewer.routes_days[day].households = newHhVec;
            _interviewer.routes_days[day].villages = newVVec;

            newHhVec.erase(newHhVec.begin(), newHhVec.end());
            newVVec.erase(newVVec.begin(), newVVec.end());

            newHhVec.push_back(hhID);
            newVVec.push_back(home);
            newVVec.push_back(V);

            predV = newVVec.back();
            twork = thome + ti + thome;
 */

            // /*
            // leaf last interview on the same day and close it/ start to plan new day
            _interviewer.routes_days[day].households.erase(
                        _interviewer.routes_days[day].households.begin(),
                        _interviewer.routes_days[day].households.end());

            _interviewer.routes_days[day].villages.erase(
                        _interviewer.routes_days[day].villages.begin(),
                        _interviewer.routes_days[day].villages.end());

            newVVec.push_back(home);

            std::reverse(newHhVec.begin(), newHhVec.end());
            std::reverse(newVVec.begin(), newVVec.end());

            _interviewer.routes_days[day].time = twork;
            _interviewer.routes_days[day].households = newHhVec;
            _interviewer.routes_days[day].villages = newVVec;

            newHhVec.erase(newHhVec.begin(), newHhVec.end());
            newVVec.erase(newVVec.begin(), newVVec.end());

            newVVec.push_back(home);
            predV = home;
            twork = 0.;
            thome = 0.;
            // */
            if (day!=0)
                day = std::max(day - 1 , week*5);

        } // if twork > tmax

    } while(! hhVecToReorder.empty());

    // save last array

    _interviewer.routes_days[day].households.erase(
                _interviewer.routes_days[day].households.begin(),
                _interviewer.routes_days[day].households.end());

    _interviewer.routes_days[day].villages.erase(
                _interviewer.routes_days[day].villages.begin(),
                _interviewer.routes_days[day].villages.end());

    newVVec.push_back(home);

    std::reverse(newHhVec.begin(), newHhVec.end());
    std::reverse(newVVec.begin(), newVVec.end());

    _interviewer.routes_days[day].time = twork + thome;
    _interviewer.routes_days[day].households = newHhVec;
    _interviewer.routes_days[day].villages = newVVec;
}


// start from the beginning of the current day (day) of the week (week) and adapt sequentially working time on each day after
// moving sequentially to the right,
// Interviews, which takes too long, can be split and continued on the next day
void propagateToTheRight(unsigned int week, unsigned int day,
                         stInterviewer& _interviewer,
                         const std::vector<stHousehold> _households,         // _households
                         const std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                         const std::vector<std::vector<double> >  _distmatrixRain, // distmatrix)
                         std::vector<std::vector <std::pair<unsigned int, double> > >& _hhITimePlan_week)
{
    unsigned int d = 0;

    double tmax = 8 * 60.;
    double ti;      // interview time
    double ti_rest;
    double thome, thome_pred;
    double tmove;
    double twork;

    unsigned int predV;
    unsigned int V;

    std::vector<unsigned int> hhVecToReorder; // new vector of hh

    std::vector<unsigned int> newHhVec; // new vector of hh
    std::vector<unsigned int> newVVec;  // new vector of villages

    unsigned int home = 142 - 101;
    unsigned int hhID;
    std::vector<std::pair<unsigned int, double> >::iterator _hhITimePlan_it;

    bool rainingSeason = 0;
    bool addVillage; // = false;

    if (week>=20 && week<=40) rainingSeason = 1;
    else rainingSeason = 0;

    // fill new array of hh to be reordered
    for(d = day; d < (week+1)*5; ++d)
    {
        for (unsigned int h = 0;
             h < _interviewer.routes_days[d].households.size();++h)
            hhVecToReorder.push_back(_interviewer.routes_days[d].households[h]);

        _interviewer.routes_days[d].households.erase(
                    _interviewer.routes_days[d].households.begin(),
                    _interviewer.routes_days[d].households.end());
        _interviewer.routes_days[d].villages.erase(
                    _interviewer.routes_days[d].villages.begin(),
                    _interviewer.routes_days[d].villages.end());
        _interviewer.routes_days[d].time = 0.;

        _interviewer.routes_days[d].villages.push_back(home);
        _interviewer.routes_days[d].villages.push_back(home);
    }
    std::reverse(hhVecToReorder.begin(), hhVecToReorder.end());

    predV = home;
    thome =    rainingSeason *_distmatrixRain[predV][home]
            + (1-rainingSeason)*_distmatrixDry[predV][home];

    twork = 2 * thome;
    thome_pred = thome;
    assert( twork == 0.);

    newVVec.push_back(home);

    // rearange array
    do { // while(! hhVecToReorder.empty());

        twork -= thome;
        hhID = hhVecToReorder.back();  hhVecToReorder.pop_back();
        V    = _households[hhID].villageID - 101;

        newHhVec.push_back(hhID);

        addVillage = false;
        if (std::find(newVVec.begin(), newVVec.end(), V) == newVVec.end())
        {
            newVVec.push_back(V);
            addVillage = true;
        }

        _hhITimePlan_it = std::find_if(_hhITimePlan_week[week].begin(),
                                       _hhITimePlan_week[week].end(),
                                       [hhID] (std::pair<unsigned int, double> const& element)
        { return element.first == hhID;});
        ti = (*_hhITimePlan_it).second;  // it's interview time

        tmove =    rainingSeason *_distmatrixRain[predV][V]
                + (1-rainingSeason)*_distmatrixDry[predV][V];

        thome =    rainingSeason *_distmatrixRain[V][home]
                + (1-rainingSeason)*_distmatrixDry[V][home];

        twork += tmove + ti + thome;

        predV = V;

        if (twork - tmax - thome >= 10 && day < (week+1)*5 - 1) // ti_rest >= 10 min
        {
            ti_rest = (twork - tmax) - thome;

            if (ti_rest < ti)   // split
            {
                ti -= ti_rest;
                // add to the time table, that interview time of hh hhID was splitted
                (*_hhITimePlan_it).second = ti;
                _hhITimePlan_week[week].push_back(std::make_pair(hhID, ti_rest));
                //
                _interviewer.routes_days[day].households.erase(
                            _interviewer.routes_days[day].households.begin(),
                            _interviewer.routes_days[day].households.end());

                _interviewer.routes_days[day].villages.erase(
                            _interviewer.routes_days[day].villages.begin(),
                            _interviewer.routes_days[day].villages.end());

                newVVec.push_back(home);

                _interviewer.routes_days[day].time = tmax + thome;
                _interviewer.routes_days[day].households = newHhVec;
                _interviewer.routes_days[day].villages = newVVec;

                newHhVec.erase(newHhVec.begin(), newHhVec.end());
                newVVec.erase(newVVec.begin(), newVVec.end());

                newHhVec.push_back(hhID);
                twork = thome + ti_rest + thome;    /// 2x thome??

                newVVec.push_back(home);
                newVVec.push_back(V);

                day = std::min(day + 1, (week+1)*5 - 1);
            }
        }   // if twork - tmax - thome > 10

        if (twork>tmax && day < (week+1)*5 - 1)
        { // move last interview on the next

            /*
            newHhVec.erase(newHhVec.end()-1);
            if (addVillage)
                newVVec.erase(newVVec.end()-1);
            twork -= tmove + ti + thome;


            _interviewer.routes_days[day].households.erase(
                        _interviewer.routes_days[day].households.begin(),
                        _interviewer.routes_days[day].households.end());

            _interviewer.routes_days[day].villages.erase(
                        _interviewer.routes_days[day].villages.begin(),
                        _interviewer.routes_days[day].villages.end());

            thome_pred =    rainingSeason *_distmatrixRain[newVVec.back()][home]
                       + (1-rainingSeason)*_distmatrixDry [newVVec.back()][home];
            twork += thome_pred;
            newVVec.push_back(home);

            _interviewer.routes_days[day].time = twork;
            _interviewer.routes_days[day].households = newHhVec;
            _interviewer.routes_days[day].villages = newVVec;

            newHhVec.erase(newHhVec.begin(), newHhVec.end());
            newVVec.erase(newVVec.begin(), newVVec.end());

            newHhVec.push_back(hhID);
            newVVec.push_back(home);
            newVVec.push_back(V);
            predV = V;
            twork = thome + ti + thome;
 */

            // /*
            // leaf last interview on the same day abd close it/ start to plan new day
            _interviewer.routes_days[day].households.erase(
                        _interviewer.routes_days[day].households.begin(),
                        _interviewer.routes_days[day].households.end());

            _interviewer.routes_days[day].villages.erase(
                        _interviewer.routes_days[day].villages.begin(),
                        _interviewer.routes_days[day].villages.end());

            newVVec.push_back(home);

            _interviewer.routes_days[day].time = twork;
            _interviewer.routes_days[day].households = newHhVec;
            _interviewer.routes_days[day].villages = newVVec;

            newHhVec.erase(newHhVec.begin(), newHhVec.end());
            newVVec.erase(newVVec.begin(), newVVec.end());

            newVVec.push_back(home);
            predV = home;
            twork = 0.;
            thome = 0.;
            // */

            day = std::min(day + 1, (week+1)*5 - 1);
        }// if twork > tmax

    } while(! hhVecToReorder.empty());

    // save last array
    _interviewer.routes_days[day].households.erase(
                _interviewer.routes_days[day].households.begin(),
                _interviewer.routes_days[day].households.end());

    _interviewer.routes_days[day].villages.erase(
                _interviewer.routes_days[day].villages.begin(),
                _interviewer.routes_days[day].villages.end());

    newVVec.push_back(home);

    _interviewer.routes_days[day].time = twork + thome;
    _interviewer.routes_days[day].households = newHhVec;
    _interviewer.routes_days[day].villages = newVVec;
}


void split_longinterviews(std::vector<stInterviewer>& _interviewer,
                          const std::vector<stHousehold> _households,         // _households
                          std::vector<std::vector<double> >  _distmatrixDry,  // distmatrix
                          std::vector<std::vector<double> >  _distmatrixRain, // distmatrix)
                          std::vector<std::vector <std::pair<unsigned int, double> > >& _hhITimePlan_week)
{
    unsigned int nI = _interviewer.size();
    unsigned int nWeeks = constant::P * constant::nweeks;

    unsigned int d = 0, day_r, day_l = 0;

    double freetime_left = 0.;
    double freetime_right = 0.;

    double tmax = 8 * 60.;
    double twork;

    bool overhours = false;

    for (unsigned int i=0; i< nI; ++i) // for each interviewer
    {
        //                unsigned int i = 2;
        // check each week if there are too long working days
        // repeat 5 times because after one run there still can be overloaded days
        for (unsigned int w=0; w<nWeeks; ++w )
        {
            //                            unsigned int w = 40;
            overhours = false;

            for (day_l = w*5; day_l < (w+1)*5; ++day_l)
            {
                twork = _interviewer[i].routes_days[day_l].time;
                if (twork > tmax)
                {
                    overhours = true;
                    break;
                }
            }

            for (day_r = (w+1)*5 - 1; day_r>=w*5 && day_r<(w+1)*5; --day_r)
            {
                twork = _interviewer[i].routes_days[day_r].time;
                if (twork > tmax)
                {
                    overhours = true;
                    break;
                }
            }
            if (day_r > (w+1)*5)
                day_r = w*5;

            if (overhours)
            {
                // decide to expand time to the beginning of the week or to the end according to the free time
                freetime_left = 0.;
                for (d=w*5; d < day_r; ++d)
                    freetime_left += tmax - _interviewer[i].routes_days[d].time;

                freetime_right = 0.;
                for (d=day_l+1; d < (w+1)*5; ++d)
                    freetime_right += tmax - _interviewer[i].routes_days[d].time;

                if (freetime_right > freetime_left)
                    propagateToTheRight(w /*week*/, day_l /*day*/,
                                        _interviewer[i] /* current interviewer*/,
                                        _households /* households */,
                                        _distmatrixDry /* distmatrix */,
                                        _distmatrixRain /* distmatrix */,
                                        _hhITimePlan_week /* year time schedule*/);
                else
                    propagateToTheLeft(w /*week*/, day_r /*day*/,
                                       _interviewer[i] /* current interviewer*/,
                                       _households /* households */,
                                       _distmatrixDry /* distmatrix */,
                                       _distmatrixRain /* distmatrix */,
                                       _hhITimePlan_week /* year time schedule*/);
            } // if overhours

        }   // for w

    } // for i
}


/*
 * -------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------
 * Initial Solution
 * -------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------
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


    std::cout << "Define interview times of each hh in each of the three visits and assign weeks of the visits\n";
    _hhITimePlan_weekly = planInterviews_weekly(_households_sorted, _TimeInfo);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Divide weekly plans from previous step into daily plans";
    std::vector<std::vector<stRoute> > _hhITimePlan_daily;
   _hhITimePlan_daily = createDailyPlansFromWeeklyPlans(nI, _households,_hhITimePlan_weekly,
                                   _distmatrixDry, _distmatrixRain);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Assign daily plans to interviewer";
    assignDailyPlansToInterviewers(_interviewer, _hhITimePlan_daily);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Combine daily routes of each interviewer into week routes";
    makeWeekRoutes(_interviewer);
    std::cout << " ... finished\n" << std::endl;


//    std::cout << "For each interviewer assign days of the visitis\n";
//    for (unsigned int k=0; k<_interviewer.size(); ++k)
//        make_day_plans(_interviewer[k], _villages, _households,_hhITimePlan_weekly, _distmatrixDry, _distmatrixRain);
//    std::cout << " ... finished\n" << std::endl;


    //    std::cout << "Split too long interviews into several days";
    //    split_longinterviews(_interviewer, _households, _distmatrixDry, _distmatrixRain, _hhITimePlan_weekly);
    //    std::cout << " ...finished" << std::endl;


    //    std::cout << "Allow to stay over the night in some villages";
    //    stay_over_night(_interviewer, _distmatrixDry, _distmatrixRain);
    //    std::cout << " ...finished\n" << std::endl;


//    std::cout << "\n--------------------------Test1-----------------------------------------------\n";
//    // small test, how much interviewer have extra working hours in a week
//    unsigned int count1 = 0;
//    for (unsigned int k=0; k< _interviewer.size(); ++k)
//    {
//        unsigned int week = 0;
//        while(week<48)
//        {
//            double week_wtime = 0.;
//            for (unsigned int d=week*5; d<(week+1)*5; ++d)
//                week_wtime += _interviewer[k].routes_days[d].time;

//            if (week_wtime > 5*8*60)
//            {
//                ++count1;
//                std::cout << "interviewer " << k + 1 << " has extra hours on the week " << week + 1
//                          << ": " << 5*8*60 - week_wtime << " min"<< std::endl;
//            }
//            ++week;
//        }
//    }
//    std::cout << "Summary: " << count1 << " interviewers have extra hours in some weeks" << std::endl;
//    std::cout << "------------------------------------------------------------------------------\n";


    //    std::cout << "\n--------------------------Test2-----------------------------------------------\n";
    //    // small test, how much interviewer have extra working hours in a day
    //    unsigned int count2 = 0;
    //    for (unsigned int k=0; k< _interviewer.size(); ++k)
    //    {
    //        for (unsigned int d=0; d<_interviewer[k].routes_days.size(); ++d )
    //            if (_interviewer[k].routes_days[d].time - 8*60. > 0.001)
    //            {
    //                ++count2;
    //                std::cout << "interviewer " << k + 1 << " has extra hours on the day " << d % 5 + 1
    //                          << "(week " << d/5 + 1 << "): "
    //                          << 8*60 - _interviewer[k].routes_days[d].time << " min"<< std::endl;

    //            }
    //    }
    //    std::cout << "Summary: " << count2 << " interviewers have extra hours in some days" << std::endl;
    //    std::cout << "------------------------------------------------------------------------------\n";
}

