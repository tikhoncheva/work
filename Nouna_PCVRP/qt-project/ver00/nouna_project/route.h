#ifndef ROUTE_H
#define ROUTE_H

/*
 * Route
 */
struct stRoute
{
    unsigned int ID;
    std::string name;
    unsigned int start;		// start village
    unsigned int end;		// end village
    double dist;
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


#endif // ROUTE_H
