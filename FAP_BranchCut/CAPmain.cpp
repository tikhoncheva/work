//============================================================================
// Name        : CAPmain.cpp
// Author      : 
// Version     :
// Comments    : main fuction
//============================================================================

#include <iostream>
#include <iomanip>

#include "CAP.cpp"

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

		//	inputData->print();

		MIP_Problem CAP(atoi(argv[1])/*d_max*/, inputData);	// define new CAP Problem
		CAP.print();										// print problem
		CAP.solve();										// solve problem
		CAP.printSolution();								// print solution on the screen
		CAP.saveSolutionToFile(inputData->getFileName());	// save solution in file

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

