#ifndef COLLECTDATA_H
#define COLLECTDATA_H

#include <hrd/datadef.h>
#include <hrd/distmatrix.h> // adjazent matrix with distances
#include <hrd/dijkstra2.h>  // shortes ways between villages

/*
 * ------------------------------------------------------------------------------------
 */


class matchVillageID
{
    unsigned int ID;
public:
    matchVillageID(const unsigned int &_ID) : ID(_ID) {}

    bool operator()(const stHousehold &household) const
    {
        return household.villageID == ID;
    }
};


std::vector<std::vector<unsigned int> > households_in_villages(std::vector<stVillage> villages,
                                                               std::vector<stHousehold> households)
{
    // get list of households in each village
    unsigned int V = villages.size();
    std::vector<std::vector<unsigned int> > _village_household(V);

    unsigned int vID;
    std::vector<stHousehold>::iterator household_in_vID;

    for (unsigned int i=0; i<V; ++i)    // for each village
    {
        vID = i + 101;
        // find household in this village i ;
        household_in_vID = std::find_if(households.begin(),
                                        households.end(),
                                        matchVillageID(vID));
        while (household_in_vID!=households.end())
        {
            // add household index in the list
            _village_household[i].push_back(int(household_in_vID-households.begin()));
            // find next household in the village i
            household_in_vID = std::find_if(household_in_vID + 1,
                                            households.end(),
                                            matchVillageID(vID));
        }

    }

    return _village_household;
}

/*
 * Function that calculates distance and time matrices
 */

void collectdata_routine(std::vector<stVillage> _villages,
                         std::vector<stRoad> _roads,
                         std::vector<stHousehold> _households)
{
    std::cout << "Data routine:" << std::endl;
    unsigned int V = _villages.size();

    // set number of available interviewers
    std::cout << "  set number of available interviewers ...";
    Interviewer.resize(20);
    std::cout << "finished" << std::endl;

    // compute adjazent matrix with distances
    std::cout << "  compute adjazent matrix with distances ...";
    distmatrix = compute_distmatrix(_villages, _roads);
    std::cout << "finished" << std::endl;

    // shortes ways between villages
    std::cout << "  compute shortes ways between villages ...";
    /*
     * // using dijkstra.h
    timematrixDry = dijkstraAlg(distmatrix, Road,0);// rainingseason = 0
    timematrixRain = dijkstraAlg(distmatrix, Road,1);// rainingseason = 1
    */

    // using dijkstra2.h
    timematrixDry.resize(V);
    timematrixRain.resize(V);

    predecessorsDry.resize(V);    // shortest ways
    predecessorsRain.resize(V);   // between all villages

    for (unsigned int i=0; i< V; ++i)
    {
        timematrixDry[i]  = dijkstraAlg(i, distmatrix, 0, predecessorsDry[i]);// rainingseason = 0
        timematrixRain[i] = dijkstraAlg(i, distmatrix, 1, predecessorsRain[i]);// rainingseason = 1
    }
    std::cout << "finished" << std::endl;

//    std::cout << "Distances from the Nouna in not raining season" << std::endl;
//    for (unsigned int i=0; i<timematrixDry[41].size(); ++i)
//        std::cout << timematrixDry[41][i] << " ";
//    std::cout << std::endl;

//    std::cout << "Distances from the Nouna in raining season" << std::endl;
//    for (unsigned int i=0; i<timematrixRain[41].size(); ++i)
//        std::cout << timematrixRain[41][i] << " ";
//    std::cout << std::endl;

    // get additional list of hh in villages
    std::cout << "  get additional list of hh in villages ...";
    village_household = households_in_villages(_villages, _households);
    std::cout << "finished" << std::endl;

    std::cout << "finished" << std::endl;
}

#endif // COLLECTDATA_H
