//============================================================================
// Name        : OPL_CAP.cpp
// Author      : 
// Version     :
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <cmath>
#include <algorithm>
#include <time.h>
#include <ilcplex/ilocplex.h>	/* CPLEX */

#include "ProblemData.h"

ILOSTLBEGIN

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> MatrixInt;

//typedef std::vector<double> VectorDouble;
//typedef std::vector<VectorDouble> MatrixDouble;

struct SInfo
{
	int numAddVar;
	int numConstr;
	int numVar;
	int sp_runs;

	VectorInt ConstrIndex; // Indices of the vertices, that interferent to eachother
};

struct MIPSolution
{
	double lambda;
	//double lambda_glob;
	//int lambda_glob;
	double time;
};

class IP_Problem
{
private:
	void coordTOdist();
	void setModel_IP(IloEnv& env, IloModel& model, const IloNumVar lambda,
			const IloNumVarArray c);
	void solve_IP(const IloEnv env, const IloModel model, IloNumVar& lambda,
			IloNumVarArray& c);
	
public:
	// input data
	CProblemData* inputData;
	// parameters
	MatrixDouble d; // use simply line-function
	double UB; // lambda <= UB
	double max_distance;

	// Variables
	VectorDouble C; // labeling
	VectorInt Z; // additional variables
	// Solution
	int solved;
	MIPSolution solution;
	SInfo info;

	// additional functions
	void print();
	//void set_d();

	int optimize_IP();

	void printSolution();
	int saveSolutionToFile(std::string _FileName);

	IP_Problem(int max_d, CProblemData* inputData_) :
			max_distance(max_d) //, inputData(&inputData_)
	{
		inputData = inputData_;
		info.numAddVar = 0;
		info.numConstr = 0;
		info.numVar = 1 + inputData->n; // lambda + all c;
		solved = 0;

		set_d_function();
	}

	~IP_Problem()
	{

	}

private:
	double dist_linear(double x);
	void set_d_function();
};

double IP_Problem::dist_linear(double x)
{
	double value = 1 + max_distance - x;
	return (value < 0) ? 0 : value; /* linear function of distances*/
	//return value;
}

void IP_Problem::set_d_function()
{

	d.resize(inputData->n);
	for (unsigned int u = 0; u < inputData->n; u++)
	{
		d[u].resize(inputData->n);
		for (unsigned int v = 0; v < u; v++)
		{
			d[u][v] = dist_linear(inputData->distances[u][v]);
			d[v][u] = d[u][v];
			if (d[u][v] <= max_distance && u != v)
			{ // save vertices for which additional constraints to be created
				info.numAddVar++;
				info.ConstrIndex.push_back(u);
				info.ConstrIndex.push_back(v);
			}
		}
	}
	info.numVar += info.numAddVar;
	std::cout << " numAddVar " << info.numAddVar << "\n";

}

void IP_Problem::print()
{
	std::cout << " Distance Function d: " << "\n";
	for (unsigned int u = 0; u < d.size(); u++)
	{
		for (unsigned int v = 0; v < d[u].size(); v++)
		{
			std::cout << std::setw(3) << d[u][v] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	//std::cout << "max distance : " << max_distance;
	std::cout << "\n";
}

void IP_Problem::setModel_IP(IloEnv& env, IloModel& model, const IloNumVar lambda,
		const IloNumVarArray c)
{

	unsigned int n = inputData->n;

	IloArray<IloIntVarArray> z(env, n);
	for (unsigned int u = 0; u < n; u++)
	{
		z[u] = IloIntVarArray(env, n);
		for (unsigned int v = 0; v < n; v++)
		{
			std::stringstream ss;
			ss << u;
			ss << v;
			std::string str = "z" + ss.str();
			z[u][v] = IloIntVar(env, 0, 1, str.c_str());
		}
	}
	/* Parameters */
	IloArray<IloNumArray> Par_d(env, n);
	for (unsigned int u = 0; u < n; u++)
	{
		Par_d[u] = IloNumArray(env, n);
		for (unsigned int v = 0; v < n; v++)
		{
			Par_d[u][v] = d[u][v];
		}
	}

	int M = (max_distance + 1) * n;

	/* Objective */
	model.add(IloMinimize(env, lambda));

	/*Constrains*/
	model.add(lambda - M <= 0);

	for (int i = 0; i < info.numAddVar; i++)
	{
		int u = info.ConstrIndex[i * 2];
		int v = info.ConstrIndex[i * 2 + 1];

		model.add(c[v] - c[u] + M * z[u][v] >= Par_d[u][v]);
		model.add(c[u] - c[v] + M * (1 - z[u][v]) >= Par_d[u][v]);
		model.add(z[u][v] = z[v][u]);
	}

	for (unsigned int v = 0; v < n; v++)
	{
		model.add(c[v] <= lambda);
		model.add(c[v] >= 0);
	}

}



void IP_Problem::solve_IP(const IloEnv env, const IloModel model,
		IloNumVar& lambda, IloNumVarArray& c)
{
	unsigned int n = inputData->n;
	int solveError = 0;

	IloCplex cplex(env);
	IloTimer timer(env);

	cplex.extract(model);
	cplex.exportModel("L-Labeling.lp");

	std::cout << "========================================" << " Solving ";
	std::cout << "========================================" << std::endl;

	std::cout << std::endl << "==================  IP ======================"
			<< std::endl << std::endl;
	timer.start();
	solveError = cplex.solve();
	timer.stop();

	if (!solveError)
	{
		std::cout << "STATUS : " << cplex.getStatus() << "\n";
		env.error() << "MIPOptimizer: failed to solve IP ;( \n";
		solved = 0;
		throw(std::string("MIPOptimizer: failed to solve IP ;( \n"));
	}

	solved = 1;
	solution.time = timer.getTime();

	env.out() << "\nSolution status IP = " << cplex.getStatus() << "\n";
	info.numConstr = cplex.getNrows();
	env.out() << "Number of constraints = " << info.numConstr << "\n";
	solution.lambda = cplex.getObjValue();
	env.out() << "Solution value IP = " << solution.lambda << "\n";

	for (unsigned int u = 0; u < n; u++)
	{
		C.push_back(cplex.getValue(c[u]));
	}
}


int IP_Problem::optimize_IP()
{
	unsigned int n = inputData->n;
	IloEnv env;
	try
	{
		IloModel model(env);
		/* Variables */
		IloNumVar lambda(env, "lambda");

		IloNumVarArray c(env, n);
		for (unsigned int u = 0; u < n; u++)
		{
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u] = IloNumVar(env, str.c_str());
		}
		setModel_IP(env, model, lambda, c); //	set constraints and all addition parameters
		solve_IP(env, model, lambda, c); // solve problem
	} // end try
	catch (IloException& e)
	{
		solved = 0;
		std::cerr << "Concert exception caught: " << e << std::endl;
	} catch (std::string& e)
	{
		solved = 0;
		std::cerr << e << std::endl;
	} catch (...)
	{
		solved = 0;
		std::cerr << "Unknown exception caught" << std::endl;
	}
	env.end();
	return 0;
}


void IP_Problem::printSolution()
{
	std::cout << std::endl << "==================================="
			<< std::endl;
	std::cout << std::endl << "SOLUTION MIP OPTIMIZER:" << std::endl;

	if (!solved)
		std::cout << "The problem wasn't solved yet!\n";
	else
	{
		std::cout << "Spend time " << solution.time << std::endl;
		std::cout << "Optimal L(G; d_1,d_2, ..., d_n) = " << solution.lambda
				<< std::endl;
		std::cout << "with the optimal labeling:" << std::endl;
		for (unsigned int u = 0; u < C.size(); u++)
		{
			std::cout << C[u] << " ";
		}
	}
	std::cout << std::endl << "==================================="
			<< std::endl;
}

int IP_Problem::saveSolutionToFile(std::string _fileName)
{

	std::ofstream outFile;

	std::string outFileNameSolve = "results/resultsSolve_";
	std::string outFileName;

	std::stringstream ss;
	ss << max_distance + 1;
	std::string str = "d " + ss.str();
	outFileNameSolve += str;

	outFileNameSolve += "_function_";

	outFileName = outFileNameSolve;

	/*
	 unsigned found = inputData->fileName.find_last_of("/");
	 std::string line = inputData->fileName.substr(found + 1,
	 inputData->fileName.length() - 1 - found);
	 */
	outFileName = outFileName + _fileName; //line;

	outFile.open(outFileName.c_str());
	if (!outFile.is_open())
	{
		std::cerr << "Cannot open file to save a result!\n";
		exit(1);
	}

	outFile << "Name of file:\t" << _fileName << "\n";
	outFile << "Optimal value:\t" << solution.lambda << "\n";
	outFile << "Number of constraints: " << info.numConstr << "\n";
	outFile << "Number of variables: " << info.numVar << "\n";

	if (solved)
	{
		outFile << "Used time: " << solution.time << "sec\n";
		outFile << "Labeling:\n";
		for (unsigned int u = 0; u < C.size(); u++)
			outFile << "c[" << u + 1 << "]=" << C[u] << " ";

		outFile.close();
	}
	return 0;
}

int main(int argc, char **argv)
{
	if (argc > 4 || argc < 2)
	{
		std::cerr << "LLabeling <d max> <lattice> <file name to open> \n";
		std::cerr << "lattice: 0 read from file;\n";
		std::cerr << "         1 hexagonal lattice;\n ";
		std::cerr << "        2 triangular lattice;\n ";
		std::cerr << "        3 square lattice\n";
		return 1;
	}

	CProblemData* inputData;

	if (atoi(argv[2]) == 0) // if we want read from file
	{
		std::string fileName = argv[3];
		std::cout << "Start with the file " << fileName << "\n";

		inputData = new CProblemData(fileName);
	}
	else // we work with a lattice graph
	{
		inputData = new CProblemData(atoi(argv[2]) /*lattice*/);
	}

	inputData->print();

	IP_Problem MipProblem(atoi(argv[1])/*d_max*/, inputData);
	MipProblem.print();

	MipProblem.optimize_IP();
	
	MipProblem.printSolution();
	MipProblem.saveSolutionToFile(inputData->getFileName());
	return 0;
} // END main

