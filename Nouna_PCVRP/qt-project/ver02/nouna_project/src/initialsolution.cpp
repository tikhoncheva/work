#include "hrd/initialsolution.h"

bool compareTime ( const std::pair<int, double>& a,
                   const std::pair<int, double>& b)
{
    return a.second > b.second;
}

bool isZero (const unsigned int a)
{
    return a==0;
}

bool visited (std::pair<unsigned int, bool> a)
{
    return !a.second;
}

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

/*
 * ------------------------------------------------------------------------------------
 */

std::vector<std::pair<unsigned int, bool> > specialPresorting(std::vector<stHousehold> _households)
{
    unsigned int N = _households.size();
    std::vector<std::pair<double,int> > iT(N, std::make_pair(0.,0));    // interview time

    for (unsigned int i=0; i<N; ++i)
    {
        double it = (1-_households[i].type)*10*_households[i].nPersons  // 10 min for standart interview
                +    _households[i].type *30*_households[i].nPersons;  // 30 min for special cases
        iT[i] = std::make_pair(it,i);
    }

    std::sort(iT.begin(), iT.end(), compareTime);

    // mark all households as unvisited
    std::vector<std::pair<unsigned int, bool> > toDo;

    for (unsigned int i=0; i<N; ++i)
        toDo.push_back(std::make_pair( iT[i].second, true));

    return toDo;
}


std::vector<std::pair<unsigned int, bool> >
specialPresorting2( std::vector<std::vector<std::pair<unsigned int, double> > > _village_household)
{

    unsigned int V = _village_household.size();

    // sort Villages in decreasing order of the interview times all it households
    std::vector<std::pair<unsigned int, double> > itime(V, std::make_pair(0,0.));
    for (unsigned int i=0; i<V; ++i)
    {
        double itime_i = 0.;
        std::sort(_village_household[i].begin(), _village_household[i].end(), compareTime);

        itime_i = std::accumulate(_village_household[i].begin(),
                                  _village_household[i].end(),
                                  0.0,
                                  [](double sum, const std::pair<unsigned int, double>& b){ return sum + b.second;}
        );

        itime[i] = std::make_pair(i, itime_i);
    }
    std::sort(itime.begin(), itime.end(), compareTime);

    // sort housesholds in the order of villages
    std::vector<std::pair<unsigned int, bool> > toDo;

    for (unsigned int i=0; i<V; ++i)    // for each village
        for (unsigned int h=0; h < _village_household[i].size(); ++h)
            toDo.push_back(std::make_pair(_village_household[i][h].second, true));

    return toDo;
}
/*
 * ------------------------------------------------------------------------------------
 */




/*
 * Do not close routes after a hh didn't pass in
 */
int planForAWeek (std::vector <std::pair<int, double> > ToVisit,
                  std::vector<stInterviewer>& _interviewer,             // _interviewer
                  const std::vector<stHousehold> _household,                 // _households
                  const std::vector<std::vector<double> > _distmatrix        // distmatrix
                  )
{
    unsigned int N = ToVisit.size();
    const unsigned int K = _interviewer.size();  // maximal number of available interviewers

    const unsigned int home = 142-101; // Capital (Nouna)

    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    std::vector<std::vector<unsigned int> > visitedV(K); // visited villages
    std::vector<std::vector<unsigned int> > visitedHh(K);// visited _households
    stRoute tmpRoute;

    const double tmax = 5*8*60.;    // maximal worktime pro week (in min)
    double ti;                    // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start

    int nUnvisited = 0;

    std::vector<double> remaining_time (K, tmax);
    std::vector<double> remaining_time_new (K);

    bool allFull = false;

    // each route starts at capital (home)
    for (unsigned int k=0; k<K; ++k)
        visitedV[k].push_back(home);



    for (unsigned int i=0; i<N && !allFull; ++i)
    {

        hhID = ToVisit[i].first;                    // next household to visite
        nextV = _household[hhID].villageID - 101;   // village of the next household

        ti = ToVisit[i].second;     // Interview time
        t_home = _distmatrix[nextV][home]; // if interviewer would come back home after this hh

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

        predV= visitedV[k_min].back();
        t_changev = _distmatrix[predV][nextV];
        remaining_time[k_min] = remaining_time_new[k_min] + t_home; // we dont want to come back immediately
        visitedHh[k_min].push_back(hhID);

        if (predV !=nextV)
            visitedV[k_min].push_back(nextV);

        allFull = std::all_of(remaining_time.begin(), remaining_time.end(), isZero);

    }

    // close all routes: add time to go home
    // assign routes to Interviewer
    for (unsigned int k=0; k<K; ++k)
    {
        predV = visitedV[k][visitedV[k].size()-1];    // last village in the route

        remaining_time[k] -= _distmatrix[predV][home];
        visitedV[k].push_back(home);

        tmpRoute.villages = visitedV[k];
        tmpRoute.households = visitedHh[k];
        tmpRoute.time = tmax - remaining_time[k];

        _interviewer[k].routes.push_back(tmpRoute);
    }

    return nUnvisited;
}

void planForAWeek0 (std::vector <std::pair<int, double> > ToVisit,
                    std::vector<stInterviewer>& _interviewer,             // _interviewer
                    const std::vector<stHousehold> _household,                 // _households
                    const std::vector<std::vector<double> > _distmatrix        // distmatrix
                    )
{
    unsigned int N = ToVisit.size();  // number of housesholds to visit at the given week
    std::cout << "To visit: " << N << "households" << std::endl;
    std::vector<bool> visited(N, false);

    const unsigned int K = _interviewer.size();  // maximal number of available _interviewers

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int k; // route/_interviewer number
    unsigned int hhID; // hhID

    std::vector<std::vector<unsigned int> > visitedV(K); // visited villages
    std::vector<std::vector<unsigned int> > visitedHh(K);// visited _households


    stRoute tmpRoute;

    const double tmax = 5*8*60.0;    // maximal worktime pro week (in min)
    double ti;                     // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start
    std::vector<double> remaining_time(K, tmax);

    // each route starts at capital (home)
    for (k=0; k<K; ++k)
        visitedV[k].push_back(home);

    k = 0;                         // start planning for the first interviewer
    predV = home;

    for (unsigned int i=0; i<N; ++i)
    {

        hhID = ToVisit[i].first;
        nextV = _household[hhID].villageID - 101;

        if (predV !=nextV)  // save village if we need to move to other village
            visitedV[k].push_back(nextV);

        t_changev = _distmatrix[predV][nextV];
        ti = ToVisit[i].second; // Interview time
        t_home = _distmatrix[nextV][home];

        remaining_time[k] -= t_changev + ti + t_home;

        if (remaining_time[k]>0)
        {
            remaining_time[k] += t_home; // we don't need to return back now
            visitedHh[k].push_back(hhID);
        }
        else
        {
            // start new route
            k = std::min(k+1,K);

            // new route starts with the current hh,
            // which previous interviewer could not visit any more
            visitedV[k].push_back(home);
            visitedV[k].push_back(nextV);
            visitedHh[k].push_back(hhID);
            remaining_time[k] -= _distmatrix[home][nextV] + ti;

        }

        visited[i] = true;
        predV = nextV;
    }

    // close all routes: add time to go home
    // assign routes to Interviewer
    for (unsigned int i=0; i<K; ++i)
    {
        predV = visitedV[i].back();    // last village in the route

        remaining_time[i] -= _distmatrix[predV][home];
        visitedV[i].push_back(home);

        tmpRoute.villages = visitedV[i];
        tmpRoute.households = visitedHh[i];
        tmpRoute.time = tmax - remaining_time[i];

        _interviewer[i].routes.push_back(tmpRoute);
    }

    std::cout << "actually needed " << k << " interviewer" << std::endl;
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------

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

std::vector<std::vector <std::pair<int, double> > > planInterviewTimes_week(
        std::vector<stHousehold> _households, // _households
        timeStatistic _TimeInfo,
        std::vector<std::vector <std::pair<int, double> > > planForPeriod)
{
    unsigned int N = _households.size();
    unsigned int hhID;
    double itime;
    unsigned int week;

    double meanLongITime = _TimeInfo.sLongITime1 / (3.*constant::nweeks);

    std::vector<bool> ToVis(N, true);

    std::vector<std::vector <std::pair<int, double> > > planITimes;
    planITimes.resize(constant::nweeks * 3);


    std::vector< double > remainingTime (constant::nweeks, meanLongITime);
    std::vector< double > remainingTime_new(constant::nweeks, 0.);

    std::vector<std::pair<int, double> >::iterator hhNode;

    planITimes.resize(constant::nweeks * 3);

    // plan long interviews first
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

    std::cout << "Construct initial solution" << std::endl;

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
        _interviewer[k].routes.erase(_interviewer[k].routes.begin(),
                                     _interviewer[k].routes.end());
    }

    /*
     * determine interview time of each household in each period
     */
    std::vector<std::vector <std::pair<int, double> > > planITimes_period;
    planITimes_period = planInterviewTimes_period(_households, _TimeInfo);// _households

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
    planITimes_week = planInterviewTimes_week( _households, _TimeInfo,
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
        planForAWeek( planITimes_week[week], _interviewer, _households, _distmatrix);
        //planForAWeek0( planITimes_week[week], _interviewer, _households, _distmatrix);
    }

    unsigned int totalVisHh = 0;
    double totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        for (unsigned int w=0; w<_interviewer[i].routes.size(); ++w)
        {
            totalVisHh += _interviewer[i].routes[w].households.size();
            totalRemainingTime += 5*8*60. -_interviewer[i].routes[w].time;
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
            planForAWeek( planITimes_week[week], _interviewer, _households, _distmatrixRain);
        else
            planForAWeek( planITimes_week[week], _interviewer, _households, _distmatrix);
        //planForAWeek0( planITimes_week[week], _interviewer, _households, _distmatrix);
    }

    totalVisHh = 0;
    totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        for (unsigned int w=constant::nweeks; w<_interviewer[i].routes.size(); ++w)
        {
            totalVisHh += _interviewer[i].routes[w].households.size();
            totalRemainingTime += 5*8*60. -_interviewer[i].routes[w].time;
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
            planForAWeek( planITimes_week[week], _interviewer, _households, _distmatrixRain);
        else
            planForAWeek( planITimes_week[week], _interviewer, _households, _distmatrix);
        //planForAWeek0( planITimes_week[week], _interviewer, _households, _distmatrix);
    }

    totalVisHh = 0;
    totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        for (unsigned int w=2*constant::nweeks; w<_interviewer[i].routes.size(); ++w)
        {
            totalVisHh += _interviewer[i].routes[w].households.size();
            totalRemainingTime += 5*8*60. -_interviewer[i].routes[w].time;
        }
    }

    std::cout << "Total number of the visited households in third period: "
              << totalVisHh << std::endl;
    std::cout << " Still to visite "
              << N - totalVisHh << std::endl;
    std::cout << "Total remaining free time: " <<  totalRemainingTime << "min"<< std::endl;

    std::cout << std::endl;

}
