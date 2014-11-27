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

std::vector<std::vector<unsigned int> > compute_adjmatrix(std::vector<stVillage> Village,
//std::vector<std::vector<stRoad_short> > compute_distmatrix(std::vector<stVillage> Village,
                                                            std::vector<stRoad> Road)
{
//    std::vector<std::vector<stRoad_short> > distmatrix;
    std::vector<std::vector<unsigned int> > adjM;
    //stRoad_short tmpRoadInfo;

    std::vector<stVillage>::iterator it_start;
    std::vector<stVillage>::iterator it_end;

    unsigned int i;
    unsigned int j;

    unsigned int nV = Village.size();
    unsigned int nR = Road.size();

    unsigned int startID;
    unsigned int endID;

    adjM.resize(nV);
    for (unsigned int i=0; i<nV; i++)
        adjM[i] = std::vector<unsigned int>(nV,0.);

    for (unsigned int r=0; r<nR; ++r)
    {

        startID  = Road[r].start;
        endID  = Road[r].end;

        it_start = std::find_if(Village.begin(), Village.end(), matchVillageID(startID));
        it_end = std::find_if(Village.begin(), Village.end(), matchVillageID(endID));

        i = it_start - Village.begin();
        j = it_end - Village.begin();
/*
        tmpRoadInfo.category = Road[r].category;
        tmpRoadInfo.dist = Road[r].dist;
        tmpRoadInfo.rain = Road[r].rain;
        tmpRoadInfo.ID = Road[r].ID;
*/
        adjM[i][j] = Road[r].ID;
        adjM[j][i] = Road[r].ID;
    }

    return adjM;
}
