#ifndef ROAD_H
#define ROAD_H

#include <vector>

/*
 * Road
 */
struct stRoad
{
    unsigned int ID;
    std::string name;
    unsigned int start;		// start village
    unsigned int end;		// end village
    float dist;
    unsigned int category;	// 1, 2, 3, 4
    unsigned int rainDepending;

    void print()
    {
        std::cout << "ID=" << ID << " name=" << name << "  from " << start
                << "  to " << end << "  dist=" << dist
                << "  category=" << category << "  rain=" << rainDepending
                << std::endl;
    }

};

// additional structure for distance matrix in km
struct uDist
{
    unsigned int roadID;
    double dist;
    unsigned int speeddry;
    unsigned int speedrain;
    bool rainDepending;

    uDist() : roadID(0), dist(0), speeddry(0), speedrain(0), rainDepending(0)
    {}

    uDist(unsigned int _roadID, double _dist,
          unsigned int _speeddry, unsigned int _speedrain,
          bool _rainDepending) : roadID(_roadID),
                                 dist(_dist),
                                 speeddry(_speeddry),
                                 speedrain(_speedrain),
                                 rainDepending(_rainDepending)
    {}


};


#endif // ROAD_H
