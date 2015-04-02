#ifndef STVILLAGE_H
#define STVILLAGE_H

/*
 * A Village
 */
struct stVillage
{
    unsigned int ID;
    std::string name;
    std::pair<double, double> coord; // longitude and latitude

    void print()
    {
        std::cout << "ID=" << ID << " name=" << name << "  coord=("
                << coord.first << "," << coord.second << ")" << std::endl;
    }
};
#endif // STVILLAGE_H
