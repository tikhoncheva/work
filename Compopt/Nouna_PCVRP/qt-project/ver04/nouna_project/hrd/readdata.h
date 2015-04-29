/*
 * Three functions for reading input data (list of villages,
 * list of households, list of roads) for the problem from corresponding csv-files.
 */
#ifndef READDATA_H
#define READDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

#include <assert.h>

#include "village.h"
#include "road.h"
#include "household.h"

// read villages.csv
std::vector<stVillage> readdata_villages(std::string fileName);
// read Roads.csv
std::vector<stRoad> readdata_Roads(std::string fileName);
// read households.csv
std::vector<stHousehold> readdata_households(std::string fileName);


#endif // READDATA_H
