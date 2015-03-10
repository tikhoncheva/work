#ifndef DISTMATRIX_H
#define DISTMATRIX_H

#include <iostream>
#include <vector>
#include <algorithm>    // std::find_if
#include "village.h"
#include "road.h"

static const int speeddry[] = {50,40,30,20};
static const int speedrain[] = {50,35,25,15};

//std::vector<std::vector<uDist> > distmatrix;
std::vector<std::vector<uDist> > compute_distmatrix(std::vector<stVillage> ,
                                                    std::vector<stRoad>);

#endif // DISTMATRIX_H1

