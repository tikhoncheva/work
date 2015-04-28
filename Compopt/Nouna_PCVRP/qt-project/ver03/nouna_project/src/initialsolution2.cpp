#include "hrd/initialsolution2.h"


/*
 * returns position on the minimum positive value in the container
 */
template <class ForwardIterator>
ForwardIterator find_positivemin( ForwardIterator first, ForwardIterator last )
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
 * sort villages in increasing order of the angle they make with the capital
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
/*
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
*/

// ------------------------------------------------------------------------------------------------
/*
 * sort Villages (and households) in increasing order of the angle they make with the capital
 */

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

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


/* -------------------------------------------------------------------------------------------------
 * For each week in an year (48 weeks) plan households, that should be visited on that week.
 * -------------------------------------------------------------------------------------------------
 * Output: table week/hhIDs: row i corresponds to week number i, each entry in the row i constains
 * a pair : hhID and interview time of this hh on the week i. i=0...47
 *
 * Each week should contain roughly same amount (in min) of long interviews
 * The short interviews should be spread uniformly in the whole year
 */
std::vector<std::vector<std::pair<unsigned int, double> > > assign_hh_to_weeks(
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
            w = find_positivemin(remainingTime_new.begin(), remainingTime_new.end())
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
            w = find_positivemin(remainingTime_new.begin(), remainingTime_new.end())
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
            w = find_positivemin(remainingTime_new.begin(), remainingTime_new.end())
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
 * Output: week vs daily routes on the week
 *
 * Use result from assign_hh_to_weeks (_hhITimePlan_weekly) and split each week plan into groups,
 * that represent time plans for one working day. Check, that number of such groups in each week can
 * not exceed number (#working_days_in_week x # Interviewers). Otherwise it is imposible to visit
 * all planed households.
 */

std::vector<std::vector<stRoute> > create_daily_routes(unsigned int nI, // number of interviews
                                   std::vector<stHousehold> _households,         // _households
                                   std::vector<std::vector<std::pair<unsigned int, double> > >& _hhITimePlan_weekly,
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
    double ti, ti1, ti2;            // interview time and first part of interview time, if interview
                                    // was splitted into two or more day

    double thome, thome_pred;       // time to return to Nouna
    double tmove;                   // time to change village

    double remaining_time;          // remaining day plan
    bool rainingSeason = 0;         // raining Season

    bool it_is_split = false;             // if an interview need to be split in several days


    hhITimePlan_daily.resize(nWeeks);  // daily plans are grouped accoreding to week they will be visited at

    std::vector<std::vector<std::pair<unsigned int, double> > > copyITimePlan; // copy of _hhITimePlan_weekly
    std::vector<std::pair<unsigned int, double> >::iterator it;

    copyITimePlan.resize(_hhITimePlan_weekly.size());
    for (unsigned int w=0; w<_hhITimePlan_weekly.size(); ++w)
        for (unsigned int p=0; p<_hhITimePlan_weekly[w].size(); ++p )
        {
            copyITimePlan[w].push_back(std::make_pair(_hhITimePlan_weekly[w][p].first,
                                                      _hhITimePlan_weekly[w][p].second));
        }


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
        hhToVisit = copyITimePlan[w];

        // sort hh, that should be visited on the week w, in descending order by the interview time
        std::sort(hhToVisit.begin(), hhToVisit.end(), []( const std::pair<unsigned int, double>& a,
                                                          const std::pair<unsigned int, double>& b)
                                                        { return b.second < a.second;});

        // build groups
        predV = home;
        remaining_time = tmax;
        thome_pred = 0;

        route_V.push_back(home);

        for (unsigned int i=0; i<hhToVisit.size(); ++i)
        {
            it_is_split = false;

            hhID = hhToVisit[i].first;    // next household to visite
            ti = hhToVisit[i].second;     // Interview time

            if (hhID == 10715 - 10001)
                std::cout << ti << std::endl;

            nextV = _households[hhID].villageID - 101;   // village of the next household

            thome =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                  + (1-rainingSeason)*_distmatrixDry[nextV][home];
            tmove =    rainingSeason *_distmatrixRain[predV][nextV] // time to change village
                  + (1-rainingSeason)*_distmatrixDry[predV][nextV];

//*
            if (remaining_time < tmove + ti + thome)
            {
                // split interview
                if (tmove + 10 <= remaining_time &&
                    remaining_time <= tmove + (ti-10))
                {
                    it_is_split = true;
                    ti1 = remaining_time - tmove;
                    ti2 = ti - ti1;

                    // replace split interview with two new entries in the table with weekly plans
                    it = std::find_if(copyITimePlan[w].begin(), copyITimePlan[w].end(),
                                      [hhID] (std::pair<unsigned int, double> const& element)
                                            { return element.first == hhID;});
                    if (it == copyITimePlan[w].end())
                        std::cout << "Error in createDailyPlansFromWeeklyPlans" << std::endl;

                    (*it).first = 999999;
                    (*it).second = ti1;
                    copyITimePlan[w].push_back(std::make_pair(hhID, ti2));
//                    (*it).second = ti2;     // replace entry with new interview time


                    hhToVisit.insert(hhToVisit.begin() + i+1, std::make_pair(hhID, ti2));

                    _hhITimePlan_weekly[w][it - copyITimePlan[w].begin()].second = ti1;

//                    if (ti2 <= tmax)    // otherwise we will split interview one more time
                    _hhITimePlan_weekly[w].push_back(std::make_pair(hhID, ti2));

                    ti = ti1;

                    remaining_time = 0;
                    thome_pred = thome;

                    route_Hh.push_back(hhID);
                    if (std::find(route_V.begin(), route_V.end(), nextV) == route_V.end())
                        route_V.push_back(nextV);
                }

                    // close current day and start a new one
                    route_V.push_back(home);

                    tmpRoute.villages = route_V;
                    tmpRoute.households = route_Hh;
                    tmpRoute.time = tmax - (remaining_time - thome_pred);

                    // save day route into _hhITimePlan_daily[w]
                    hhITimePlan_daily[w].push_back(tmpRoute);
                    tmpRoute.villages.clear();
                    tmpRoute.households.clear();

                    route_V.erase(route_V.begin(), route_V.end());
                    route_Hh.erase(route_Hh.begin(), route_Hh.end());

                    remaining_time = tmax;
                    tmove = thome;
                    predV = home;
                    route_V.push_back(home);
            }

            if (it_is_split) continue;

            // add interview to the current day
            remaining_time -= (tmove + ti);
            route_Hh.push_back(hhID);
            if (std::find(route_V.begin(), route_V.end(), nextV) == route_V.end())
                route_V.push_back(nextV);

            thome_pred = thome;
            predV = nextV;
        }   // end build groups

//*
/*
            if (remaining_time < tmove + ti + thome)
            {
                // split interview
                if (tmove + 10 <= remaining_time &&
                    remaining_time <= tmove + (ti-10))
                {


                    ti_new = remaining_time - tmove;
                    ti -= ti_new;

                    // replace split interview with two new entries in the table with weekly plans
                    _hhITimePlan_it = std::find_if(_hhITimePlan_weekly[w].begin(),
                                                   _hhITimePlan_weekly[w].end(),
                                                   [hhID] (std::pair<unsigned int, double> const& element)
                                                    { return element.first == hhID;});
                    if (_hhITimePlan_it == _hhITimePlan_weekly[w].end())
                        std::cout << "Error in createDailyPlansFromWeeklyPlans" << std::endl;

                    (*_hhITimePlan_it).second = ti_new;
                    _hhITimePlan_weekly[w].push_back(std::make_pair(hhID, ti));

                    remaining_time = 0;
                    thome_pred = thome;

                    route_Hh.push_back(hhID);
                    if (std::find(route_V.begin(), route_V.end(), nextV) == route_V.end())
                        route_V.push_back(nextV);
                }

                // close current day and start a new one
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
            predV = nextV;
        }   // end build groups
*/
        //close current route if opened
        route_V.push_back(home);

        tmpRoute.villages = route_V;
        tmpRoute.households = route_Hh;
        tmpRoute.time = tmax - (remaining_time - thome_pred);

        // save day root into _hhITimePlan_daily[w]
        hhITimePlan_daily[w].push_back(tmpRoute);

        route_V.erase(route_V.begin(), route_V.end());
        route_Hh.erase(route_Hh.begin(), route_Hh.end());

//        std::cout << "Week " << w+1 << " number of groups " << hhITimePlan_daily[w].size()
//                  << " agains " << nI * 5 << " possible." << std::endl;

        // ASSERT: number of groups in each week can not exceed number (#working_days_in_week x # Interviewers)
        assert(hhITimePlan_daily[w].size() <= 5*nI);

    } // end week w

    return hhITimePlan_daily;
}

/* -------------------------------------------------------------------------------------------------
 * Assign created daily plant to available interviewers
 * -------------------------------------------------------------------------------------------------
 */

void assign_dailyRoutes_to_interviewer(std::vector<stInterviewer>& _interviewer,
                                    const std::vector<std::vector<stRoute> > _hhITimePlan_daily)
{
    unsigned int nI = _interviewer.size();
    unsigned int nWeeks = _hhITimePlan_daily.size();
    unsigned int nDPlans;

    for (unsigned int i=0; i<nI; ++i)
        _interviewer[i].routes_days.resize(nWeeks*5);

    for (unsigned int w=0; w<nWeeks; ++w)
    {
        nDPlans = _hhITimePlan_daily[w].size();

        // ASSERT: number of groups in each week can not exceed number (#working_days_in_week x # Interviewers)
        assert(nDPlans<=5*nI);  // we have already done this, but anyway

        for (unsigned int i=0; i<nDPlans; ++i)
        {
//            _interviewer[i/5].routes_days.push_back(_hhITimePlan_daily[w][i]);
            _interviewer[i/5].routes_days[5*w + i%5] = _hhITimePlan_daily[w][i];

        }
    }

//    for (unsigned int i=0; i<nI; ++i)
//        _interviewer[i].routes_days.resize(nWeeks*5);
}

/* ------------------------------------------------------------------------------------------------
 * Make week routes for each interviewer by collecting information from daily routes
 * ------------------------------------------------------------------------------------------------
 */
void make_weekRoutes(std::vector<stInterviewer>& _interviewer)
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
 * -------------------------------------------------------------------------------------------------
 * Initial Solution
 * -------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------
 */
int initialsolution2(std::vector<stVillage> _villages,           // villages
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
    _hhITimePlan_weekly = assign_hh_to_weeks(_households_sorted, _TimeInfo);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Divide weekly plans from previous step into daily plans";
    std::vector<std::vector<stRoute> > _hhITimePlan_daily;
   _hhITimePlan_daily = create_daily_routes(nI, _households,_hhITimePlan_weekly,
                                   _distmatrixDry, _distmatrixRain);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Assign daily plans to interviewer";
    assign_dailyRoutes_to_interviewer(_interviewer, _hhITimePlan_daily);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Combine daily routes of each interviewer into week routes";
    make_weekRoutes(_interviewer);
    std::cout << " ... finished\n" << std::endl;


    std::cout << "Allow to stay over the night in some villages";
    stay_over_night(_interviewer, _distmatrixDry, _distmatrixRain);
    std::cout << " ...finished\n" << std::endl;

    return 1;
}
