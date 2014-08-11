//============================================================================
// Name        : CAPmain.cpp
// Author      : 
// Version     :
// Comments    : main fuction
//============================================================================

#include <iostream>
#include <iomanip>

#include "CAP.cpp"
#include "var.h"

static void usage(const char *progname)
{
	std::cerr << "Usage: " << progname << " <d max> <lattice> <filename>"
			<< std::endl;
	std::cerr << "   where  <d max> is d_1 for L(d_1,d_1-1,..,1)-Labeling"
			<< std::endl;
	std::cerr << "   <lattice>= 1 for hexagonal lattice;"
			<< " 2 for triangular lattice; 3 for square lattice and "
			<< " 0 for arbitrary graph defined in inputfile" << std::endl;

	std::cerr
			<< "   <filename> is a file with distance matrix and number of vertices in graph "
			<< std::endl;
	std::cerr << " Exiting..." << std::endl;
} // END usage

int main(int argc, char **argv)
{
	try
	{
		if (argc != 4)
		{
			usage(argv[0]);
			throw(-1);
		}

		max_dist = atoi(argv[1]); // d_1
		CProblemData* inputData;

		if (atoi(argv[2]) == 0) // if we want read from file
		{
			std::string fileName = argv[3];
			std::cout << "\n\nStart with the file " << fileName << "\n";
			inputData = new CProblemData(fileName);
		}
		else // we work with a predefined lattice graph
		{
			inputData = new CProblemData(atoi(argv[2]) /*lattice typ*/);
		}

		//inputData->print();

		std::vector<std::set<int> > cliqueList = findMaxClique(d);

		std::cout << "Maximal clique in the given Graph " << std::endl;

		for (unsigned int i = 0; i < cliqueList.size(); i++)
		{
			std::set<int>::iterator itC;
			std::set<int> C = cliqueList[i];

			std::cout << "Clique " << i + 1 << ": ";

			for (itC = C.begin(); itC != C.end(); itC++)
			{
				std::cout << *itC << "<->";
			}
			std::cout << "|" << std::endl;
		}

		/*
		 MIP_Problem CAP(max_dist, inputData); // define new CAP Problem
		 CAP.print(); // print problem

		 CAP.solve(); // solve problem
		 CAP.printSolution(); // print solution on the screen
		 CAP.saveSolutionToFile(inputData->getFileName()); // save solution in file

		 */
	} // END try
	catch (int& e)
	{
		std::cerr << "Error in input parameters " << e << std::endl;
	} catch (...)
	{
		std::cerr << "Unknown exception caught" << std::endl;
	}

	return 0;
}

