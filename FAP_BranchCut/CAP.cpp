//============================================================================
// Name        : OPL_CAP.cpp
// Author      : 
// Version     :
// Comments    :
//============================================================================

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <cmath>
#include <algorithm>
#include <time.h>
#include <ilcplex/ilocplex.h>	/* CPLEX */

#include "var.h"
#include "ProblemData.h"
#include "MyBranchAndCut.cpp"
#include "userCutCallback.cc"

ILOSTLBEGIN

/*
 struct SInfo
 {
 int numConstr;
 int numVar;
 int numRedundantVar;

 VectorInt ConstrIndex; // Indices of the vertices, that interferent to eachother
 };
 */
struct MIPSolution
{
	double UB;
	double LB;
	double lambda;

	double time;
};

class MIP_Problem
{
private:
//	int dist_linear(int x);
//	void set_d_function();

	void setModel(IloEnv& env, IloModel& model, const IloIntVar lambda,
			const IloArray<IloIntVarArray> x, const IloNumVarArray y);
	void optimize(const IloEnv env, const IloModel model,
			const IloIntVar lambda, const IloArray<IloIntVarArray> x,
			const IloNumVarArray y, IloArray<IloNumArray> dist);

public:
	// input data
	CProblemData* inputData;
	// parameters
	//MatrixInt d; // use simply line-function to calculate d from distance matrix
	int max_distance;

	// Variables
	VectorInt C; // labeling

	// Solution
	int solved;
	MIPSolution solution;
	//SInfo info;

	// additional functions
	//void set_d_function();

	int solve();

	void print();
	void printSolution();
	int saveSolutionToFile(std::string _FileName);

	MIP_Problem(int max_d, CProblemData* inputData_) :
			max_distance(max_d)
	{
		inputData = inputData_;
		info.numConstr = 0;
		info.numVar = inputData->n; // lambda + all c;
		info.numRedundantVar = 0;
		solved = 0;

		//set_d_function();
	}

	~MIP_Problem()
	{

	}

};

/*
 int MIP_Problem::dist_linear(int x)
 {
 int value = 1 + max_distance - x;
 return (value < 0) ? 0 : value;
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
 }
 }

 }
 */

void MIP_Problem::setModel(IloEnv& env, IloModel& model, const IloIntVar lambda,
		const IloArray<IloIntVarArray> x, const IloNumVarArray y)
{

	/* Parameters */
	unsigned int n = inputData->n;

	/* Objective */
	model.add(IloMinimize(env, lambda));
	//model.add(IloMinimize(env, IloSum(y)));

	/*Constrains*/

	for (int f = 0; f <= solution.UB; f++)
	{
		model.add(lambda >= f * y[f]);
	}

	for (unsigned int i = 0; i < n; i++)
	{
		IloExpr SumFreq(env);

		for (unsigned int f = 0; f <= solution.UB; f++)
		{
			SumFreq += x[i][f];
			model.add(x[i][f] <= y[f]);
		}

		model.add(SumFreq == 1);
		SumFreq.end();

	}

	// x[i][f]+x[j][g] <=1 |f-g|<d_ij
	for (unsigned int i = 0; i < info.InterferenceVertices.size() / 2; i++)
	{
		int u = info.InterferenceVertices[i * 2];
		int v = info.InterferenceVertices[i * 2 + 1];

		for (int f = 0; f <= solution.UB; f++)
		{
			for (int g = 0; g <= solution.UB; g++)
			{
				if ((f - g < d[u][v]) && (f - g > -d[u][v]))
				{
					model.add(x[u][f] + x[v][g] <= 1);
				}
			}
		}
	}

	// add clique-constraints
/*
	IloIntVarArray c(env, n);
	for (unsigned int i = 0; i < n; i++)
	{
		std::stringstream ss;
		ss << i;
		std::string str = "c" + ss.str();
		c[i] = IloIntVar(env, 0, solution.UB, str.c_str());
	}

	for (unsigned int i = 0; i < n; i++)
	{
		IloExpr varLabels(env);

		for (unsigned int f = 0; f <= solution.UB; f++)
		{
			IloInt ff = f;
			varLabels += x[i][f] * ff;
		}

		model.add(c[i] == varLabels);
		varLabels.end();

	}

	// first find maximal cliques in the given graph
	std::vector<std::set<int> > cliqueList = findMaxClique(d);
	info.numOfCliques = cliqueList.size();

	for (unsigned int i = 0; i < cliqueList.size(); i++)
	{
		std::set<int>::iterator itC;
		std::set<int> C = cliqueList[i];

		IloExpr cliqueConst(env);
		int sizeOfClique = 0;
		for (itC = C.begin(); itC != C.end(); itC++)
		{
			cliqueConst += c[*itC];
			sizeOfClique++;
		}
		model.add(
				cliqueConst
						>= max_distance * sizeOfClique * (sizeOfClique - 1)
								/ 2.);
		cliqueConst.end();

	}
	*/

}

void MIP_Problem::optimize(const IloEnv env, const IloModel model,
		const IloIntVar lambda, const IloArray<IloIntVarArray> x,
		const IloNumVarArray y, IloArray<IloNumArray> dist)
{
	unsigned int n = inputData->n;
	int solveError = 0;

	IloCplex cplex(env);
	IloTimer timer(env);

	//cplex.use(MyHeuristic(env, x, y));
	//cplex.use(MyBranch(env, x, y));
	//cplex.use(MySelect(env));

	cplex.setParam(IloCplex::MIPSearch, IloCplex::Traditional);
	//cplex.setParam(IloCplex::Cliques, 1);

	cplex.extract(model);

	// fill cuts-table

	// first find maximal cliques in the given graph
	std::vector<std::set<int> > cliqueList = findMaxClique(d);
	info.numOfCliques = cliqueList.size();

	//IloRangeArray cuts(env);
	//makeCuts(env, cuts, x, cliqueList);
	//cplex.addUserCuts(cuts);

	//IloRangeArray lhs(env);
	//makeCuts2(env,lhs, x, cliqueList);
	//makeCuts3(env,lhs, x, cliqueList);

	//cplex.use(MyUserCutCallback1(env, x, y));
	//cplex.use(MyUserCutCallback2(env, lhs));

	cplex.exportModel("LLabelingIP.lp");

	std::cout << "========================================" << " Solving ";
	std::cout << "========================================" << std::endl;

	//cplex.setParam(IloCplex::ClockType, 1);
	timer.start();
	//solveError = cplex.solve(MyBranchGoal(env, y));
	solveError = cplex.solve();
	timer.stop();

	if (!solveError)
	{
		std::cout << "STATUS : " << cplex.getStatus() << "\n";
		//env.error() << "MIPOptimizer: failed to solve IP ;( \n";
		solved = 0;
		throw(std::string("MIPOptimizer: failed to solve IP ;( \n"));
	}

	solved = 1;
	solution.time = timer.getTime();

	env.out() << "\nSolution status IP = " << cplex.getStatus() << "\n";
	env.out() << "Number of variables = " << info.numVar << "\n";
	env.out() << "Frequency Range = " << info.frequencyRange << "\n";
	env.out() << "Number of redundant variables = " << info.numRedundantVar
			<< "\n";
	info.numConstr = cplex.getNrows();
	env.out() << "Number of constraints = " << info.numConstr << "\n";
	env.out() << "Number of Cliques = " << info.numOfCliques << "\n";
	solution.lambda = cplex.getObjValue();

	//env.out() << "Solution value IP = " << solution.lambda << "\n";

	C.resize(n);
	for (unsigned int i = 0; i < n; i++)
	{
		for (unsigned int f = 0; f <= solution.UB; f++)
		{
			if (cplex.getValue(x[i][f]) == 1)
			{
				C[i] = f;
			}
		}
	}

	//cuts.end();
	//lhs.end();

}

int MIP_Problem::solve()
{
	unsigned int n = inputData->n;
	IloEnv env;
	try
	{
		IloModel model(env);
		/* Variables */
		IloIntVar lambda(env, "lambda");

		solution.UB = (max_distance + 1) * n;

		// D_i = D = {0, 1,2,3,...UB}

		info.frequencyRange = 1 + solution.UB;

		// distances
		MatrixIloNum IloD(env, n);
		for (unsigned int i = 0; i < n; i++)
		{
			IloD[i] = IloNumArray(env, n);
			for (unsigned int j = 0; j < n; j++)
			{
				IloD[i][j] = d[i][j];
			}
		}

		MatrixIloIntVar x(env, n);
		for (unsigned int i = 0; i < n; i++)
		{
			x[i] = IloIntVarArray(env, solution.UB + 1);
			for (unsigned int f = 0; f <= solution.UB; f++)
			{
				std::stringstream ss;
				ss << i;
				ss << f;
				std::string str = "x" + ss.str();
				x[i][f] = IloIntVar(env, 0, 1, str.c_str());
			}
		}

		IloNumVarArray y(env, solution.UB + 1);
		for (unsigned int f = 0; f <= solution.UB; f++)
		{
			std::stringstream ss;
			ss << f;
			std::string str = "y" + ss.str();
			y[f] = IloNumVar(env, 0, 1, str.c_str());
		}

		info.numVar = 1 + n * (solution.UB + 1) + (solution.UB + 1);

		//	set constraints and all addition parameters
		setModel(env, model, lambda, x, y);

		// solve problem
		optimize(env, model, lambda, x, y, IloD);

		x.end();
		y.end();
		IloD.end();
		lambda.end();
		model.end();

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

void MIP_Problem::print()
{
	std::cout << " Interference graph of the problem: " << "\n";
	for (unsigned int u = 0; u < d.size(); u++)
	{
		for (unsigned int v = 0; v < d[u].size(); v++)
		{
			std::cout << std::setw(3) << d[u][v] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "max distance : " << max_dist;
	std::cout << "\n";
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

