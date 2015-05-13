#ifndef DISTMATRIX_H
#define DISTMATRIX_H

#include <iostream>
#include <algorithm>    // std::find_if


#include <set>
#include <utility> // pair
#include <vector>

#include <iterator>
#include <limits> // infinity
#include <math.h>


#include "village.h"
#include "road.h"

static const int speeddry[] = {50,40,30,20};
static const int speedrain[] = {50,35,25,15};

// returns adjacency matrix of the road-network
// uDist contains information about road ID, lenght of the road, allowed speed in the dry/raining seasons
std::vector<std::vector<adjmatrix_entry> > w_adj_matrix(std::vector<stVillage> ,
                                                        std::vector<stRoad>);


// Implementation of the Dijkstra algorithm to compute distances between one village and
// all other villages
std::vector<double> dijkstraAlg(const unsigned int, // source
                                std::vector<std::vector<adjmatrix_entry> >, // adjmatrix
                                bool ,                            // raining season or not
                                std::vector<unsigned int>& prev); //
#endif // DISTMATRIX_H
