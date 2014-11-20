#ifndef DATADEF_H
#define DATADEF_H

#include <iostream>
#include <vector>
#include <utility>

#include "village.h"
#include "route.h"
#include "household.h"


/*
 * An interviewer
 */
struct Interviewer
{
    unsigned int ID;
};



unsigned int nH;		// number of households
unsigned int nV;     // number of villages
unsigned int nR;     // number of routes
unsigned int K;		// number of interviewer

std::vector<stVillage> Village;
std::vector<stRoute> Route;
std::vector<stHousehold> Household;

std::vector<std::vector<unsigned int> > distmatrix;


#endif // DATADEF_H
