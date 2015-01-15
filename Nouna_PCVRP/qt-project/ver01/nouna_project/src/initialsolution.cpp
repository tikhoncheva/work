#include "hrd/initialsolution.h"

bool comparator_iT ( const std::pair<double, int>& a,
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

    double sum = 0.0;
    for (unsigned int i=0; i<N; ++i)
    {
        double it = (1-_households[i].type)*10*_households[i].nPersons  // 10 min for standart interview
                  +    _households[i].type *30*_households[i].nPersons;  // 30 min for special cases
        sum += it;
        iT[i] = std::make_pair(it,i);
    }

    std::cout << "Complete interview time " << sum << std::endl;

    std::sort(iT.begin(), iT.end(), comparator_iT);

    // mark all households as unvisited
     std::vector<std::pair<unsigned int, bool> > toDo;

     for (unsigned int i=0; i<N; ++i)
        toDo.push_back(std::make_pair( iT[i].second, true));

    return toDo;
}

/*
 * ------------------------------------------------------------------------------------
 */

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

    const double tmax = 5*8*60;    // maximal worktime pro week (in min)
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

    unsigned int i=0;   // start with the first household in the ToVis list
    bool allFull = false;

    while( ++i<N && !allFull)   // ~ for 0, 1, ..., N-1
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
                     std::vector<std::vector<unsigned int> > _village_household
                     )
{
    unsigned int N = _households.size();


    std::vector<std::pair<unsigned int, bool> > ToVis(N, std::make_pair(0, true));
    // sort households in decreasing order their interview times
    ToVis = specialPresorting(_households);

    // erase previous tours
    for (unsigned int k=0; k < _interviewer.size(); ++k)
    {
        _interviewer[k].nRoutes = 0;
        _interviewer[k].routes.erase(_interviewer[k].routes.begin(),
                                     _interviewer[k].routes.end());
    }

    // plan nex week
    unsigned int totalVisHh = 0;
    for (unsigned int week=0; week<16; week++) // planning horizon is 16 weeks
    {
        // unvisited hh, _interviewer, _households, distmatrix
        //        planForADay(d, ToVis, _interviewer, _households, _distmatrix);
        planForAWeek( ToVis, _interviewer, _households, _distmatrix);

        unsigned int sumVisHh = 0;
        for (unsigned int i=0; i<_interviewer.size(); ++i)
            sumVisHh += _interviewer[i].routes[week].households.size();

        std::cout << "# of Households, visited at the week " << week << " of the first period: "
                  << sumVisHh << std::endl;
        totalVisHh += sumVisHh;
    }

    std::cout << "Total number of the visited households in first period: "
              << totalVisHh << std::endl;

    std::cout << " Still to visite "
              << N - totalVisHh << std::endl;

}
