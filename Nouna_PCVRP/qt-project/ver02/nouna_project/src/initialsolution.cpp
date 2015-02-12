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

std::vector<std::vector<unsigned int> > sort_v_with_hh(std::vector<unsigned int> villagesID,
                                                       std::vector<unsigned int> householdsID,
                                                       const std::vector<stHousehold> _households)
{
    // get list of households in each village
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


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

/*
 * Plan Interviewer time in each period
 *
 * Each Pperiod should contain roughly same amount (in min) of long interviews
 *
 **/

std::vector<std::vector <std::pair<int, double> > > planInterviewTimes_period
( std::vector<stHousehold> _households, // _households
  timeStatistic _TimeInfo)
{
    unsigned int N = _households.size();
    unsigned int p = 0;
    unsigned int hhID;
    double meanLongITime = _TimeInfo.sLongITime1 / 3.;
    double sumTimePeriod = 0.;

    std::vector<bool> ToVis(N, true);

    std::vector<std::vector <std::pair<int, double> > > planITimes; // short interviews
    planITimes.resize(3);

    for (unsigned int i=0; i<_households.size(); ++i)
    {
        hhID = i;// rand()%(N-1);
        //        while (!ToVis[hhID])
        //            hhID= (hhID+1)%N;


        planITimes[0].push_back(std::make_pair(hhID, constant::shortITime));
        planITimes[1].push_back(std::make_pair(hhID, constant::shortITime));
        planITimes[2].push_back(std::make_pair(hhID, constant::shortITime));


        if (_households[hhID].type == 1)
        {
            if (sumTimePeriod + _households[hhID].itime > meanLongITime)
            {
                sumTimePeriod = 0.;
                p = std::min(int(p+1), 2);
            }
            planITimes[p].back() = std::make_pair(hhID, _households[hhID].itime);
            sumTimePeriod += _households[hhID].itime;
        }
        ToVis[hhID] = false;
    }

    return planITimes;
}

/*
 * ------------------------------------------------------------------------------------
 * Plan Interviewer time in each week
 *
 * Each week should contain roughly same amount (in min) of long interviews
 *
 **/
std::vector<std::vector <std::pair<int, double> > > planInterviewTimes_week(unsigned int nK, //number of interviewer
                                                                            std::vector<stHousehold> _households, // _households
                                                                            timeStatistic _TimeInfo,
                                                                            std::vector<std::vector <std::pair<int, double> > > planForPeriod)
{
    unsigned int N = _households.size();
    unsigned int hhID;
    double itime;
    unsigned int week;

    double meanLongITime = _TimeInfo.sLongITime1 / double(constant::P*constant::nweeks);

    std::vector<bool> ToVis(N, true);

    std::vector<std::vector <std::pair<int, double> > > planITimes (constant::P * constant::nweeks);


    std::vector< double > remainingTime (constant::nweeks, meanLongITime);
    std::vector< double > remainingTime_new(constant::nweeks, 0.);

    std::vector<std::pair<int, double> >::iterator hhNode;

    // make week plan for the first period and accept it then in two other periods

    // start with the long interviews
    for (unsigned int i=0; i< planForPeriod[0].size(); ++i)
    {
        hhID = planForPeriod[0][i].first;// rand()%(N-1);
        //        while (!ToVis[hhID])
        //            hhID= (hhID+1)%N;

        itime = planForPeriod[0][i].second;

        if (_households[hhID].type == 1 && itime > 10)
        {
            // for each week calculate the remaining time, if hh will be assign to this week
            for (unsigned int week=0; week<constant::nweeks; ++week)
                remainingTime_new[week] = remainingTime[week] - itime;

            // find min remaining time over all positive values
            unsigned int  week_min = find_posmin(remainingTime_new.begin(),
                                                 remainingTime_new.end()) - remainingTime_new.begin();


            week_min = std::min(week_min, constant::nweeks-1); // it can happend that last week in period
            // is a little bit overloaded with long interviews

            planITimes[week_min].push_back(planForPeriod[0][i]);

            remainingTime[week_min] = remainingTime_new[week_min];

        }
        ToVis[hhID] = false;
    }

    // fill free time with short interviews
    week = 0;
    for (unsigned int i=0; i< planForPeriod[0].size(); ++i)
    {
        hhID = planForPeriod[0][i].first;// rand()%(N-1);
        //        while (!ToVis[hhID])
        //            hhID= (hhID+1)%N;

        itime = planForPeriod[0][i].second;

        if (itime == 10)
        {
            planITimes[week].push_back(planForPeriod[0][i]);
            week = (week+1) % constant::nweeks;
        }
        ToVis[hhID] = false;
    }

    // fill free time with short interviews
    remainingTime.erase(remainingTime.begin(), remainingTime.end());
    remainingTime =  std::vector< double >(constant::nweeks, 5*8*60*nK);


    // accept week plan for first period in the second and third periods
    for (unsigned int week=0; week<constant::nweeks; ++week)
    {
        for (unsigned int h=0; h<planITimes[week].size(); ++h)
        {
            hhID = planITimes[week][h].first;

            // find this household in the second period
            hhNode = std::find_if(planForPeriod[1].begin(), planForPeriod[1].end(),
                    [hhID] (std::pair<unsigned int, double> const& element)
            { return element.first == hhID;});

            planITimes[constant::nweeks + week].push_back(*hhNode);


            // find this household in the third period
            hhNode = std::find_if(planForPeriod[2].begin(), planForPeriod[2].end(),
                    [hhID] (std::pair<unsigned int, double> const& element)
            { return element.first == hhID;});

            planITimes[2*constant::nweeks + week].push_back(*hhNode);

        }
    }

    return planITimes;
}

/*
 * ------------------------------------------------------------------------------------
 */


/*
 * Week plan for each interviewer
 *
 * calculate week plans for each interviewer. Here we still do not consider travel times
 * but we leave in each week fee time, that should be enough for traveling
 *
 */
int weekplan_interviewers (std::vector <std::pair<int, double> > weekplan,
                           std::vector<stInterviewer>& _interviewer,             // _interviewer
                           const std::vector<stHousehold> _household,                 // _households
                           const std::vector<std::vector<double> > _distmatrix,        // distmatrix
                           bool rainingSeason    // 1 - raining season, 0 - not
                           )
{
    unsigned int N = weekplan.size();
    const unsigned int K = _interviewer.size();  // maximal number of available interviewers

    const unsigned int home = 142-101; // Capital (Nouna)

    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    std::vector<std::vector<unsigned int> > visitedV(K); // visited villages
    std::vector<std::vector<unsigned int> > visitedHh(K);// visited _households
    stRoute tmpRoute;

    // maximal worktime pro week (in min) - time to get to the farthest village
    const double tmax = 5*8*60.;// - 5* constant::maxDistDry  *(1 - rainingSeason)
    //- 5* constant::maxDistRain *     rainingSeason;

    double ti;                    // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start

    int nUnvisited = 0;

    std::vector<double> remaining_time (K, tmax);
    std::vector<double> remaining_time_new (K, tmax);

    /*bool allFull = false*/;

    // each week starts at capital (home)
    for (unsigned int k=0; k<K; ++k)
        visitedV[k].push_back(home);


    for (unsigned int i=0; i<N; ++i)// && !allFull; ++i)
    {

        hhID = weekplan[i].first;    // next household to visite
        ti = weekplan[i].second;     // Interview time

        nextV = _household[hhID].villageID - 101;   // village of the next household
        t_home = _distmatrix[nextV][home]; // if interviewer would come back home after this hh

        //        predV= home;
        // for each Interviewer calculate the remaining time, if he takes the hh
        for (unsigned int k=0; k<K; ++k)
        {
            predV = visitedV[k].back(); // previous village in tour k
            t_changev = _distmatrix[predV][nextV];// travel time between previous village and new village
            remaining_time_new[k] = remaining_time[k] - (t_changev + ti + t_home);
        }

        // find min remaining time over all positive values
        unsigned int  k_min = find_posmin(remaining_time_new.begin(), remaining_time_new.end())
                - remaining_time_new.begin();

        k_min = std::min(k_min, K);
        if (k_min==K){   // all values in remaining_time_ are negative
            ++nUnvisited;
            continue;
        }

        predV = visitedV[k_min].back();
        t_changev = _distmatrix[predV][nextV];
        //?????????????????????????????????????????????????????????????????????????????
        remaining_time[k_min] = remaining_time_new[k_min];// + t_home; // we dont want to come back immediately
        //?????????????????????????????????????????????????????????????????????????????
        visitedHh[k_min].push_back(hhID);

        if (predV !=nextV)
            visitedV[k_min].push_back(nextV);

        //        allFull = std::all_of(remaining_time.begin(), remaining_time.end(),
        //                              [] (const unsigned int element){ return element == 0;});

    }

    // close all routes: add time to go home
    // assign routes to Interviewer
    for (unsigned int k=0; k<K; ++k)
    {
        predV = visitedV[k].back();    // last village in the route

        //remaining_time[k] -= _distmatrix[predV][home];
        visitedV[k].push_back(home);

        tmpRoute.villages = visitedV[k];
        tmpRoute.households = visitedHh[k];
        tmpRoute.time = tmax - remaining_time[k];

        _interviewer[k].routes_weeks.push_back(tmpRoute);
    }

    return nUnvisited;
}

/*
 * Day Schedules
 */

void dayplan_interviewer (stInterviewer& interviewer,
                           std::vector<std::vector <std::pair<int, double> > >  ITimePlan, // <hh, itime>
                          const std::vector<stHousehold> _households,         // _households
                          const std::vector<std::vector<double> > _distmatrixDry,        // distmatrix
                          const std::vector<std::vector<double> > _distmatrixRain        // distmatrix
                          )
{

    unsigned int N;     // number of houselholds to visit on the current week
    std::vector<unsigned int> hhToVis; // households to visit on the current week

    unsigned int V;     // number of villages to visit on the current week
    std::vector<unsigned int> vToVis; // villages to visit on the current week

    const unsigned int home = 142-101; // Capital (Nouna)

    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int firstV, lastV; // last villaege of priveous day, first village of current day

    unsigned int hhID; // hhID

    std::vector<std::vector<unsigned int> > visitedV(5); // visited villages
    std::vector<std::vector<unsigned int> > visitedHh(5);// visited _households

    stRoute tmpRoute;
    std::vector<std::pair<int, double> >::iterator TimePlan_hhID;

    const double tmax = 8*60.;    // maximal worktime pro day (in min)
    std::vector<double> remaining_time(5, tmax); // work time at each day
    double ti;                    // interview time
    double t_changev;             // time to move in other village
    double t_home;                // time to come back at the start

    unsigned int day, day_it;         // current week day

    bool rainingSeason = 0;         // raining Season


    // plan each week one after another
    for (unsigned int w=0; w<constant::nweeks; ++w)
    {
        if (w>=20 && w <=40) rainingSeason = 1;
        else rainingSeason = 0;

        // delete entities from the previous iteration
        for (day_it=0; day_it<5; ++day_it)
        {
            visitedV[day_it].erase(visitedV[day_it].begin(), visitedV[day_it].end());
            visitedHh[day_it].erase(visitedHh[day_it].begin(), visitedHh[day_it].end());
            remaining_time[day_it] = tmax;
        }

        // villages and households to visit on the current week
        hhToVis = interviewer.routes_weeks[w].households;
        N = interviewer.routes_weeks[w].households.size();

        vToVis.resize(interviewer.routes_weeks[w].villages.size()-2);
        std::copy ( interviewer.routes_weeks[w].villages.begin()+1,
                    interviewer.routes_weeks[w].villages.end()-1,
                    vToVis.begin());
        V = vToVis.size();

        // if there is nothing to be done on the current week
        if (hhToVis.empty()){
            // add 5 empty day plans in schedule of the interviewer
            for (unsigned int d=0; d<5; ++d)
            {
                tmpRoute.villages = visitedV[d];
                tmpRoute.households = visitedHh[d];
                tmpRoute.time = tmax - remaining_time[d];

                interviewer.routes_days.push_back(tmpRoute);
            }
            continue;
        }

        /*
         * sort households in descending order of village IDs they belong to
         */
        std::sort(vToVis.begin(), vToVis.end());    // first sort villages in descending order

        std::vector<std::vector<unsigned int> > village_household_Matrix;
        village_household_Matrix = sort_v_with_hh(vToVis, hhToVis, _households);

        // each week starts at capital (home)
        predV = home;
        visitedV[0].push_back(home);
        day = 0;
        // fill one day afte another
        for (unsigned int v=0; v<V; ++v)    // for each village
        {
            nextV = vToVis[v];   // village of the next household

            while (!village_household_Matrix[v].empty())
            {
                hhID = village_household_Matrix[v].back(); // next household to visite
                village_household_Matrix[v].pop_back();

                // interview time
                TimePlan_hhID = std::find_if(ITimePlan[w].begin(), ITimePlan[w].end(),
                                             [hhID] (std::pair<unsigned int, double> const& element)
                                                { return element.first == hhID;});
                ti = (*TimePlan_hhID).second;

                for (day_it = 0; day_it<=day; ++day_it)
                {
                    predV = visitedV[day_it].back();

                    // travel time
                    t_home    =    rainingSeason *_distmatrixRain[nextV][home] // if interviewer would come back home after this hh
                            + (1-rainingSeason)*_distmatrixDry[nextV][home];
                    t_changev =    rainingSeason *_distmatrixRain[predV][nextV] // travel time between previous village and new village
                            + (1-rainingSeason)*_distmatrixDry[predV][nextV];

                    if (remaining_time[day_it] - (t_changev + ti + t_home) > 0 )
                    {
                        if (predV != nextV)
                            visitedV[day_it].push_back(nextV);
                        visitedHh[day_it].push_back(hhID);
                        remaining_time[day_it] -= t_changev + ti;
                        break;
                    }
                }

                if (day_it>day)
                {
                    day_it = std::min(int(day+1), 4);
                    // if we start a new day and last village in previous day differs from current next
                    if (day!=day_it)// && nextV!=visitedV[day].back())
                    {
                        day = day_it;
                        t_changev =    rainingSeason *_distmatrixRain[home][nextV] // travel time between previous village and new village
                                + (1-rainingSeason)*_distmatrixDry[home][nextV];

                        visitedV[day].push_back(home);
                        remaining_time[day] -= t_changev;
                    }
                    if (predV != nextV)
                        visitedV[day].push_back(nextV);
                    remaining_time[day] -= ti;
                    visitedHh[day].push_back(hhID);

                }

            } // end while
        }   // end for v

        lastV = home;
        firstV = home;
        // save all routes
        for (day_it=1; day_it<5; ++day_it)
        {
            if (visitedV[day_it].empty())
            {
                visitedV[day_it-1].push_back(home);
                t_home =    rainingSeason *_distmatrixRain[lastV][home] // return at capital
                       + (1-rainingSeason)*_distmatrixDry[lastV][home];
                remaining_time[day_it-1] -= t_home;

                tmpRoute.villages = visitedV[day_it-1];
                tmpRoute.households = visitedHh[day_it-1];
                tmpRoute.time = tmax - remaining_time[day_it-1];

                interviewer.routes_days.push_back(tmpRoute);

                continue;
            }

            lastV = visitedV[day_it-1].back();    // last village in the route
            firstV = visitedV[day_it].front();    // last village in the route

            assert (lastV==firstV || firstV==home);
            if (lastV!=firstV && firstV==home)
            {
                visitedV[day_it-1].push_back(home);
                t_home =    rainingSeason *_distmatrixRain[lastV][home] // return at capital
                       + (1-rainingSeason)*_distmatrixDry[lastV][home];
                remaining_time[day_it-1] -= t_home;
            }

            tmpRoute.villages = visitedV[day_it-1];
            tmpRoute.households = visitedHh[day_it-1];
            tmpRoute.time = tmax - remaining_time[day_it-1];

            interviewer.routes_days.push_back(tmpRoute);
        }
        // add last day

        if (!visitedV[day_it-1].empty())
        {
            lastV = visitedV[day_it-1].back();
            visitedV[day_it-1].push_back(home);
            t_home =    rainingSeason *_distmatrixRain[lastV][home] // return at capital
                   + (1-rainingSeason)*_distmatrixDry[lastV][home];
            remaining_time[day_it-1] -= t_home;
        }
        tmpRoute.villages = visitedV[day_it-1];
        tmpRoute.households = visitedHh[day_it-1];
        tmpRoute.time = tmax - remaining_time[day_it-1];

        interviewer.routes_days.push_back(tmpRoute);

    }   // end week

}




/*
 * Initial Solution
 */
void initialsolution(std::vector<stVillage> _villages,           // villages
                     std::vector<stHousehold> _households,         // _households
                     std::vector<stInterviewer>& _interviewer,      // _interviewer
                     std::vector<std::vector<double> >  _distmatrix,// distmatrix
                     std::vector<std::vector<double> >  _distmatrixRain,// distmatrix
                     std::vector<std::vector<std::pair<unsigned int, double> > > _village_household,
                     timeStatistic _TimeInfo
                     )
{

    std::cout << "Construct initial solution" << std::endl << std::endl;

    unsigned int N = _households.size();
    unsigned int K = _interviewer.size();

    std::vector<std::pair<unsigned int, bool> > ToVis(N, std::make_pair(0, true));
    // sort households in decreasing order their interview times
    //    ToVis = specialPresorting(_households);
    //    ToVis = specialPresorting2(_village_household);

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
     * determine interview time of each household in each period
     */
    std::vector<std::vector <std::pair<int, double> > > planITimes_period;
    planITimes_period = planInterviewTimes_period(_households, _TimeInfo);// _households

    // check that we will visit all interviewers in each period
    assert(planITimes_period[0].size() == N);
    assert(planITimes_period[1].size() == N);
    assert(planITimes_period[2].size() == N);

    // ckeck time limit for each  period
    double globalInterviewTime = 0;
    for (unsigned int p=0; p< planITimes_period.size(); ++p)
    {
        double sumITime_period = 0.;
        for (unsigned int h=0; h<planITimes_period[p].size(); ++h)
            sumITime_period += planITimes_period[p][h].second;

        assert (sumITime_period <= K * constant::nweeks * 5 * 8 * 60);
        std::cout << "Period " << p+1 << " summary interview time " << sumITime_period
                  << "/" << K * constant::nweeks * 5 * 8 * 60 << std::endl;
        globalInterviewTime += sumITime_period ;
    }
    std::cout << "Summary : " << globalInterviewTime << std::endl;
    std::cout << std::endl;

    /*
     * for each household set week of interview
     */
    std::vector<std::vector <std::pair<int, double> > > planITimes_week;
    planITimes_week = planInterviewTimes_week(K, _households, _TimeInfo,
                                              planITimes_period);

    globalInterviewTime = 0;
    //check time limit for each week
    for (unsigned int week=0; week< planITimes_week.size(); ++week)
    {
        double sumITime_week = 0.;

        for (unsigned int h=0; h<planITimes_week[week].size(); ++h)
            sumITime_week += planITimes_week[week][h].second;

        assert (sumITime_week <= K * 5 * 8 * 60);
        std::cout << "Week " << week+1 << " summary interview time " << sumITime_week
                  << "/" << K * 5 * 8 * 60 << std::endl;
        globalInterviewTime += sumITime_week ;
    }
    std::cout << "Summary : " << globalInterviewTime << std::endl;
    std::cout << std::endl;


    // make week schedule for each interviewer (only for first period)
    // i.e. planning horizon is 16 weeks

    /*
     *  First period
    */
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "                    First period                              " << std::endl;

    for (unsigned int week=0; week<constant::nweeks; ++week)
    {
        // unvisited hh, _interviewer, _households, distmatrix
        //        planForADay(d, ToVis, _interviewer, _households, _distmatrix);
        weekplan_interviewers( planITimes_week[week], _interviewer, _households, _distmatrix, 0);
        //planForAWeek0( planITimes_week[week], _interviewer, _households, _distmatrix);
    }

    unsigned int totalVisHh = 0;
    double totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        for (unsigned int w=0; w<_interviewer[i].routes_weeks.size(); ++w)
        {
            totalVisHh += _interviewer[i].routes_weeks[w].households.size();
            totalRemainingTime += 5*8*60. -_interviewer[i].routes_weeks[w].time;
        }
    }

    std::cout << "Total number of the visited households in first period: "
              << totalVisHh << std::endl;
    std::cout << " Still to visite "
              << N - totalVisHh << std::endl;
    std::cout << "Total remaining free time: " <<  totalRemainingTime << "min"<< std::endl;

    /*
     *  Second period
    */
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "                   Second period                              " << std::endl;

    for (unsigned int week=constant::nweeks; week<2*constant::nweeks; ++week)
    {
        // unvisited hh, _interviewer, _households, distmatrix
        //        planForADay(d, ToVis, _interviewer, _households, _distmatrix);
        if (week>=20 && week <=40)
            weekplan_interviewers( planITimes_week[week], _interviewer, _households, _distmatrixRain, 1);
        else
            weekplan_interviewers( planITimes_week[week], _interviewer, _households, _distmatrix, 0);
        //planForAWeek0( planITimes_week[week], _interviewer, _households, _distmatrix);
    }

    totalVisHh = 0;
    totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        for (unsigned int w=constant::nweeks; w<_interviewer[i].routes_weeks.size(); ++w)
        {
            totalVisHh += _interviewer[i].routes_weeks[w].households.size();
            totalRemainingTime += 5*8*60. -_interviewer[i].routes_weeks[w].time;
        }
    }

    std::cout << "Total number of the visited households in second period: "
              << totalVisHh << std::endl;
    std::cout << " Still to visite "
              << N - totalVisHh << std::endl;
    std::cout << "Total remaining free time: " <<  totalRemainingTime << "min"<< std::endl;


    /*
     *  Third period
    */
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "                    Third period                              " << std::endl;
    for (unsigned int week=2*constant::nweeks; week<3*constant::nweeks; ++week)
    {
        // unvisited hh, _interviewer, _households, distmatrix
        //        planForADay(d, ToVis, _interviewer, _households, _distmatrix);
        if (week>=20 && week <=40)
            weekplan_interviewers( planITimes_week[week], _interviewer, _households, _distmatrixRain, 0);
        else
            weekplan_interviewers( planITimes_week[week], _interviewer, _households, _distmatrix, 1);
        //planForAWeek0( planITimes_week[week], _interviewer, _households, _distmatrix);
    }

    totalVisHh = 0;
    totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        for (unsigned int w=2*constant::nweeks; w<_interviewer[i].routes_weeks.size(); ++w)
        {
            totalVisHh += _interviewer[i].routes_weeks[w].households.size();
            totalRemainingTime += 5*8*60. -_interviewer[i].routes_weeks[w].time;
        }
    }

    std::cout << "Total number of the visited households in third period: "
              << totalVisHh << std::endl;
    std::cout << " Still to visite "
              << N - totalVisHh << std::endl;
    std::cout << "Total remaining free time: " <<  totalRemainingTime << "min"<< std::endl;

    std::cout << std::endl;


    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
    std::cout << "                    Plan for a day                            " << std::endl;

    for (unsigned int i=0; i<_interviewer.size(); ++i)
        dayplan_interviewer( _interviewer[i], planITimes_week, _households, _distmatrix, _distmatrixRain);

}
