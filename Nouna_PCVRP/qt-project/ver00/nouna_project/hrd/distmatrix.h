#ifndef DISTMATRIX_H
#define DISTMATRIX_H

#include <iostream>
#include <vector>
#include <algorithm>    // std::find_if
#include "village.h"
#include "road.h"


struct stRoad_short
{
    unsigned int ID;
    double dist;
    unsigned int category;	// 1, 2, 3, 4
    unsigned int rain;
};


//extern std::vector<std::vector<stRoad_short> > distmatrix;
extern std::vector<std::vector<unsigned int> > adjmatrix;

extern std::vector<double>  distmatrix;

std::vector<std::vector<unsigned int> > compute_adjmatrix(std::vector<stVillage>,
//std::vector<std::vector<stRoad_short> > compute_distmatrix(std::vector<stVillage>,
                                                            std::vector<stRoad>);

#endif // DISTMATRIX_H

