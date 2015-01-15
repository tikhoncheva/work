#include "hrd/initialsolution.h"

bool compareTime ( const std::pair<double, int>& a,
                     const std::pair<double, int>& b)
{
    return a.first > b.first;
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
specialPresorting2( std::vector<std::vector<std::pair<double, unsigned int> > > _village_household)
{

    unsigned int V = _village_household.size();

    // sort Villages in decreasing order of the interview times all it households
    std::vector<std::pair<double,int> > itime(V, std::make_pair(0.,0));
    for (unsigned int i=0; i<V; ++i)
    {
        double itime_i = 0.;
        std::sort(_village_household[i].begin(), _village_household[i].end(), compareTime);

        itime_i = std::accumulate(_village_household[i].begin(),
                                  _village_household[i].end(),
                                  0.0,
                                  [](double sum, const std::pair<double, int>& b){ return sum + b.first;}
                                  );

        itime[i] = std::make_pair(itime_i,i);
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


void planForAWeek0 (std::vector<std::pair<unsigned int, bool> >& ToVis,
                   std::vector<stInterviewer>& _interviewer,             // _interviewer
                   const std::vector<stHousehold> _household,                 // _households
                   const std::vector<std::vector<double> > _distmatrix        // distmatrix
                   )
{
    unsigned int N = ToVis.size();
    const unsigned int K = _interviewer.size();  // maximal number of available _interviewers

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int first; //start household
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int k; // route/_interviewer number
    unsigned int hhID; // hhID

    std::vector<unsigned int> visitedV; // visited villages
    std::vector<unsigned int> visitedHh;// visited _households
    stRoute tmpRoute;

    const double tmax = 5*8*60.0;    // maximal worktime pro day (in min)
    double t_work = 0.;          // worktime of one _interviewer
    double ti;                     // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start

    // pick randomly first unvisited household
    first = rand()%(N-1);
    while (!ToVis[first].second)
        first= (first+1)%N;

    k = 1;                         // start planning for the first _interviewer
    predV = home;
    visitedV.push_back(home);      // start from the capital

    // for j, j+1, ..., N-1, 0, 1, ..., j-1
    for (unsigned int i=first; i<N+first; ++i)
    {
        if (!ToVis[i%N].second)    //skip already visited citys
            continue;


        hhID = ToVis[i%N].first;
        nextV = _household[hhID].villageID - 101;

        // if hh is not in the previous village
        if (predV !=nextV)
        {
            t_changev = _distmatrix[predV][nextV];
            visitedV.push_back(nextV);
        }
        else
            t_changev = 0;

        // Interview time
        ti = (1-_household[hhID].type)*10*_household[hhID].nPersons   // 10 min for standart interview
           +    _household[hhID].type *30*_household[hhID].nPersons;  // 30 min for special cases

        t_home = _distmatrix[nextV][home];

        if (t_work + t_changev + ti + t_home < tmax)
        {
            t_work += t_changev + ti; // we don't need to come back now
            visitedHh.push_back(hhID);
        }
        else
        {
            // interviewer k does not have any free time
            t_work += _distmatrix[predV][home];// return back from previous location
            visitedV.push_back(home);

            // save route in _interviewer k
            tmpRoute.villages = visitedV;
            tmpRoute.households = visitedHh;
            tmpRoute.time = t_work;
            _interviewer[k-1].routes.push_back(tmpRoute);

            // start new route
            //k = std::min(k+1,K);
            k = k+1;
            if (k > K)
            {
                break;
            }

            visitedV.erase(visitedV.begin()+1, visitedV.end());
            visitedHh.erase(visitedHh.begin(), visitedHh.end());
            // new route starts with the current hh,
            // which previous interviewer could not visit any more
            visitedV.push_back(nextV);
            visitedHh.push_back(hhID);
            t_work= _distmatrix[home][nextV] + ti;

        }

        ToVis[i%N].second = false;
        predV = nextV;
    }


    std::cout << std::endl;
}


void fillTheGaps0 (std::vector<std::pair<unsigned int, bool> >& ToVis,
                   std::vector<stInterviewer>& _interviewer,             // _interviewer
                   const std::vector<stHousehold> _household,                 // _households
                   const std::vector<std::vector<double> > _distmatrix        // distmatrix
                   )
{
    /*
    unsigned int N = ToVis.size();
    const unsigned int K = _interviewer.size();  // maximal number of available interviewers

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int first; //start household
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    const double tmax = 5*8*60.;    // maximal worktime pro week (in min)
    double ti;                     // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start

    std::vector<double> remaining_time;
    std::vector<double> remaining_time_new;


    remaining_time.resize(K);
    remaining_time_new.resize(K);
    for (unsigned int k=0; k<K; ++k)
    {

        remaining_time[k] = tmax;
    }

    // go through all citys and try to add them in already existing routes
    first = 0;

    unsigned int i=first;
    bool allFull = false;

    while( ++i<N && !allFull)   // ~ for j, j+1, ..., N-1, 0, 1, ..., j-1
    {
        if (!ToVis[i%N].second)    //skip already visited citys
            continue;

        hhID = ToVis[i%N].first;                    // next household to visite
        nextV = _household[hhID].villageID - 101;   // village of the next household

        // Interview time
        ti = (1-_household[hhID].type)*10*_household[hhID].nPersons   // 10 min for standart interview
           +    _household[hhID].type *30*_household[hhID].nPersons;  // 30 min for special cases

        // if interviewer would come back home after this hh
        t_home = _distmatrix[nextV][home];

        // for each Interviewer calculate the remaining time, if he takes the hh
        for (unsigned int k=0; k<K; ++k)
        {
            predV = visitedV[k].back(); // previous village in tour k
            // travel time between previous village and new village
            t_changev = _distmatrix[predV][nextV];
            remaining_time_new[k] = remaining_time[k] - (t_changev + ti + t_home);
        }

        // find min remaining time over all positive values
        unsigned int  k_min = find_posmin(remaining_time_new.begin(), remaining_time_new.end())
                            - remaining_time_new.begin();

        if (k_min==K)   // all values in remaining_time_ are negative
            continue;

        predV= visitedV[k_min].back();
        t_changev = _distmatrix[predV][nextV];

        remaining_time[k_min] = remaining_time_new[k_min] + t_home; // we dont want to come back immediately
        visitedHh[k_min].push_back(hhID);

        if (predV !=nextV)
            visitedV[k_min].push_back(nextV);

        ToVis[i%N].second = false; // set hh as visited

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
*/

}

/*
 * Do not close routes after a hh didn't pass in
 */
void planForAWeek (std::vector<std::pair<unsigned int, bool> >& ToVis,
                   std::vector<stInterviewer>& _interviewer,             // _interviewer
                   const std::vector<stHousehold> _household,                 // _households
                   const std::vector<std::vector<double> > _distmatrix        // distmatrix
                   )
{
    unsigned int N = ToVis.size();
    const unsigned int K = _interviewer.size();  // maximal number of available interviewers

    const unsigned int home = 142-101; // Capital (Nouna)
    unsigned int first; //start household
    unsigned int predV; // predecessor of the hh j in the tour
    unsigned int nextV; // next village to go

    unsigned int hhID; // hhID

    std::vector<std::vector<unsigned int> > visitedV; // visited villages
    std::vector<std::vector<unsigned int> > visitedHh;// visited _households
    stRoute tmpRoute;

    const double tmax = 5*8*60.;    // maximal worktime pro week (in min)
    double ti;                    // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start

    std::vector<double> remaining_time;
    std::vector<double> remaining_time_new;

    // each route starts at capital (home)
    visitedV.resize(K);
    visitedHh.resize(K);
    remaining_time.resize(K);
    remaining_time_new.resize(K);
    for (unsigned int k=0; k<K; ++k)
    {
        visitedV[k].push_back(home);
        remaining_time[k] = tmax;
    }

    // pick randomly first unvisited household (an the beginning of period all hh need to be visited)
    first = rand()%(N-1);
    while (!ToVis[first].second)
        first= (first+1)%N;

    unsigned int i=first;
    bool allFull = false;

    while( ++i<(N+first) && !allFull)   // ~ for j, j+1, ..., N-1, 0, 1, ..., j-1
    {
        // select a household
        if (!ToVis[i%N].second)    //skip already visited citys
            continue;

        hhID = ToVis[i%N].first;                    // next household to visite
        nextV = _household[hhID].villageID - 101;   // village of the next household

        // Interview time
        ti = (1-_household[hhID].type)*10*_household[hhID].nPersons   // 10 min for standart interview
           +    _household[hhID].type *30*_household[hhID].nPersons;  // 30 min for special cases

        // if interviewer would come back home after this hh
        t_home = _distmatrix[nextV][home];

        // for each Interviewer calculate the remaining time, if he takes the hh
        for (unsigned int k=0; k<K; ++k)
        {
            predV = visitedV[k].back(); // previous village in tour k
            // travel time between previous village and new village
            t_changev = _distmatrix[predV][nextV];
            remaining_time_new[k] = remaining_time[k] - (t_changev + ti + t_home);
        }

        // find min remaining time over all positive values
        unsigned int  k_min = find_posmin(remaining_time_new.begin(), remaining_time_new.end())
                            - remaining_time_new.begin();

        if (k_min==K)   // all values in remaining_time_ are negative
            continue;

        predV= visitedV[k_min].back();
        t_changev = _distmatrix[predV][nextV];

        remaining_time[k_min] = remaining_time_new[k_min] + t_home; // we dont want to come back immediately
        visitedHh[k_min].push_back(hhID);

        if (predV !=nextV)
            visitedV[k_min].push_back(nextV);

        ToVis[i%N].second = false; // set hh as visited

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


}

/*
 * Initial Solution
 */
void initialsolution(std::vector<stVillage> _villages,           // villages
                     std::vector<stHousehold> _households,         // _households
                     std::vector<stInterviewer>& _interviewer,      // _interviewer
                     std::vector<stInterviewer>& _cleaner,      // _interviewer
                     std::vector<std::vector<double> >  _distmatrix,// distmatrix
                     std::vector<std::vector<std::pair<double, unsigned int> > > _village_household
                     )
{
    unsigned int N = _households.size();
    unsigned int weeks = 16;

    std::vector<std::pair<unsigned int, bool> > ToVis(N, std::make_pair(0, true));
    // sort households in decreasing order their interview times
//    ToVis = specialPresorting(_households);
    ToVis = specialPresorting2(_village_household);

    // erase previous tours
    for (unsigned int k=0; k < _interviewer.size(); ++k)
    {
        _interviewer[k].nRoutes = 0;
        _interviewer[k].routes.erase(_interviewer[k].routes.begin(),
                                     _interviewer[k].routes.end());
    }

    // plan nex week
    unsigned int totalVisHh = 0;
    for (unsigned int w=0; w<weeks; ++w) // planning horizon is 16 weeks
    {
        // unvisited hh, _interviewer, _households, distmatrix
        //        planForADay(d, ToVis, _interviewer, _households, _distmatrix);
        //planForAWeek( ToVis, _interviewer, _households, _distmatrix);
        planForAWeek0( ToVis, _interviewer, _households, _distmatrix);
        //fillTheGaps0(ToVis, _interviewer, _households, _distmatrix);

//        unsigned int sumVisHh = 0;
//        for (unsigned int i=0; i<_interviewer.size(); ++i)
//            sumVisHh += _interviewer[i].routes[week].households.size();

//        std::cout << "# of Households, visited at the week " << week << " of the first period: "
//                  << sumVisHh << std::endl;
//        totalVisHh += sumVisHh;
    }

    double totalRemainingTime = 0.;
    for (unsigned int i=0; i<_interviewer.size(); ++i)
        for (unsigned int w=0; w< 16; ++w)
        {
            totalVisHh += _interviewer[i].routes[w].households.size();
            totalRemainingTime +=_interviewer[i].routes[w].time;
        }

    std::cout << "Total number of the visited households in first period: "
              << totalVisHh << std::endl;
    std::cout << " Still to visite "
              << N - totalVisHh << std::endl;

    std::cout << "Total remaining free time: " <<  totalRemainingTime << "min"<< std::endl;

    double totalReamingITime = 0.;
    for (unsigned int h=0; h<N; ++h)
        if (ToVis[h].second)
            totalReamingITime += (1-_households[h].type)*10*_households[h].nPersons   // 10 min for standart interview
                               +    _households[h].type *30*_households[h].nPersons;  // 30 min for special cases

    std::cout<< "Total remaining interview time:" << totalReamingITime << "min" << std::endl;
}
