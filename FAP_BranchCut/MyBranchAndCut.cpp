#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

typedef IloArray<IloIntVarArray> MatrixIloIntVar;
typedef IloArray<IloNumArray> MatrixIloNum;

/*
 * Heuristic Callback
 */

//ILOHEURISTICCALLBACK1(MyHeuristic, IloNumVarArray, vars) {
ILOHEURISTICCALLBACK2(MyHeuristic, MatrixIloIntVar, varsX,
		const IloNumVarArray, varsY)
{
	IloArray<IntegerFeasibilityArray> feasX;
	IntegerFeasibilityArray feasY;

	MatrixIloNum x;
	IloNumArray y;
	const IloIntVar lambda;

	try
	{
		IloInt rowsX = varsX.getSize();
		IloInt colsX = varsX[0].getSize();
		IloInt colsY = varsY.getSize();

		feasX = IloArray<IntegerFeasibilityArray>(getEnv(), rowsX);
		for (int i = 0; i < rowsX; i++)
		{
			feasX[i] = IntegerFeasibilityArray(getEnv(), colsX);
			getFeasibilities(feasX[i], varsX[i]);
		}

		feasY = IntegerFeasibilityArray(getEnv());
		getFeasibilities(feasY, varsY);

		x = MatrixIloNum(getEnv(), rowsX);
		for (int i = 0; i < rowsX; i++)
		{
			x[i] = IloNumArray(getEnv(), colsX);
			for (unsigned int f = 0; f < colsX; f++)
			{
				x[i][f] = getValue(varsX[i][f]);
			}
		}

		y = IloNumArray(getEnv());
		getValues(y, varsY);

		IloNum lambda = getObjValue();


		/*
		 for (IloInt j = 0; j < cols; j++)
		 {
		 // Set the fractional variable to zero and update the objective value
		 if (feas[j] == Infeasible)
		 {
		 objval -= x[j] * obj[j];
		 x[j] = 0.0;
		 }
		 }
		 */
		//setSolution(varsY, y, lambda);
	} catch (...)
	{
		feasX.end();
		feasY.end();
		obj.end();
		//x.end();
		y.end();
		throw;
	}
	feasX.end();
	feasY.end();
	obj.end();
	//x.end();
	y.end();
}

/*
 * Cut Callback
 */

/*
 * Branch Selection Callback
 */ILOBRANCHCALLBACK1(MyBranch, IloNumVarArray, vars)
{
	if (getBranchType() != BranchOnVariable)
		return;

	// Branch on var with largest objective coefficient
	// among those with largest infeasibility

	IloNumArray x;
	IloNumArray obj;
	IntegerFeasibilityArray feas;

	try
	{
		x = IloNumArray(getEnv());
		obj = IloNumArray(getEnv());
		feas = IntegerFeasibilityArray(getEnv());
		getValues(x, vars);
		getObjCoefs(obj, vars);
		getFeasibilities(feas, vars);

		IloInt bestj = -1;
		IloNum maxinf = 0.0;
		IloNum maxobj = 0.0;
		IloInt cols = vars.getSize();
		for (IloInt j = 0; j < cols; j++)
		{
			if (feas[j] == Infeasible)
			{
				IloNum xj_inf = x[j] - IloFloor(x[j]);
				if (xj_inf > 0.5)
					xj_inf = 1.0 - xj_inf;
				if (xj_inf >= maxinf
						&& (xj_inf > maxinf || IloAbs(obj[j]) >= maxobj))
				{
					bestj = j;
					maxinf = xj_inf;
					maxobj = IloAbs(obj[j]);
				}
			}
		}

		if (bestj >= 0)
		{
			makeBranch(vars[bestj], x[bestj], IloCplex::BranchUp,
					getObjValue());
			makeBranch(vars[bestj], x[bestj], IloCplex::BranchDown,
					getObjValue());
		}
	} catch (...)
	{
		x.end();
		obj.end();
		feas.end();
		throw;
	}
	x.end();
	obj.end();
	feas.end();
}

/*
 * Node Selection Callback
 */ILONODECALLBACK0(MySelect)
{
	IloInt remainingNodes = getNremainingNodes();
	IloInt bestnode = -1;
	IloInt maxdepth = -1;
	IloNum maxiisum = 0.0;
	for (IloInt i = 0; i < remainingNodes; i++)
	{
		IloInt depth = getDepth(i);
		IloNum iisum = getInfeasibilitySum(i);
		if ((depth >= maxdepth) && (depth > maxdepth || iisum > maxiisum))
		{
			bestnode = i;
			maxdepth = depth;
			maxiisum = iisum;
		}
	}
	if (bestnode >= 0)
		selectNode(bestnode);
}

/*
 ILOCPLEXGOAL1(MyBranchGoal, IloNumVarArray, vars) {
 IloNumArray x;
 IloNumArray obj;
 IntegerFeasibilityArray feas;

 x    = IloNumArray(getEnv());
 obj  = IloNumArray(getEnv());
 feas = IntegerFeasibilityArray(getEnv());
 getValues(x, vars);
 getObjCoefs(obj, vars);
 getFeasibilities(feas, vars);

 IloInt bestj  = -1;
 IloNum maxinf = 0.0;
 IloNum maxobj = 0.0;
 IloInt cols = vars.getSize();
 for (IloInt j = 0; j < cols; j++) {
 if ( feas[j] == Infeasible ) {
 IloNum xj_inf = x[j] - IloFloor (x[j]);
 if ( xj_inf > 0.5 )
 xj_inf = 1.0 - xj_inf;
 if ( xj_inf >= maxinf                             &&
 (xj_inf > maxinf || IloAbs (obj[j]) >= maxobj)  ) {
 bestj  = j;
 maxinf = xj_inf;
 maxobj = IloAbs (obj[j]);
 }
 }
 }

 IloCplex::Goal res;
 if ( bestj >= 0 ) {
 res = AndGoal(OrGoal(vars[bestj] >= IloFloor(x[bestj])+1,
 vars[bestj] <= IloFloor(x[bestj])),
 this);
 }

 x.end();
 obj.end();
 feas.end();

 return res;
 }
 */
