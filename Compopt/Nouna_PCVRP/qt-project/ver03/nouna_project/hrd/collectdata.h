#ifndef COLLECTDATA_H
#define COLLECTDATA_H

#include <hrd/datadef.h>
#include <hrd/distmatrix.h> // adjazent matrix with distances
#include <hrd/dijkstra.h>  // shortes ways between villages
#include <hrd/const.h>

#include <utility>      // std::pair

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

    unsigned int vID;   // village's ID
    unsigned int hID;   // household's ID

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
            hID = int(household_in_vID-households.begin());

            _village_household[i].push_back(hID);
            // find next household in the village i
            household_in_vID = std::find_if(household_in_vID + 1,
                                            households.end(),
                                            matchVillageID(vID));
        }

        std::sort(_village_household[i].begin(), _village_household[i].end());

    }
    return _village_household;
}

/*
 * Function that calculates distance and time matrices
 * and collects statistic about interview time
 */

void collectdata_routine(std::vector<stVillage> _villages,
                         std::vector<stRoad> _roads,
                         std::vector<stHousehold> _households)
{
    std::cout << "\nData routine:" << std::endl;
    unsigned int V = _villages.size();

    /*
     *  set number of available interviewers
     */
    nK = constant::nInterviewers;
    std::cout << "  set number of available interviewers to " << nK << " ...";
    Interviewer.resize(nK);
    std::cout << "finished" << std::endl;

    /*
     *  compute adjazent matrix with distances
     */
    std::cout << "  compute adjazent matrix with distances ...";
    distmatrix = compute_distmatrix(_villages, _roads);
    std::cout << "finished" << std::endl;

    /*
     *  shortes ways between villages
     */
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

    constant::maxDistDry = 0.;
    constant::maxDistRain = 0.;
    double maxDry = 0., maxRain = 0.;
    for (unsigned int i=0; i< V; ++i)
    {
        timematrixDry[i]  = dijkstraAlg(i, distmatrix, 0, predecessorsDry[i]);// rainingseason = 0
        timematrixRain[i] = dijkstraAlg(i, distmatrix, 1, predecessorsRain[i]);// rainingseason = 1

        maxDry = *std::max_element(timematrixDry[i].begin(), timematrixDry[i].end());
        maxRain = *std::max_element(timematrixRain[i].begin(), timematrixRain[i].end());

        if (maxDry > constant::maxDistDry)
            constant::maxDistDry = maxDry;
        if (maxRain > constant::maxDistRain)
            constant::maxDistRain = maxRain;
    }
    std::cout << "finished. Maximal travel time " << constant::maxDistDry << " ("
                                                  << constant::maxDistRain << " raining season)" << std::endl;

//    std::cout << "Distances from the Nouna in not raining season" << std::endl;
//    for (unsigned int i=0; i<timematrixDry[41].size(); ++i)
//        std::cout << timematrixDry[41][i] << " ";
//    std::cout << std::endl;

//    std::cout << "Distances from the Nouna in raining season" << std::endl;
//    for (unsigned int i=0; i<timematrixRain[41].size(); ++i)
//        std::cout << timematrixRain[41][i] << " ";
//    std::cout << std::endl;

    /*
     *                  get additional list of hh in villages
     */
    std::cout << "  get additional list of hh in villages ...";
    village_households = households_in_villages(_villages, _households);
    std::cout << "finished" << std::endl;


    /*
     * Time Statistic
     */
    std::cout << "  time statistic..." << std::endl;

    double sShortITime = 0.;
    double sLongITime1 = 0.;
    double sLongITime23 = 0.;

    int nLongHh = 0;
    int nShortHh = 0;

    // small interviews must be planned in each period
    for (unsigned int i=0; i<_households.size(); ++i)
        if (_households[i].type==0)
        {
            sShortITime += _households[i].itime;
            ++nShortHh;
        }
        else {
            sLongITime1 += _households[i].itime;
            sLongITime23 += 10;//.*_households[i].nPersons;
            ++nLongHh;
        }

    TimeInfo.workTimeYear = nK * 240 /*days*/
                               * 8  /*hours*/
                               * 60 /*min*/;
    TimeInfo.sShortITime = sShortITime;
    TimeInfo.sLongITime1 = sLongITime1;
    TimeInfo.sLongITime23 = sLongITime23;
    TimeInfo.nLongInterviews = nLongHh;
    TimeInfo.nShortInterviews = nShortHh;

    std::cout << "      " << nShortHh
              << " short interviews with summary itime " << sShortITime << std::endl;
    std::cout << "      " << nLongHh
              << " long interviews with summary itime " << sLongITime1
              << " or " << sLongITime23 << std::endl;
    std::cout << std::endl;

    std::cout << "      Summary (year) with "  << nK << " interviewers: "<< std::endl;
    std::cout << "      Working time " << TimeInfo.workTimeYear << std::endl;
    std::cout << "      Short interviews " << 3 * sShortITime << std::endl;
    std::cout << "      Long interviews " <<      sLongITime1 + 2 * sLongITime23 << std::endl;

    std::cout << "      Summary " << 3 * sShortITime + sLongITime1 + 2 * sLongITime23 << std::endl;
    std::cout << "      -----------------------------------------" << std::endl;
    std::cout << "      Remains for travel time " << TimeInfo.workTimeYear - 3 * sShortITime
                                               - sLongITime1 - 2 * sLongITime23 << std::endl;


    std::cout << std::endl << "finished" << std::endl << std::endl;
}

#endif // COLLECTDATA_H
