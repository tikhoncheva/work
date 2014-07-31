/*
 * CSPProblem.h
 */

#ifndef CSPPROBLEM_H_
#define CSPPROBLEM_H_

#include <vector>
#include <ilcp/cpext.h>	/* CP */

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> MatrixInt;

struct CSPSolution
{
	// int success;
	double time;
	int lambda;
	VectorInt C;
};

class CSP_Problem
{
private:
	unsigned int n;
	int lambda;
	VectorInt grade;
	MatrixDouble d;
	std::string fileName;

	void setModel(IloEnv& env, IloModel& model, const IloIntVarArray c);
	int solveModel(const IloEnv env, const IloModel model, IloIntVarArray& c);IlcGoal search();
public:
	CSPSolution solution;

	CSP_Problem(int n_, int lambda_, MatrixDouble d_) :
			n(n_), lambda(lambda_), d(d_)
	{
		solution.C.resize(n, 0);
	}
	void set_lambda(int lambda_)
	{
		lambda = lambda_;

	}

	const int get_lambda()
	{
		return solution.lambda;
	}
	const VectorInt get_c()
	{
		return solution.C;
	}

	int solve();
	void printSolution();
	int saveSolutionToFile(std::string _FileName);
};

void CSP_Problem::setModel(IloEnv& e, IloModel& model, const IloIntVarArray c)
{

	// Constrains to be satisfied
	/*
	 for (unsigned int v = 0; v < n; v++)
	 {
	 model.add(c[v] <= lambda);
	 model.add(c[v] >= 0);
	 }
	 */
	for (unsigned int u = 0; u < n; u++)
	{
		for (unsigned int v = 0; v < n; v++)
		{
			model.add(c[v] - c[u] >= d[u][v] || c[u] - c[v] >= d[u][v]);
		}
	}
}

ILOSTLBEGIN

ILCGOAL2(MyGoalPhase, IlcInt, d, IlcIntVarArray, x)
{
	// First choose a variable to instantiate.
	// choose variable with the smallest domain
	IlcInt i = IlcChooseMinSizeInt(x);

	// Goal succeeded?
	if (i < 0)
		return 0;

	// Now choose the best value.
	// select smallest possible value from the domain
	IlcInt j = x[i].getMin();
	//IlcInt j = g[i];

	// branching
	IlcGoal left = IlcAnd(x[i] == j, this);
	if (d == 0)
	{
		return left;
	}
	else
		return IlcOr(left, IlcAnd(x[i] != j, MyGoalPhase(getCP(), d - 1, x)));
}

ILCGOAL3(MyGoalSBS, IlcInt, d, IlcInt, maxD, IlcIntVarArray, x)
{
	IloCP cp = getCP();
	if (d > maxD)
	{
		return IlcGoalFail(cp);
	}
	return IlcOr(MyGoalPhase(cp, d, x), MyGoalSBS(cp, d + 1, maxD, x));
}

ILOCPGOALWRAPPER2(MyGoalSBS, cp, IloInt, maxD, IloIntVarArray, x)
{
	return MyGoalSBS(cp, 0, maxD, cp.getIntVarArray(x));

}

int CSP_Problem::solveModel(const IloEnv env, const IloModel model,
		IloIntVarArray& c)
{

	std::cout << std::endl << "==================  CSP ======================"
			<< std::endl << std::endl;

	// use cpoptimizer
	IloCP cp(model);
	cp.setParameter(IloCP::LogVerbosity, IloCP::Quiet);
	//cp.setParameter(IloCP::AllDiffInferenceLevel, IloCP::Extended);
	//cp.setParameter(IloCP::SearchType, IloCP::DepthFirst);
	cp.setParameter(IloCP::Workers, 2);

	IloTimer timer(env);

	//int success;
	std::fstream fileLog;
	fileLog.open("LOG.txt",
			std::fstream::in | std::fstream::out | std::fstream::app);

	int check = 0;
	int depth = 3;
	//for (int depth = 2, i = 1; i <= 4; i++, depth *= 2)
	{
		timer.start();
		//success =
				cp.solve(MyGoalSBS(env, depth, c));
		timer.stop();
		if (cp.getStatus() == cp.Feasible)
		{
			check++;
			//break;
		}

		cp.out() << "LAMBDA = " << lambda << std::endl;
		cp.out() << "SOLUTION STATUS : " << cp.getStatus() << "\n";
		solution.time = timer.getTime();
		std::cout << "CSPent time " << solution.time << std::endl;

		// write in LOG
		fileLog << depth << " " << lambda << " " << cp.getStatus() << "\t";
	}

	fileLog << "\n";
	fileLog.close();
	if (check > 0)
	//if (success)
	{
		// get solution from optimizer
		solution.lambda = lambda;
		for (unsigned int u = 0; u < n; u++)
		{
			solution.C[u] = cp.getIntValue(c[u]);
		}
	}

	else
	{
		throw(std::string("CPOptimizer: Failed to solve CSP :( \n"));
	}

	return 0;
}

int CSP_Problem::solve()
{
	IloEnv env;

	try
	{
		IloModel model(env);
		// Variables
		IloIntVarArray c(env, n, 0, lambda);
		/*
		 for (unsigned int u = 0; u < n; u++)
		 {
		 std::stringstream ss;
		 ss << u;
		 std::string str = "c" + ss.str();
		 c[u] = IloIntVar(env, str.c_str());
		 }
		 */
		// set parameters
		setModel(env, model, c);
		// solve model
		solveModel(env, model, c);
	} catch (IloException& e)
	{
		std::cerr << "Concert exception caught: " << e << std::endl;
		return (-1);
	} catch (std::string& str)
	{
		std::cerr << "SCP solve caught: " << str << std::endl;
		return (-1);
	} catch (...)
	{
		std::cerr << "Unknown exception caught" << std::endl;
		return (-1);
	}
	env.end();

	return 0;
}

void CSP_Problem::printSolution()
{
	std::cout << std::endl << "==================================="
			<< std::endl;
	std::cout << std::endl << "SOLUTION CP OPTIMIZER:" << std::endl;
	std::cout << "CSPent time " << solution.time << std::endl;
	std::cout << "Optimal L(G; d_1,d_2, ..., d_n) = " << solution.lambda
			<< std::endl;
	std::cout << "with the optimal labeling:" << std::endl;
	for (unsigned int u = 0; u < n; u++)
	{
		std::cout << solution.C[u] << " ";
	}
	std::cout << std::endl << "==================================="
			<< std::endl;
}

class Search
{
private:
	IloIntArray _domains;
public:
	Search(IloCP cp, IloIntArray domains);
	//IlcGoal smallestDomain(IloCP cp);

};

#endif /* CSPPROBLEM_H_ */
