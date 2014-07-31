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
#include <ilcp/cp.h>			/* CP */

#include "ProblemData.h"
#include "SPProblem.h"

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
	//double lambda_;
	int lambda;
	int UB;
	int LB;
	//double lambda_glob;
	//int lambda_glob;
	double time;
};

class MIP_Problem
{
private:
	void setModel_MIP(IloEnv& env, IloModel& model, const IloNumVar lambda,
			const IloIntVarArray c);
	void setModel_MIP_CSP(IloEnv& env, IloModel& model, const IloNumVar lambda,
			const IloIntVarArray c);
	void solve_MIP(const IloEnv env, const IloModel model, IloNumVar& lambda,
			IloIntVarArray& c);
	void solve_MIP_CSP(const IloEnv env, const IloModel model,
			IloNumVar& lambda, IloIntVarArray& c);
public:
	// input data
	CProblemData* inputData;
	// parameters
	MatrixDouble d; // use simply line-function
	//double UB; // lambda <= UB
	double max_distance;

	// Variables
	VectorInt C; // labeling
	VectorInt Z; // additional variables
	// Solution
	int withCSP;
	int solved;
	MIPSolution solution;
	SInfo info;

	// additional functions
	void print();
	//void set_d();

	int optimize_MIP();
	int optimize_MIP_SCP();

	void printSolution();
	int saveSolutionToFile(std::string _FileName);

	MIP_Problem(int max_d, CProblemData* inputData_) :
			max_distance(max_d) //, inputData(&inputData_)
	{
		inputData = inputData_;
		info.numAddVar = 0;
		info.numConstr = 0;
		info.numVar = 1 + inputData->n; // lambda + all c;

		solved = 0;
		solution.time = 0.;
		solution.lambda = 0;
		solution.LB = 0;
		solution.UB = 0;

		withCSP = 0;
		set_d_function();
	}

	~MIP_Problem()
	{

	}

private:
	double dist_linear(double x);
	void set_d_function();
};

double MIP_Problem::dist_linear(double x)
{
	double value = 1 + max_distance - x;
	return (value < 0) ? 0 : value; /* linear function of distances*/
	//return value;
}

void MIP_Problem::set_d_function()
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

void MIP_Problem::print()
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

void MIP_Problem::setModel_MIP(IloEnv& env, IloModel& model,
		const IloNumVar lambda, const IloIntVarArray c)
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

	if (solution.UB == 0)
		solution.UB = (max_distance + 1) * n;

	/* Objective */
	model.add(IloMinimize(env, lambda));

	/*Constrains*/
	model.add(lambda - solution.UB <= 0);
	/*
	 if (solution.LB == 0)
	 solution.LB = (inputData->maxGrad - 1) * (max_distance - 1) + max_distance;

	 model.add(lambda - solution.LB >= 0);

	 */

	int M; // = solution.UB;
	M = (max_distance + 1) * n;

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

void MIP_Problem::setModel_MIP_CSP(IloEnv& env, IloModel& model,
		const IloNumVar lambda, const IloIntVarArray c)
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

	if (solution.UB == 0)
		solution.UB = (max_distance + 1) * n;

	/* Objective */
	model.add(IloMinimize(env, lambda));

	/*Constrains*/
	model.add(lambda - solution.UB <= 0);
	/*
	 solution.LB = (inputData->maxGrad - 1) * (max_distance - 1) + max_distance;
	 model.add(lambda - solution.LB >= 0);

	 */

	/*
	 *  we solve the problem in this case without following conditions
	 *
	 int M = (max_distance + 1) * n;

	 for (int i = 0; i < info.numAddVar; i++)
	 {
	 int u = info.ConstrIndex[i * 2];
	 int v = info.ConstrIndex[i * 2 + 1];

	 model.add(c[v] - c[u] + M * z[u][v] >= Par_d[u][v]);
	 model.add(c[u] - c[v] + M * (1 - z[u][v]) >= Par_d[u][v]);
	 model.add(z[u][v] = z[v][u]);
	 }
	 */

	for (unsigned int v = 0; v < n; v++)
	{
		model.add(c[v] <= lambda);
		model.add(c[v] >= 0);
	}

}

void MIP_Problem::solve_MIP(const IloEnv env, const IloModel model,
		IloNumVar& lambda, IloIntVarArray& c)
{
	unsigned int n = inputData->n;
	int solveError = 0;

	IloCplex cplex(env);
	IloTimer timer(env);

	cplex.extract(model);
	cplex.exportModel("L-LabelingMIP.lp");

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
	solution.time = solution.time + timer.getTime();

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

void MIP_Problem::solve_MIP_CSP(const IloEnv env, const IloModel model,
		IloNumVar& lambda, IloIntVarArray& c)
{
	unsigned int n = inputData->n;

	IloCplex cplex(env);
	IloTimer timer(env);

	cplex.extract(model);
	cplex.exportModel("L-LabelingCSP.lp");

	solution.lambda = solution.LB;

	std::cout << "solution.lambda = " << solution.lambda << "\n";

	std::cout << "========================================" << " Solving ";
	std::cout << "========================================" << std::endl;

	CSP_Problem spproblem(n, solution.lambda, d);
	info.sp_runs = 1;

	std::fstream fileLog;
	fileLog.open("LOG.txt");
	fileLog << inputData->getFileName() << "\n";
	fileLog.close();

	while (spproblem.solve())
	{
		solution.time += spproblem.solution.time;
		info.sp_runs++;

		solution.lambda += 1;

		spproblem.set_lambda(solution.lambda);
	}

	//C = spproblem.get_c();

	solution.UB = spproblem.get_lambda();
	solution.lambda = 0;

	fileLog.open("LOG.txt",
			std::fstream::in | std::fstream::out | std::fstream::app);
	fileLog << "\n";
	fileLog.close();

}

int MIP_Problem::optimize_MIP()
{
	withCSP = 0;
	unsigned int n = inputData->n;
	IloEnv env;
	try
	{
		IloModel model(env);
		/* Variables */
		IloNumVar lambda(env, "lambda");

		IloIntVarArray c(env, n);
		for (unsigned int u = 0; u < n; u++)
		{
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u] = IloIntVar(env, str.c_str());
		}
		setModel_MIP(env, model, lambda, c); //	set constraints and all additional parameters
		solve_MIP(env, model, lambda, c); // solve problem
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

int MIP_Problem::optimize_MIP_SCP()
{
	withCSP = 1;
	unsigned int n = inputData->n;
	IloEnv env;
	try
	{
		IloModel model(env);
		/* Variables */
		IloNumVar lambda(env, "lambda");

		IloIntVarArray c(env, n);
		for (unsigned int u = 0; u < n; u++)
		{
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u] = IloIntVar(env, str.c_str());
		}
		setModel_MIP_CSP(env, model, lambda, c); //	set constraints and all addition parameters
		solve_MIP_CSP(env, model, lambda, c); // solve problem
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

	// Verification
	std::cout << std::endl << "==============  Verification =================="
			<< std::endl << std::endl;

	try
	{
		IloModel model(env);
		// Variables
		IloNumVar lambda(env, "lambda");

		IloIntVarArray c(env, n);
		for (unsigned int u = 0; u < n; u++)
		{
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u] = IloIntVar(env, str.c_str());
		}

		setModel_MIP(env, model, lambda, c); //	set constraints and all addition parameters
		model.add(lambda <= solution.UB);
		solve_MIP(env, model, lambda, c); // solve problem
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

void MIP_Problem::printSolution()
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

int MIP_Problem::saveSolutionToFile(std::string _fileName)
{

	std::ofstream outFile;

	std::string outFileNameSolve = "results/resultsSolve_";
	std::string outFileName;

	std::stringstream ss;
	ss << max_distance; //+ 1;
	std::string str = "d" + ss.str();
	outFileNameSolve += str;

	if (withCSP)
		outFileNameSolve += "_MMIP_CSP_";
	else
		outFileNameSolve += "_MMIP_";

	outFileName = outFileNameSolve;

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

/*
 void solveCP(const CProblemData* inputData, int max_distance)
 {
 unsigned int n = inputData->n;
 int UB = (max_distance + 1) * n;
 int LB = 0;
 int lambda = (UB - LB) / 2;
 double time = 0.;

 MatrixDouble d;
 VectorInt C;

 d.resize(n);
 for (unsigned int u = 0; u < n; u++)
 {
 d[u].resize(n);
 for (unsigned int v = 0; v < u; v++)
 {
 double value = 1 + max_distance - inputData->distances[u][v];

 d[u][v] = (value < 0) ? 0 : value;
 d[v][u] = d[u][v];
 }
 }
 std::cout << std::endl << "LB = " << LB << "\nUB = " << UB << std::endl;
 CSP_Problem spproblem(n, lambda, d);
 while (UB - LB > 1)
 {
 int solved;
 solved = spproblem.solve();
 time += spproblem.solution.time;

 if (solved == 0)
 {
 UB = lambda;
 spproblem.set_lambda(lambda);
 }
 else
 {
 LB = lambda;
 spproblem.set_lambda(lambda);
 }

 std::cout << std::endl << "LB = " << LB << "\nUB = " << UB << std::endl;
 lambda = (LB + UB) / 2;
 spproblem.set_lambda(lambda);
 }

 C = spproblem.get_c();

 std::cout << std::endl << "=============================================";
 if (lambda != UB)
 {
 spproblem.set_lambda(UB);
 if (spproblem.solve() == 0)
 {
 lambda = UB;
 C = spproblem.get_c();
 }
 }

 std::cout << std::endl << "=============================================";
 std::cout << std::endl;
 // print solution
 std::cout << "Optimal L(G; d_1,d_2, ..., d_n) = " << lambda << std::endl;
 std::cout << "with the optimal labeling:" << std::endl;
 for (unsigned int u = 0; u < C.size(); u++)
 {
 std::cout << C[u] << " ";
 }
 std::cout << std::endl;
 std::cout << "Spent time " << time << std::endl;
 }
 */

int main(int argc, char **argv)
{
	std::string fileName;

	if (argc > 5 || argc < 3)
//	if (argc != 5)
	{
		std::cerr
				<< "LLabeling <d max> <lattice>  [<file name to open>] <MIP|CP>\n";
		std::cerr << "lattice: 0 read from file;\n";
		std::cerr << "         1 hexagonal lattice;\n ";
		std::cerr << "         2 triangular lattice;\n ";
		std::cerr << "         3 square lattice\n";
		return 1;
	}

	CProblemData* inputData;
	int Verfahren;

	if (atoi(argv[2]) == 0) // if we want read from file
	{
		try
		{
			fileName = argv[3];
		} catch (...)
		{
			std::cerr << "Wrong file name to open" << std::endl;
			return -1;
		}
		std::cout << "Start with the file " << fileName << "\n";

		inputData = new CProblemData(fileName);
		Verfahren = atoi(argv[4]);
	}
	else // we work with a lattice graph
	{
		Verfahren = atoi(argv[3]);
		inputData = new CProblemData(atoi(argv[2]) /*lattice*/);
	}

	inputData->print();

	MIP_Problem MipProblem(atoi(argv[1])/*d_max*/, inputData);
//MipProblem.print();

	if (Verfahren == 0)
		MipProblem.optimize_MIP();
	else
		MipProblem.optimize_MIP_SCP();

	MipProblem.printSolution();
	MipProblem.saveSolutionToFile(inputData->getFileName());

	//solveCP(inputData, atoi(argv[1])/*d_max*/);
	return 0;
} // END main

