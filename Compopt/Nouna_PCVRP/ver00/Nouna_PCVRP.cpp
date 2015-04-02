//============================================================================
// Name        : Nouna_PCVRP.cpp
// Author      : Ekaterina Tikhoncheva
// Version     : 1.0
//============================================================================

#include <iostream>
#include <array>
#include "datadef.h"
#include "readdata.h"
#include "graphics.h"

/*
 * correct call format
 */
void usage()
{

}

int main(int argc, char** argv)
{
	int error;

	tets_opengl (argc, argv);
	std::vector<std::string> fileName(3);
	fileName[0] = "../data/villages.csv";
	fileName[1] = "../data/routes.csv";
	fileName[2] = "../data/households.csv";

	// read data
	error = readdata_csv(fileName);

	Houshold[0].print();
	Village[0].print();
	Route[0].print();
	std::cout << "\nFinished main" << std::endl;
	return 0;
}
