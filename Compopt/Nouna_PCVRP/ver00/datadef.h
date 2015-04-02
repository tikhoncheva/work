#ifndef DATADEF_H_
#define DATADEF_H_

#include "datadef.h"

#include <iostream>
#include <vector>
#include <utility>

/*
 * A household
 */
struct stHousehold
{
	unsigned int ID;
	std::string name;
	unsigned int type; // 1 or 0
	unsigned int nPersons;
	unsigned int villageID;

	void print()
	{
		std::cout << "ID=" << ID << " name=" << name << "  type=" << type
				<< "  nPersons=" << nPersons << "  villageID=" << villageID
				<< std::endl;
	}
};

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

/*
 * An interviewer
 */
struct Interviewer
{
	unsigned int ID;
};

unsigned int nH;		// number of households
unsigned int nV;
unsigned int K;		// number of interviewer

std::vector<stVillage> Village;
std::vector<stRoute> Route;
std::vector<stHousehold> Houshold;

#endif /* DATADEF_H_ */
