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

std::vector<std::vector<unsigned int> > compute_distmatrix(std::vector<stVillage> Village,
//std::vector<std::vector<stRoute_short> > compute_distmatrix(std::vector<stVillage> Village,
                                                            std::vector<stRoute> Route)
{
//    std::vector<std::vector<stRoute_short> > distmatrix;
    std::vector<std::vector<unsigned int> > distmatrix;
    //stRoute_short tmpRouteInfo;

    std::vector<stVillage>::iterator it_start;
    std::vector<stVillage>::iterator it_end;

    unsigned int i;
    unsigned int j;

    unsigned int nV = Village.size();
    unsigned int nR = Route.size();

    unsigned int startID;
    unsigned int endID;

    distmatrix.resize(nV);
    for (unsigned int i=0; i<nV; i++)
        distmatrix[i] = std::vector<unsigned int>(nV,0.);

    for (unsigned int r=0; r<nR; ++r)
    {

        startID  = Route[r].start;
        endID  = Route[r].end;

        it_start = std::find_if(Village.begin(), Village.end(), matchVillageID(startID));
        it_end = std::find_if(Village.begin(), Village.end(), matchVillageID(endID));

        i = it_start - Village.begin();
        j = it_end - Village.begin();
/*
        tmpRouteInfo.category = Route[r].category;
        tmpRouteInfo.dist = Route[r].dist;
        tmpRouteInfo.rain = Route[r].rain;
        tmpRouteInfo.ID = Route[r].ID;
*/
        distmatrix[i][j] = Route[r].ID;
    }




    return distmatrix;
}
