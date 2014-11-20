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


std::vector<stRoute> readdata_routes(std::string fileName)
{
    std::cout << "...read routes" << std::endl;

    std::vector<stRoute> result;	// result vector

    std::ifstream file(fileName.c_str());	// file to open
    std::string line;			// single line in the file
    std::stringstream buf;		// buffer to get infos from a single line

    stRoute tmpRoute;				// tmp Route instance

    assert(file.is_open() && "ERROR readdata_routes : File cannot be opened");

    try
    {
        std::getline(file, line);// the fist line contains titles of the columns
        while (std::getline(file, line))
        {
            buf.clear();
            buf << line;

            buf >> tmpRoute.ID;
            buf >> tmpRoute.name;
            buf >> tmpRoute.start;
            buf >> tmpRoute.end;
            buf >> tmpRoute.dist;
            buf >> tmpRoute.category;
            buf >> tmpRoute.rain;

            result.push_back(tmpRoute);

            buf.clear();
        }

        //nR = result.size();
    } // end try
    catch (...)
    {
        file.close();
        std::cerr << "ERROR readdata_routes : problem by reading from "
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

