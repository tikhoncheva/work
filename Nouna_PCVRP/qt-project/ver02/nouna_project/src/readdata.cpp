#include"../hrd/readdata.h"

std::vector<stVillage> readdata_villages(std::string fileName)
{
    std::cout << "...read villages" << std::endl;

    std::vector<stVillage> result;	// result vector

    std::ifstream file(fileName.c_str());	// file to open
    std::string line;			// single line in the file
    std::stringstream buf;		// buffer to get infos from a single line

    stVillage tmpVillage;			// tmp Village instance
    double x, y;

    assert(file.is_open() && "ERROR readdata_villages : File cannot be opened");

    try
    {
        std::getline(file, line);// the fist line contains titles of the columns

        while (std::getline(file, line))
        {

            buf << line;

            buf >> tmpVillage.ID;
            buf >> tmpVillage.name;
            buf >> x;
            buf >> y;
            tmpVillage.coord = std::make_pair(x, y);

            if (tmpVillage.name=="Koussiri")
                tmpVillage.coord = std::make_pair(-3.81667, 12.7333);
            result.push_back(tmpVillage);

            buf.clear();
        }

        //nV = result.size();

    } // end try
    catch (...)
    {
        file.close();
        std::cerr << "ERROR readdata_villages : problem by reading from "
                  << fileName << std::endl;
    }

    file.close();

    return result;

}


std::vector<stRoad> readdata_Roads(std::string fileName)
{
    std::cout << "...read Roads" << std::endl;

    std::vector<stRoad> result;	// result vector

    std::ifstream file(fileName.c_str());	// file to open
    std::string line;			// single line in the file
    std::stringstream buf;		// buffer to get infos from a single line

    stRoad tmpRoad;				// tmp Road instance

    assert(file.is_open() && "ERROR readdata_Roads : File cannot be opened");

    try
    {
        std::getline(file, line);// the fist line contains titles of the columns
        while (std::getline(file, line))
        {
            buf.clear();
            buf << line;

            buf >> tmpRoad.ID;
            buf >> tmpRoad.name;
            buf >> tmpRoad.start;
            buf >> tmpRoad.end;
            buf >> tmpRoad.dist;
            buf >> tmpRoad.category;
            buf >> tmpRoad.rainDepending;

            result.push_back(tmpRoad);

            buf.clear();
        }

        //nR = result.size();
    } // end try
    catch (...)
    {
        file.close();
        std::cerr << "ERROR readdata_Roads : problem by reading from "
                  << fileName << std::endl;
    }

    file.close();

    return result;
}

std::vector<stHousehold> readdata_households(std::string fileName)
{
    std::cout << "...read households" << std::endl;

    std::vector<stHousehold> result;	// result vector

    std::ifstream file(fileName.c_str());	// file to open

    std::string line;			// single line in the file
    std::stringstream buf;		// buffer to get infos from a single line


    stHousehold tmpHh;				// tmp Household instance

    assert(
                file.is_open()
                && "ERROR readdata_households : File cannot be opened");

    try
    {
        std::getline(file, line);// the fist line contains titles of the columns
        while (std::getline(file, line))
        {
            buf.clear();
            buf << line;

            buf >> tmpHh.ID;
            buf >> tmpHh.name;
            buf >> tmpHh.type;
            buf >> tmpHh.nPersons;
            buf >> tmpHh.villageID;

            tmpHh.itime = (1-tmpHh.type)*10.  // 10 min for standart interview
                        +    tmpHh.type *25.*tmpHh.nPersons;  // 30 min for special cases
            result.push_back(tmpHh);

            buf.clear();
        }

        //H = result.size();

    } // end try
    catch (...)
    {
        file.close();
        std::cerr << "ERROR readdata_households : problem by reading from "
                  << fileName << std::endl;
    }

    file.close();

    return result;

}

