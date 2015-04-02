#include "../hrd/distmatrix.h"

class matchVillageID
{
    unsigned int ID;
public:
    matchVillageID(const unsigned int &_ID) : ID(_ID) {}

    bool operator()(const stVillage &village) const
    {
        return village.ID == ID;
    }
};

std::vector<std::vector<uDist> > compute_distmatrix(std::vector<stVillage> Village,
                                                    std::vector<stRoad> Road)
{
    std::vector<std::vector<uDist> > dmatrix;

    std::vector<stVillage>::iterator it_start;
    std::vector<stVillage>::iterator it_end;

    unsigned int i;
    unsigned int j;

    unsigned int nV = Village.size();
    unsigned int nR = Road.size();

    unsigned int startID;
    unsigned int endID;

    dmatrix.resize(nV);
    for (unsigned int i=0; i<nV; i++)
        dmatrix[i] = std::vector<uDist>(nV,uDist(0, 0, 0, 0, 0));

    for (unsigned int r=0; r<nR; ++r)
    {

        startID  = Road[r].start;
        endID  = Road[r].end;

        it_start = std::find_if(Village.begin(), Village.end(), matchVillageID(startID));
        it_end = std::find_if(Village.begin(), Village.end(), matchVillageID(endID));

        i = it_start - Village.begin();
        j = it_end - Village.begin();

        dmatrix[i][j] = uDist(Road[r].ID, Road[r].dist, speeddry[Road[r].category-1],
                                                        speedrain[Road[r].category-1],
                                                        Road[r].rainDepending);
        dmatrix[j][i] = dmatrix[i][j];
    }

    return dmatrix;
}

