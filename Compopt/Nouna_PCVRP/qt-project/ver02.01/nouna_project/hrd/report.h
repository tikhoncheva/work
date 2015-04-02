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

struct writeFormat1
{
    unsigned int hhID;
    std::string itime;
    std::string day;

    writeFormat1(unsigned int _hhID, std::string _itime = "", std::string _day = ""):
        hhID(_hhID), itime(_itime), day(_day)
    {  }

    bool operator<(const writeFormat1& a) const
    {  return hhID < a.hhID; }

    bool operator==(const writeFormat1& a) const
    {
        return hhID == a.hhID;
    }
};

//
struct writeFormat2
{
    unsigned int hhID;
//    std::string period1;
//    std::string period2;
//    std::string period3;

    std::string itime;
    std::string day;
    std::string Interviewer;

    bool operator<(const writeFormat2& a) const
    {  return hhID < a.hhID; }

    bool operator==(const writeFormat2& a) const
    {
        return hhID == a.hhID;
    }
};

// save itime schedule
void saveHH_ITPlan (const std::vector<std::vector<std::pair<unsigned int, double> > >,
                    const std::string);
// save initial scheduling of households
std::set<writeFormat2> saveHHSchedule(const std::vector<stInterviewer>,
                                      std::vector<std::vector<std::pair<unsigned int, double> > >);
                                      //const std::string);

#endif // REPORT_H
