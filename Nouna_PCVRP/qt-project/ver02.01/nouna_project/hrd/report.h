#ifndef REPORT_H
#define REPORT_H

#include <algorithm>
#include <vector>
#include <set>
#include <utility>      // std::pair

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <hrd/interviewer.h>
#include <hrd/household.h>

// save itime schedule
void saveHH_ITPlan (const std::vector<std::vector<std::pair<unsigned int, double> > >,
                    const std::string);
// save initial scheduling of households
void saveHHSchedule(const std::vector<stInterviewer>,
                    std::vector<std::vector<std::pair<unsigned int, double> > >  ,
                    const std::string);

#endif // REPORT_H
