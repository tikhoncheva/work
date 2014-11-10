/*
 * readdata.h
 *
 *  Created on: Nov 6, 2014
 *      Author: kitty
 */

#ifndef READDATA_H_
#define READDATA_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

#include <assert.h>

#include "datadef.h"

std::vector<stVillage> readdata_villages(std::string fileName)
{
	std::cout << "...read villages" << std::endl;

	std::ifstream file(fileName.c_str());	// file to open

	std::string line;			// single line in the file
	std::stringstream buf;		// buffer to get infos from a single line

	std::vector<stVillage> result;	// result vector
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

//			std::cout << tmpVillage.ID << " " << tmpVillage.name << " "
//					<< tmpVillage.coord.first << " " << tmpVillage.coord.second
//					<< " " << std::endl;

			result.push_back(tmpVillage);

			buf.clear();
		}

	} // end try
	catch (...)
	{
		std::cerr << "ERROR readdata_villages : problem by reading from "
				<< fileName << std::endl;
	}

	file.close();
	return result;
}

std::vector<stRoute> readdata_routes(std::string fileName)
{
	std::cout << "...read routes" << std::endl;

	std::ifstream file(fileName.c_str());	// file to open

	std::string line;			// single line in the file
	std::stringstream buf;		// buffer to get infos from a single line

	std::vector<stRoute> result;	// result vector
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

//			std::cout << tmpRoute.ID << " " << tmpRoute.name << " "
//					<< tmpRoute.start << " " << tmpRoute.end << " "
//					<< tmpRoute.dist << " " << tmpRoute.category << " "
//					<< tmpRoute.rain << " " << std::endl;

			result.push_back(tmpRoute);

			buf.clear();

		}
	} // end try
	catch (...)
	{
		std::cerr << "ERROR readdata_routes : problem by reading from "
				<< fileName << std::endl;
	}

	file.close();
	return result;
}

std::vector<stHousehold> readdata_households(std::string fileName)
{
	std::cout << "...read households" << std::endl;

	std::ifstream file(fileName.c_str());	// file to open

	std::string line;			// single line in the file
	std::stringstream buf;		// buffer to get infos from a single line

	std::vector<stHousehold> result;	// result vector
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
	} // end try
	catch (...)
	{
		std::cerr << "ERROR readdata_households : problem by reading from "
				<< fileName << std::endl;
	}

	file.close();
	return result;
}

int readdata_csv(std::vector<std::string> fileName)
{
	std::cout << "Reading data" << std::endl;

	assert(fileName.size() == 3);

	Village = readdata_villages(fileName[0]);
	Route = readdata_routes(fileName[1]);
	Houshold = readdata_households(fileName[2]);

	N = Houshold.size();
	std::cout << "finished!" << std::endl;
	return 0;
}

#endif /* READDATA_H_ */
