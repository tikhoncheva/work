#include "hrd/initialsolution.h"

bool comparator ( const std::pair<double, int>& a,
                  const std::pair<double, int>& b)
{
    return a.first < b.first;
}

bool isZero (const unsigned int a)
{
    return a==0;
}

/*
 * returns position on the minimum positive value in the container
*/
template <class ForwardIterator>
ForwardIterator find_posmin( ForwardIterator first, ForwardIterator last )
{
    if (first==last)
        return last-1;

    ForwardIterator smallest = first;

    while (++first!=last)
    {
        if ((*first>=0 && *first<*smallest) || *smallest<0)
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

std::vector<std::pair<unsigned int, bool> > specialPresorting(std::vector<stVillage> villages,
                                                              std::vector<std::vector<unsigned int> > _village_household)
{
    unsigned int V = villages.size();
    const unsigned int Nouna = 142-100-1; // Capital (Nouna)
    // set Nouna to the map center
    double x0 = villages[Nouna].coord.first;
    double y0 = villages[Nouna].coord.second;

    // sort Villages in increasing order of the angle they make with the capital
    std::vector<std::pair<double,int> > angles(V, std::make_pair(0.,0));
    for (unsigned int i=0; i<V; ++i)
    {
        double x = villages[i].coord.first - x0;
        double y = villages[i].coord.second - y0;
        angles[i] = std::make_pair(atan2(y,x),i);
    }
    std::sort(angles.begin(), angles.end(), comparator);

    // sort housesholds in the order of villages
    std::vector<std::pair<unsigned int, bool> > toDo;

    for (unsigned int i=0; i<V; ++i)    // for each village
        for (unsigned int j=0; j < _village_household[i].size(); ++j)
            toDo.push_back(std::make_pair(_village_household[i][j], true));

    return toDo;
}

/*
 * ------------------------------------------------------------------------------------
 */

void planForADay (const unsigned int day,
                  std::vector<std::pair<unsigned int, bool> >& ToVis,
                  std::vector<stInterviewer>& _interviewer,             // _interviewer
                  const std::vector<stHousehold> _household,                 // _households
                  const std::vector<std::vector<double> > _distmatrix        // distmatrix
                  )
{
    std::cout << "Day " << day << std::endl;

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

    const double tmax = 480.0;    // maximal worktime pro day (in min)
    double t_work = 0.;          // worktime of one _interviewer
    double ti;                    // interview time
    double t_changev;              // time to move in other village
    double t_home;                 // time to come back at the start


    // pick randomly first unvisited household (an the beginning of period all hh need to be visited)
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
        ToVis[i%N].second = false;


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
            //            std::cout << hhID << "/" << nextV << "(+" << t_changev << "+" << ti << ")->";
            visitedHh.push_back(hhID);
        }
        else
        {
            // finish current route
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
        predV = nextV;
    }
    std::cout << std::endl;
}

/*
 * Do not close routes after a hh didn't pass in
 */
void planForADay2 (const unsigned int day,
                   std::vector<std::pair<unsigned int, bool> >& ToVis,
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

    const double tmax = 480.0;    // maximal worktime pro day (in min)
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

    std::cout << "Remaining time after iteration " << 0 << std::endl;
    for (unsigned int k=0; k<K; ++k)
        std::cout << remaining_time[k] << " ";
    std::cout << std::endl;

    // pick randomly first unvisited household (an the beginning of period all hh need to be visited)
    first = rand()%(N-1);
    while (!ToVis[first].second)
        first= (first+1)%N;

    predV = home;   // start allways from home

    unsigned int i=first;
    bool allFull = false;
    // for j, j+1, ..., N-1, 0, 1, ..., j-1
//    for (unsigned int i=first; i<N+first; ++i)
    while(++i<N+first && !allFull)
    {
        // select a household
        if (!ToVis[i%N].second)    //skip already visited citys
            continue;

        hhID = ToVis[i%N].first;

        nextV = _household[hhID].villageID - 101;

        // if hh is not in the previous village, travel time is not 0
        if (predV !=nextV)
            t_changev = _distmatrix[predV][nextV];
        else
            t_changev = 0;

        // Interview time
        ti = (1-_household[hhID].type)*10*_household[hhID].nPersons   // 10 min for standart interview
           +    _household[hhID].type *30*_household[hhID].nPersons;  // 30 min for special cases

        // if interviewer would come back home after this hh
        t_home = _distmatrix[nextV][home];

        // for each Interviewer calculate the remaining time, if he takes the hh
        for (unsigned int k=0; k<K; ++k)
        {
            remaining_time_new[k] = remaining_time[k] - (t_changev + ti + t_home);
        }


        // find min remaining time over all positive values
        unsigned int  k_min = find_posmin(remaining_time_new.begin(), remaining_time_new.end()) - remaining_time_new.begin();
        std::cout << "needed time" << t_changev + ti << "+" << t_home
                                   << "=" << t_changev + ti + t_home<< std::endl;

        if (k_min==K)
        {
            std::cout << std::endl;
            for (unsigned int k=0; k<K; ++k)
                std::cout << remaining_time[k] << " ";
            std::cout << std::endl;

//            std::cout << "needed time" << t_changev + ti << "+" << t_home
//                                       << "=" << t_changev + ti + t_home<< std::endl;
            for (unsigned int k=0; k<K; ++k)
                std::cout << remaining_time_new[k] << " ";
            std::cout << std::endl;
            std::cout << std::endl;
            continue;
        }
//        if (k_min==K && remaining_time_new[k_min]<0) // check if not interviewers are completely full
//            allFull = true;


        remaining_time[k_min] = remaining_time[k_min] - (t_changev + ti);
        visitedHh[k_min].push_back(hhID);
        if (predV !=nextV)
        {
            visitedV[k_min].push_back(nextV);
        }

        ToVis[i%N].second = false; // set hh as visited

//        std::cout << "Remaining time after iteration " << 0 << std::endl;
//        for (unsigned int k=0; k<K; ++k)
//            std::cout << remaining_time[k] << " ";
//        std::cout << std::endl;
        if (std::remove_if(remaining_time.begin(), remaining_time.end(), isZero) == remaining_time.begin())
           allFull = true;

    }

    // close all routes: add time to go home
    // assign routes to Interviewer
    for (unsigned int k=0; k<K; ++k)
    {
        unsigned int lastV = visitedV[k][visitedV[k].size()-1];    // last village in the route
        remaining_time[k] -= _distmatrix[lastV][home];
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
                     std::vector<std::vector<double> >  _distmatrix,// distmatrix
                     std::vector<std::vector<unsigned int> > _village_household
                     )
{
    unsigned int V = _villages.size();
    unsigned int N = _households.size();



    std::vector<std::pair<unsigned int, bool> > ToVis(N, std::make_pair(0, true));
    // sort Villages in increasing order of the angle they make with the capital
    ToVis = specialPresorting(_villages, _village_household);

    // erase previous tours
    for (unsigned int k=0; k < _interviewer.size(); ++k)
    {
        _interviewer[k].nRoutes = 0;
        _interviewer[k].routes.erase(_interviewer[k].routes.begin(),
                                     _interviewer[k].routes.end());
    }

    // plan nex day
    for (unsigned int d=0; d<1; d++) // planning horizon 80 weekdays
    {
        // day, unvisited hh, _interviewer, _households, distmatrix
//        planForADay(d, ToVis, _interviewer, _households, _distmatrix);
        planForADay2(d, ToVis, _interviewer, _households, _distmatrix);

        unsigned int sumVisHh = 0;
        for (unsigned int i=0; i<_interviewer.size(); ++i)
            sumVisHh += _interviewer[i].routes[d].households.size();
    }

    for (unsigned int i=0; i<_interviewer.size(); ++i)
    {
        std::cout << "Interviewer " << i+1 << std::endl;
        for (unsigned int d=0; d<_interviewer[i].routes.size(); ++d)
        {
            std::cout << "day " << d << ": visitid households " << _interviewer[i].routes[d].households.size() << std::endl;
//            for (unsigned int r=0; r<_interviewer[i].routes[d].households.size(); ++r)
//                std::cout << _interviewer[i].routes[d].households[r] << "->";
            std::cout << "  work time = " << _interviewer[i].routes[d].time << std::endl;
        }
    }

    //    std::cout << "Unvisited househols:" << std::endl;
    //    for (unsigned int i=0; i<N; ++i)
    //        if (ToVis[i])
    //        std::cout << "Household " << i+1 << " ";
    //    std::cout << std::endl;

    //    std::cout << "Routes length:" << std::endl;
    //    for (unsigned int i=0; i<_interviewer.size(); ++i)
    //    {
    //        std::cout << _interviewer[i].routes.size();
    //        if (_interviewer[i].routes.size()>0)
    //            std::cout << "\t_interviewer " << i+1
    //                      << "  route length " << _interviewer[i].routes[0].villages.size();
    //        std::cout << std::endl;
    //    }
    //    std::cout << std::endl;
}
