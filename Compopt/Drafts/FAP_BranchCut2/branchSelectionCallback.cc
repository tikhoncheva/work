#include "var.h"
/*
 * Branch Selection Callback
 */
ILOBRANCHCALLBACK1(MyBranch, IloIntVarArray, varsC)
{
	IntegerFeasibilityArray feasC;
	IloNumArray c;
	int n;

	try
	{
		n = varsC.getSize();
		c = IloNumArray(getEnv());
		feasC = IntegerFeasibilityArray(getEnv());

		getValues(c, varsC);
		//getObjCoefs(obj, varsC);
		getFeasibilities(feasC, varsC);

		std::cout << "Current Labeling: " << std::endl;
		for (int i=0; i< n; i++)
		{
			std::cout << " " << c[i];
		}
		std::cout << std::endl;

		std::cout << "Feasibilities: " << std::endl;
		for (int i=0; i< n; i++)
		{
			std::cout << " " << feasC[i];
		}
		std::cout << std::endl;

		IloInt bestj = -1;
		IloNum maxinf = 0.0;

		for (IloInt j = 0; j < n; j++)
		{
			if ( feasC[j] == Infeasible )
			{
				IloNum cj_inf = c[j] - IloFloor (c[j]);

				if ( cj_inf > 0.5 )
				cj_inf = 1.0 - cj_inf;
				if ( cj_inf >= maxinf)
				{
					bestj = j;
					maxinf = cj_inf;
				}
			}
		}

		if ( bestj >= 0 )
		{
			makeBranch(varsC[bestj], c[bestj], IloCplex::BranchDown, getObjValue());
			makeBranch(varsC[bestj], c[bestj], IloCplex::BranchUp, getObjValue());
		}
	}
	catch (...)
	{
		c.end();
		feasC.end();
		throw;
	}
	c.end();
	feasC.end();
}

ILOBRANCHCALLBACK1(MyBranch_new, IloIntVarArray, varsC)
{
	IntegerFeasibilityArray feasC;
	IloNumArray c;
	int n;

	try
	{
		n = varsC.getSize();
		c = IloNumArray(getEnv());
		feasC = IntegerFeasibilityArray(getEnv());

		getValues(c, varsC);
		//getObjCoefs(obj, varsC);
		getFeasibilities(feasC, varsC);

		std::cout << "Current Labeling: " << std::endl;
		for (int i=0; i< n; i++)
		{
			std::cout << " " << c[i];
		}
		std::cout << std::endl;

		std::cout << "Feasibilities: " << std::endl;
		for (int i=0; i< n; i++)
		{
			std::cout << " " << feasC[i];
		}
		std::cout << std::endl;

		IloInt i,j;

		for ( i = 0; i < n; i++)
		{
			for ( j = 0; j < n; j++)
			{
				if ((c[i]-c[j]<IG[i][j] || c[i]-c[j]>-IG[i][j]) && i!=j)
				break;
			}
		}

		if ( i<n && j<n )
		{

			IloInt tmp1 = std::max(c[i]-IG[i][j], 0.0);
			IloInt tmp2 = c[i]+IG[i][j];

			makeBranch(varsC[j], tmp1, IloCplex::BranchDown, getObjValue());
			makeBranch(varsC[j], tmp2, IloCplex::BranchUp, getObjValue());
		}

	}
	catch (...)
	{
		c.end();
		feasC.end();
		throw;
	}
	c.end();
	feasC.end();
}

ILOBRANCHCALLBACK2(MyBranch2, IloIntVarArray, varsC, const MatrixInt &, vDeg)
{
	IntegerFeasibilityArray feasC;
	IloNumArray c;
	int n;

	try
	{
		n = varsC.getSize();
		c = IloNumArray(getEnv());
		feasC = IntegerFeasibilityArray(getEnv());

		getValues(c, varsC);
		//getObjCoefs(obj, varsC);
		getFeasibilities(feasC, varsC);

		std::cout << "Current Labeling: " << std::endl;
		for (int i=0; i< n; i++)
		{
			std::cout << " " << c[i];
		}
		std::cout << std::endl;

		std::cout << "Feasibilities: " << std::endl;
		for (int i=0; i< n; i++)
		{
			std::cout << " " << feasC[i];
		}
		std::cout << std::endl;

		IloInt bestj = -1;
		IloInt maxDeg = 0;

		for (IloInt j = 0; j < n; j++)
		{
			if ( feasC[j] == Infeasible )
			{
				int deg = vDeg[j][0];

				if ( deg >= maxDeg)
				{
					bestj = j;
					maxDeg = deg;
				}
			}
		}

		if ( bestj >= 0 )
		{
			makeBranch(varsC[bestj], c[bestj], IloCplex::BranchDown, getObjValue());
			makeBranch(varsC[bestj], c[bestj], IloCplex::BranchUp, getObjValue());
		}

	}
	catch (...)
	{
		c.end();
		feasC.end();
		throw;
	}
	c.end();
	feasC.end();
}
