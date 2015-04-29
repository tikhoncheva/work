#ifndef INTERVIEWER_H
#define INTERVIEWER_H

#include <sstream>
#include <string>
#include <iostream>

// additional structure for routes
struct stRoute
{
    std::vector<unsigned int> households;       // households on the route
    std::vector<unsigned int> villages;         // villages the households belong to
    double time;                                // work time
};

/*
 * An interviewer
 *
 * declaration of a structure stVInterviewer that describes one interview:
 */
struct stInterviewer
{
    unsigned int ID;
    unsigned int nRoutes;
    std::vector<stRoute> routes_weeks;
    std::vector<stRoute> routes_days;

    std::string visVilToString(unsigned int day) const
    {
        std::stringstream ss;
        for (unsigned int i=0; i< routes_days[day].villages.size(); i++)
            ss << routes_days[day].villages[i] + 101 << " ";
        return ss.str();
    }

    std::string visHhToString(unsigned int day) const
    {
        std::stringstream ss;
        for (unsigned int i=0; i< routes_days[day].households.size(); i++)
            ss << routes_days[day].households[i] + 10001 << " ";
        return ss.str();
    }

    void print()
    {
        std::cout << "Interviewer: ID=" << ID << "  nRoutes"
                << nRoutes << std::endl;
    }
};
#endif // INTERVIEWER_H
