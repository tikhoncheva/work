#ifndef INITIALSOLUTION_H
#define INITIALSOLUTION_H

#include <iostream>
#include <vector>

#include "hrd/village.h"
#include "hrd/interviewer.h"
#include "hrd/road.h"
#include "hrd/household.h"



void setNInterviwer();

std::vector<std::vector<unsigned int> > initialsolution(std::vector<stVillage>,                 // villages
                                                       std::vector<stHousehold>,               // households
                                                       std::vector<stRoad>,                    // roads
                                                       std::vector<stInterviewer>,             // Interviewer
                                                       std::vector<double>                     // distmatrix
                                                       );

#endif // INITIALSOLUTION_H
