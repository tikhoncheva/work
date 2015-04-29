#ifndef TEST_H
#define TEST_H

#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#include <hrd/const.h>
#include <hrd/interviewer.h>
#include <hrd/household.h>

class clTest
{
private:
    const std::vector<stInterviewer> interviewer;
    const std::vector<stHousehold> household;
    const std::vector<std::vector<std::pair<unsigned int, double> > > ITimePlan_weekly;
//    const std::vector<std::vector<std::pair<unsigned int, double> > > ITimePlan_daily;


    int test1(std::stringstream& ss);
    int test2(std::stringstream& ss);
    int test3(std::stringstream& ss);
    int test4(std::stringstream& ss);

public:
    clTest(const std::vector<stInterviewer>&, const std::vector<stHousehold>& ,
           const std::vector<std::vector<std::pair<unsigned int, double> > >& );

    std::string run();

};

#endif // TEST_H
