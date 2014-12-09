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

void planForADay (unsigned int day,
                  std::vector<std::pair<unsigned int, bool> >& ToVis,
                  std::vector<stInterviewer>& _interviewer,             // _interviewer
                  std::vector<stHousehold> _household,                 // _households
                  std::vector<std::vector<double> > _distmatrix        // distmatrix
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
    double t_work = tmax;          // worktime of one _interviewer
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
    std::cout << "Route " << k << " :" << home << "->";

    // for j, j+1, ..., N-1, 0, 1, ..., j-1
    for (unsigned int i=first; i<N+first; ++i)
    {
        if (!ToVis[i%N].second)    //skip already visited citys
            continue;
        hhID = ToVis[i%N].first;
        ToVis[i%N].second = false;


        nextV = _household[hhID].villageID - 101;

        // if _interviewer k goes to another village
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

        if (t_work - t_changev - ti - t_home >=0)
        {
            t_work -= t_changev + ti; // we don't need to come back now
            std::cout << hhID << "/" << nextV << "(+" << t_changev << "+" << ti << ")->";
            visitedHh.push_back(hhID);
        }
        else
        {
            // finish current route
            t_work -= t_home;
            visitedV.push_back(home); // return back at home

            // save route in _interviewer k
            tmpRoute.villages = visitedV;
            tmpRoute.households = visitedHh;
            tmpRoute.time = tmax - t_work;

            _interviewer[k-1].routes.push_back(tmpRoute);


            // start new route
            k = std::min(k+1,K);

            visitedV.erase(visitedV.begin()+1, visitedV.end());
            visitedV.push_back(nextV);
            visitedHh.erase(visitedHh.begin(), visitedHh.end());
            visitedHh.push_back(hhID);

            std::cout << home << "(+" << t_home << ")    Spent time " << tmax - t_work << std::endl;
            std::cout << "Route " << k << " :" << home << "->"
                                               << nextV << "(+" << _distmatrix[home][nextV] << ")->";

            t_work= tmax - (_distmatrix[home][nextV] + ti);


       }

        predV = nextV;


    }
    std::cout << home << "   Spent time " << tmax-t_work << std::endl;
    std::cout << std::endl;
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


    // plan nex day
    for (unsigned int d=0; d<1; d++) // planning horizon 80 weekdays
        // day, unvisited hh, _interviewer, _households, distmatrix
        planForADay(d, ToVis, _interviewer, _households, _distmatrix);


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
