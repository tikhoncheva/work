
#include "hrd/initialsolution.h"

/*
 * Initial Solution
 */
std::vector<std::vector<unsigned int> > initialsolution(std::vector<stVillage> V,                 // villages
                                                       std::vector<stHousehold> Hh,               // households
                                                       std::vector<stRoad> R,                    // roads
                                                       std::vector<stInterviewer> I,             // Interviewer
                                                       std::vector<double> distmatrix// distmatrix
                                                       )
{
    unsigned int K = I.size();
    unsigned int nHh = Hh.size();

    std::vector<std::vector<unsigned int> > tours;
    tours.resize(K);


    // sort Hh
    return tours;
}
