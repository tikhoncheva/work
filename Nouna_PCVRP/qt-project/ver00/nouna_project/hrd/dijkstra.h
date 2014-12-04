#include <iostream>
#include <vector>
#include <set>
#include <utility> // pair
#include <algorithm>
#include <iterator>
#include <limits> // infinity

#include "road.h"
#include "village.h"

bool isZero(unsigned int a)
{
    return a == 0;
}

std::vector<double> dijkstraAlg(std::vector<std::vector<unsigned int> > adjMatrix,
                                std::vector<stRoad> R)
{
    unsigned int n = adjMatrix.size();
    const unsigned int source = 142-100-1; // Capital (Nouna)
    const double infi = std::numeric_limits<double>::infinity();


    std::vector<double> dist(n, infi);
    std::vector<unsigned int> prev(n, -1);
    std::set<std::pair<unsigned int, double > > Q; // vertex queue

    dist[source] = 0.;
    prev[source] = source;

    Q.insert(std::make_pair(source, dist[source]));

    while (!Q.empty())
    {
        unsigned int u = Q.begin()->first;
        double d_u = Q.begin()->second;

        Q.erase(Q.begin());

        std::vector<unsigned int> neighbors = adjMatrix[u];
        // delete all zeros
        neighbors.resize(std::remove_if(neighbors.begin(), neighbors.end(), isZero) - neighbors.begin());

        // for each neighbor v of u
        for (std::vector<unsigned int>::const_iterator neighbor_iter = neighbors.begin();
             neighbor_iter != neighbors.end();
             neighbor_iter++)
        {
            unsigned int edgeID = *neighbor_iter;

            unsigned int v = R[edgeID-1001].end - 101;
            if (u==v) // if u is end of the road
                v = R[edgeID-1001].start-101;
            double d_uv = R[edgeID-1001].dist;
            if (d_u+d_uv < dist[v])
            {
                Q.erase(std::make_pair(v, dist[v]));

                dist[v] = d_u+d_uv;
                prev[v] = u;
                Q.insert(std::make_pair(v, dist[v]));
            }
        } // end for each neighbor v
    } // Q is empty
    return dist;
}
