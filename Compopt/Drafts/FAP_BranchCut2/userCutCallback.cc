#include <ilcplex/ilocplexi.h>
#include "var.h"

ILOUSERCUTCALLBACK1(MyUserCutCallback2, IloExprArray, lhs)
{
	std::cout << "add cutts\n";
	IloNum eps = 0.2;
	IloInt n = lhs.getSize();
	int numViolatedCliqueConst = 0;
	for (IloInt i = 0; i < n; i++)
	{
		if ( getValue(lhs[i]) > 1 + eps )
		{

			numViolatedCliqueConst ++;
			IloRange cut;
			try
			{
				cut = (lhs[i] <= 1);
				add(cut).end();
			}
			catch (...)
			{
				cut.end();
				throw;
			}
		}
	}

	std::cout << "Number of violated Clique-Constraints : " << numViolatedCliqueConst<< std::endl;
}

ILOUSERCUTCALLBACK2(MyUserCutCallback1, MatrixIloIntVar, varsX,
		IloNumVarArray, varsY)
{
	MatrixIloNum x;
	IloNumArray y;
	const IloIntVar lambda;

	try
	{

		IloInt nLinks = varsX.getSize();
		IloInt nFreq = varsX[0].getSize();

		x = MatrixIloNum(getEnv(), nLinks);
		for (int i = 0; i < x.getSize(); i++)
		{
			x[i] = IloNumArray(getEnv(), nFreq);
			for (int f = 0; f < x[i].getSize(); f++)
			{
				x[i][f] = getValue(varsX[i][f]);
			}
		}

		y = IloNumArray(getEnv());
		getValues(y, varsY);

		for (int f = 0; f < y.getSize(); f++)
		{
			IloExpr sum_xf(getEnv());
			for (int i = 0; i < x.getSize(); i++)
			{
				for (int j = 0; j < x.getSize(); j++)
				{
					if (IG[i][j]==max_dist)
					{
						//sum_xf += x[i][f]+x[j][f];
					}
				}
			}
			add(sum_xf <= y[f]);
			sum_xf.end();
		}

	}
	catch (...)
	{

		x.end();
		y.end();
		throw;
	}

	x.end();
	y.end();
}

