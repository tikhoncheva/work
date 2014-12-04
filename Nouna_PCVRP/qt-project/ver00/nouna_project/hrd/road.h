#ifndef ROAD_H
#define ROAD_H

/*
 * Route
 */
struct stRoad
{
    unsigned int ID;
    std::string name;
    unsigned int start;		// start village
    unsigned int end;		// end village
    float dist;
    unsigned int category;	// 1, 2, 3, 4
    unsigned int rain;

    void print()
    {
        std::cout << "ID=" << ID << " name=" << name << "  from " << start
                << "  to " << end << "  dist=" << dist
                << "  category=" << category << "  rain=" << rain
                << std::endl;
    }

};


#endif // ROAD_H
