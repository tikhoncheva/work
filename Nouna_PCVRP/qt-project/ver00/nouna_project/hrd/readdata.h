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
#include "route.h"
#include "household.h"

// read villages.csv
std::vector<stVillage> readdata_villages(std::string fileName);
// read routes.csv
std::vector<stRoute> readdata_routes(std::string fileName);
// read households.csv
std::vector<stHousehold> readdata_households(std::string fileName);


#endif // READDATA_H
