#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <iostream>
#include <vector>
#include <set>
#include <utility> // pair
#include <algorithm>
#include <iterator>
#include <limits> // infinity

#include "road.h"
#include "village.h"

bool isZero(const uDist& a)
{
    return a.dist == 0;
}

std::vector<double> dijkstraAlg(std::vector<std::vector<uDist> > dist,
                                std::vector<stRoad> R, bool rainingseason)
{
    unsigned int n = dist.size();
    const unsigned int source = 142-100-1; // Capital (Nouna)
    const double infi = std::numeric_limits<double>::infinity();


    std::vector<double> timedist(n, infi);
    std::vector<unsigned int> prev(n, -1);
    std::set<std::pair<unsigned int, double > > Q; // vertex queue

    timedist[source] = 0.;
    prev[source] = source;

    Q.insert(std::make_pair(source, timedist[source]));

    while (!Q.empty())
    {
        unsigned int u = Q.begin()->first;
        double d_u = Q.begin()->second;

        Q.erase(Q.begin());

        std::vector<uDist> neighbors = dist[u];
        // delete all zeros
        neighbors.resize(std::remove_if(neighbors.begin(), neighbors.end(), isZero) - neighbors.begin());

        // for each neighbor v of u
        for (std::vector<uDist>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            uDist road = *neighbor_iter;

            unsigned int v = R[road.roadID-1001].end - 101;
            if (u==v) // if u is end of the road
                v = R[road.roadID-1001].start-101;

            // d_uv in min
            double d_uv = R[road.roadID-1001].dist/road.speeddry*(1-rainingseason*road.rainDepending)*60
                        + R[road.roadID-1001].dist/road.speedrain*rainingseason*road.rainDepending*60;

            if (d_u+d_uv < timedist[v])
            {
                Q.erase(std::make_pair(v, timedist[v]));

                timedist[v] = d_u+d_uv;
                prev[v] = u;
                Q.insert(std::make_pair(v, timedist[v]));
            }
        } // end for each neighbor v
    } // Q is empty
    return timedist;
}

#endif // DIJKSTRA_H
