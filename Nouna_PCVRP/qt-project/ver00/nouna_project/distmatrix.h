#ifndef DISTMATRIX_H
#define DISTMATRIX_H

#include <iostream>
#include <vector>
#include <algorithm>    // std::find_if
#include "village.h"
#include "route.h"


struct stRoute_short
{
    unsigned int ID;
    double dist;
    unsigned int category;	// 1, 2, 3, 4
    unsigned int rain;
};


//extern std::vector<std::vector<stRoute_short> > distmatrix;
extern std::vector<std::vector<double> > distmatrix;


std::vector<std::vector<double> > compute_distmatrix(std::vector<stVillage>,
//std::vector<std::vector<stRoute_short> > compute_distmatrix(std::vector<stVillage>,
                                                            std::vector<stRoute>);

#endif // DISTMATRIX_H

