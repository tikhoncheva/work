#include "hrd/test.h"

clTest::clTest(const std::vector<stInterviewer>& _interviewer,
               const std::vector<stHousehold>& _households,
               const std::vector<std::vector<std::pair<unsigned int, double> > >& _ITimePlan_weekly):
            interviewer(_interviewer),
            household(_households),
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
 * test if long interview times are split correctly in several days: the sum over days should be equal
 * entire interview time
 */
int clTest::test3()
{
    std::cout << "\n--------------------------Test3-----------------------------------------------\n";

    int itime;
    unsigned int count = 0;
    std::vector<int> interview_time(household.size(), 0);

    std::vector<std::vector<std::pair<unsigned int, double> > > copyITimePlan; // copy of _ITimePlan

    copyITimePlan.resize(ITimePlan_weekly.size());
    for (unsigned int w=0; w<ITimePlan_weekly.size(); ++w)
        for (unsigned int p=0; p<ITimePlan_weekly[w].size(); ++p )
        {
            copyITimePlan[w].push_back(std::make_pair(ITimePlan_weekly[w][p].first, ITimePlan_weekly[w][p].second));
        }

    // interview times in the solution
    for (unsigned int i=0; i<interviewer.size(); ++i)
        for (unsigned int d=0; d< interviewer[i].routes_days.size(); ++d)
            for (unsigned int h=0; h<interviewer[i].routes_days[d].households.size(); ++h)
            {
                const unsigned hhID = interviewer[i].routes_days[d].households[h];

                // find planed interview time for current hh on the day d
                std::vector<std::pair<unsigned int, double> >::iterator timeplan_it;

                timeplan_it = std::find_if(copyITimePlan[d/5].begin(),copyITimePlan[d/5].end(),
                                           [hhID] (std::pair<unsigned int, double> weekday)
                                                {return weekday.first == hhID;});
                itime = round( (*timeplan_it).second);
                (*timeplan_it).first = 99999;

                interview_time[hhID] += itime;
            }

    for (unsigned int h=0; h< household.size(); ++h)
    {
        if (household[h].type == 1 && household[h].itime != (interview_time[h]-2*constant::shortITime) )
        {
            ++count;
            std::cout << "Error: hh " << h+10001 << " expected "
                      << household[h].itime << "+2x" << constant::shortITime
                      << " sec interview time, but was interviewed "
                      << interview_time[h] << " sec" << std::endl;
        }

        if (household[h].type == 0 && household[h].itime*3 != interview_time[h])
        {
            ++count;
            std::cout << "Error: hh " << h+10001 << " expected 3x"
                      << household[h].itime << " sec interview time, but was interviewed "
                      << interview_time[h] << " sec" << std::endl;
        }

    }

    std::cout << "Summary: " << count << " interviewers have wrong interview time" << std::endl;

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

    test3(); // small test, check if all long interviews are split correctly

    std::cout << "\n----------------------------------------------------------------------------\n";
}

