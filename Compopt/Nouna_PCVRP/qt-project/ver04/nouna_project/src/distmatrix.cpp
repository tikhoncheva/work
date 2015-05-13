#include "hrd/distmatrix.h"

// ------------------------------------------------------------------------------------
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
// ------------------------------------------------------------------------------------


/* ------------------------------------------------------------------------
 * compute weighted adjacency matrix of the road graph
 * ------------------------------------------------------------------------
 */
std::vector<std::vector<adjmatrix_entry> > w_adj_matrix(std::vector<stVillage> Village,
                                              std::vector<stRoad> Road)
{
    std::vector<std::vector<adjmatrix_entry> > dmatrix;

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
        dmatrix[i] = std::vector<adjmatrix_entry>(nV,adjmatrix_entry(0, 0, 0, 0, 0));

    for (unsigned int r=0; r<nR; ++r)
    {

        startID  = Road[r].start;
        endID  = Road[r].end;

        it_start = std::find_if(Village.begin(), Village.end(), matchVillageID(startID));
        it_end = std::find_if(Village.begin(), Village.end(), matchVillageID(endID));

        i = it_start - Village.begin();
        j = it_end - Village.begin();

        dmatrix[i][j] = adjmatrix_entry(Road[r].ID, Road[r].dist, speeddry[Road[r].category-1],
                speedrain[Road[r].category-1],
                Road[r].rainDepending);
        dmatrix[j][i] = dmatrix[i][j];
    }

    return dmatrix;
}


/* ------------------------------------------------------------------------
 * Implementation of the Dijkstra algorithm to compute distances between one village and
 * all other villages
 * ------------------------------------------------------------------------
 */

std::vector<double> dijkstraAlg(const unsigned int source,
                                std::vector<std::vector<adjmatrix_entry> > dist,
                                bool rainingseason,
                                std::vector<unsigned int>& prev)
{
    unsigned int n = dist.size();
    const double infi = std::numeric_limits<double>::infinity();


    std::vector<double> timedist(n, infi);
    std::set<std::pair<unsigned int, double > > Q; // vertex queue

    prev = std::vector<unsigned int> (n, -1);

    timedist[source] = 0.;
    prev[source] = source;

    Q.insert(std::make_pair(source, timedist[source]));

    while (!Q.empty())
    {
        unsigned int u = Q.begin()->first;
        double d_u = Q.begin()->second;

        Q.erase(Q.begin());

        std::vector<adjmatrix_entry> neighbors = dist[u];

        // for each neighbor v of u
        for (std::vector<adjmatrix_entry>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            adjmatrix_entry road = *neighbor_iter;
            if (road.dist!=0)
            {
                unsigned int v = neighbor_iter - neighbors.begin();

                // d_uv in min
                double d_uv = round(
                            60* road.dist/road.speeddry*(1-rainingseason*road.rainDepending)
                            + 60* road.dist/road.speedrain*rainingseason*road.rainDepending
                            );

                if (d_u+d_uv < timedist[v])
                {
                    Q.erase(std::make_pair(v, timedist[v]));

                    timedist[v] = d_u+d_uv;
                    prev[v] = u;
                    Q.insert(std::make_pair(v, timedist[v]));
                }
            }
        } // end for each neighbor v
    } // Q is empty

    return timedist;
}


