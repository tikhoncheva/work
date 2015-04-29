#ifndef HOUSEHOLD_H
#define HOUSEHOLD_H

/*
 * A household
 *
 * declaration of a structure stVillage that describes one household
 */

struct stHousehold
{
    unsigned int ID;
    std::string name;
    unsigned int type; // 1 or 0
    unsigned int nPersons;
    unsigned int villageID;
    double itime;               // interview time

    void print()
    {
        std::cout << "ID=" << ID << " name=" << name << "  type=" << type
                << "  nPersons=" << nPersons << "  villageID=" << villageID
                << std::endl;
    }
};

#endif // HOUSEHOLD_H
