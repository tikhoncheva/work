#ifndef DIJKSTRA2_H
#define DIJKSTRA2_H

#include <iostream>
#include <vector>
#include <set>
#include <utility> // pair
#include <algorithm>
#include <iterator>
#include <limits> // infinity
#include <math.h>

#include "road.h"
#include "village.h"

/*
 * Implementation of the Dijkstra algorithm to compute distances between one village and
 * all other villages
  */

std::vector<double> dijkstraAlg(const unsigned int source,
                                std::vector<std::vector<uDist> > dist,
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

        std::vector<uDist> neighbors = dist[u];

        // for each neighbor v of u
        for (std::vector<uDist>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            uDist road = *neighbor_iter;
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

#endif // DIJKSTRA2_H
