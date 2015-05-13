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


struct rowEntries
{
    unsigned int hhID;

    std::string itime;
    std::string day;
    std::string Interviewer;

    bool operator<(const rowEntries& a) const
    {  return hhID < a.hhID; }

    bool operator==(const rowEntries& a) const
    {
        return hhID == a.hhID;
    }
};


std::set<rowEntries> HHSchedule_weekview_forTable(const std::vector<stInterviewer>,
                                      std::vector<std::vector<std::pair<unsigned int, double> > >);

void  saveHHSchedule_weekview(const std::set<rowEntries> HHSchedule,
                              const std::string fileName);
#endif // REPORT_H
