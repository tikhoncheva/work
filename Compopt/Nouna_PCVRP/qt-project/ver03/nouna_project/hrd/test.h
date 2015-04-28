#ifndef TEST_H
#define TEST_H

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


    int test1();
    int test2();
    int test3();

public:
    clTest(const std::vector<stInterviewer>&, const std::vector<stHousehold>& ,
           const std::vector<std::vector<std::pair<unsigned int, double> > >& );

    void run();

};

#endif // TEST_H
