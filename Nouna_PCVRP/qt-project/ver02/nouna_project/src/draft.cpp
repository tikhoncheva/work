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

        _interviewer[i].routes_weeks.push_back(tmpRoute);
    }

    std::cout << "actually needed " << k << " interviewer" << std::endl;
}

