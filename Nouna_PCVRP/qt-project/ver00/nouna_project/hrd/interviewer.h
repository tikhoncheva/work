#ifndef INTERVIEWER_H
#define INTERVIEWER_H

#include <sstream>
#include <string>

struct stRoute
{
    std::vector<unsigned int> households;
    std::vector<unsigned int> villages;
    unsigned int time;
};

/*
 * An interviewer
 */
struct stInterviewer
{
    unsigned int ID;
    unsigned int nRoutes;
    std::vector<stRoute> routes;

    std::string visVilToString(unsigned int day)
    {
        std::stringstream ss;
        for (unsigned int i=0; i< routes[day].villages.size(); i++)
            ss << routes[day].villages[i] << " ";
        return ss.str();
    }

    std::string visHhToString(unsigned int day)
    {
        std::stringstream ss;
        for (unsigned int i=0; i< routes[day].households.size(); i++)
            ss << routes[day].households[i] << " ";
        return ss.str();
    }

    void print()
    {
        std::cout << "Interviewer: ID=" << ID << "  nRoutes"
                << nRoutes << std::endl;
    }
};
#endif // INTERVIEWER_H
