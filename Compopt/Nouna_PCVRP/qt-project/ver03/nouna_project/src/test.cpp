#include "hrd/test.h"

clTest::clTest(std::vector<stInterviewer> _interviewer,
            std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan_weekly):
            interviewer(_interviewer),
            ITimePlan_weekly(_ITimePlan_weekly)
{ }

/*
 * test, how much interviewer have extra working hours in a week
 */
int clTest::test1()
{
    std::cout << "\n--------------------------Test1-----------------------------------------------\n";

    unsigned int count = 0;

    for (unsigned int k=0; k< interviewer.size(); ++k)
    {
        unsigned int week = 0;
        while(week<48)
        {
            double week_wtime = 0.;
            for (unsigned int d=week*5; d<(week+1)*5; ++d)
                week_wtime += interviewer[k].routes_days[d].time;

            if (week_wtime > 5*8*60)
            {
                ++count;
                std::cout << "interviewer " << k + 1 << " has extra hours on the week " << week + 1
                          << ": " << 5*8*60 - week_wtime << " min"<< std::endl;
            }
            ++week;
        }
    }
    std::cout << "Summary: " << count << " interviewers have extra hours in some weeks" << std::endl;
    std::cout << "------------------------------------------------------------------------------\n";


    return 0;
}

/*
 * test, how much interviewer have extra working hours in a day
 */

int clTest::test2()
{
    std::cout << "\n--------------------------Test2-----------------------------------------------\n";

    unsigned int count = 0;

    for (unsigned int k=0; k< interviewer.size(); ++k)
    {
        for (unsigned int d=0; d<interviewer[k].routes_days.size(); ++d )
            if ( interviewer[k].routes_days[d].time - 8*60. > 0)
            {
                ++count;
                std::cout << "interviewer " << k + 1 << " has extra hours on the day " << d % 5 + 1
                          << "(week " << d/5 + 1 << "): "
                          << 8*60 - interviewer[k].routes_days[d].time << " min"<< std::endl;

            }
    }
    std::cout << "Summary: " << count << " interviewers have extra hours in some days" << std::endl;

    std::cout << "------------------------------------------------------------------------------\n";

    return 0;
}



/*
 * Run set of test on a found solution
 */
void clTest::run()
{
    std::cout << "\n----------------------------------------------------------------------------\n";
    std::cout << "\n----------------------- Run tests ------------------------------------------\n";

    test1(); // small test, how much interviewer have extra working hours in a week

    test2(); // small test, how much interviewer have extra working hours in a day

    std::cout << "\n----------------------------------------------------------------------------\n";
}

