#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>

#include <hrd/interviewer.h>

class clTest
{
private:
    std::vector<stInterviewer>& interviewer;
    std::vector<std::vector<std::pair<unsigned int, double> > > ITimePlan_weekly;
    std::vector<std::vector<std::pair<unsigned int, double> > > ITimePlan_daily;


    int test1();
    int test2();

public:
    clTest(std::vector<stInterviewer>, std::vector<std::vector<std::pair<unsigned int, double> > >);

    void run();

};

#endif // TEST_H
