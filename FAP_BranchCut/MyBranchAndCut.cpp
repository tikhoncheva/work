//#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include "var.h"

ILOSTLBEGIN

int checkIfAssigned(IloNumArray feas)
{
	for (int i = 0; i < feas.getSize(); i++)
		if (feas[i] > 0.5)
		{
			return i;
			break;
		}
	return -1;

}

ILOHEURISTICCALLBACK3(MyHeuristic, MatrixIloIntVar, varsX,
		const IloNumVarArray, varsY, MatrixIloNum, dist)
{
	IloArray<IntegerFeasibilityArray> feasX;
	IntegerFeasibilityArray feasY;

	MatrixIloNum x;
	IloNumArray y;
	const IloIntVar lambda;

	try
	{
		//IloNum lambda = getObjValue();

		IloInt nLinks = varsX.getSize();
		IloInt nFreq = varsX[0].getSize();

		/*
		 feasX = IloArray<IntegerFeasibilityArray>(getEnv(), nLinks);
		 for (int i = 0; i < feasX.getSize(); i++)
		 {
		 feasX[i] = IntegerFeasibilityArray(getEnv(), nFreq);
		 getFeasibilities(feasX[i], varsX[i]);
		 }

		 feasY = IntegerFeasibilityArray(getEnv());
		 getFeasibilities(feasY, varsY);
		 */

		std::set<int> assignedLinks;

		x = MatrixIloNum(getEnv(), nLinks);
		for (int i = 0; i < x.getSize(); i++)
		{
			x[i] = IloNumArray(getEnv(), nFreq);
			for (int f = 0; f < x[i].getSize(); f++)
			{
				x[i][f] = getValue(varsX[i][f]);
				if (x[i][f]>0.5) x[i][f] = 1;
				else x[i][f] = 0;
			}
		}

		y = IloNumArray(getEnv());
		getValues(y, varsY);
		for (int f = 0; f < y.getSize(); f++)
		{
			if (y[f]>0.5) y[f] = 1;
			else y[f] = 0;
		}

		// find assigned Links
		std::vector <int> assignedFreq(nLinks);
		int numUnassignedLinks = 0;

		for (int i = 0; i < nLinks; i++)
		{
			assignedFreq[i] = checkIfAssigned(x[i]);
			if (assignedFreq[i]<0)
			{
				numUnassignedLinks++;
			}
		}

		// for all unassigned links
		// set a list of forbidden frequencies
		std::vector<std::set<int> > VorbiddenFreq(nLinks);

		for (unsigned int i = 0; i < info.InterferenceVertices.size() / 2; i++)
		{
			int u = info.InterferenceVertices[i * 2];
			int v = info.InterferenceVertices[i * 2 + 1];

			if (assignedFreq[u]>0)
			{
				VorbiddenFreq[v].insert(assignedFreq[u]);
			}
			if (assignedFreq[v]>0)
			{
				VorbiddenFreq[u].insert(assignedFreq[v]);
			}
		}

		// iterate till all Links are assigned
		while (numUnassignedLinks>0)
		{
			// find first unassigned frequency
			int minNumOfFreq;
			int l;

			for (unsigned int i = 0; i < assignedFreq.size(); i++)
			{
				if (assignedFreq[i]<0)
				{
					minNumOfFreq = nFreq - VorbiddenFreq[i].size();
					l = i;
				}

			}

			// For each link compute the number of frequencies that can be
			// assigned to it
			// and take the link (l) for which this number is minimal

			for (unsigned int i = 0; i < VorbiddenFreq.size(); i++)
			{
				if (assignedFreq[i]<0 && (nFreq - VorbiddenFreq[i].size()< minNumOfFreq) )
				{
					minNumOfFreq = nFreq - VorbiddenFreq[i].size();
					l = i;
				}
			}

			// 2) For each frequency that can be assigned to Link l compute
			// the totall number of possible assignemnts to the remaining links
			// that would remain, if f is assigned to l

			// assign that frequency fBest to l for which this number is maximal

			int minViolations = nFreq;
			int fBest=0;
			for (int f= 0; f<nFreq; f++)
			{
				int countViolations = 0;
				for (int i=0; i<nLinks; i++)
				{
					if ( assignedFreq[i] >=0 &&
							std::abs(f - assignedFreq[i]) < d[l][i] )
					countViolations ++;
				}

				if (countViolations<minViolations)
				{
					minViolations = countViolations;
					fBest = f;
				}
			}

			// add assigned frequence fBest to set of vorbidden frequencies of
			// all links, that interfere with l

			for (int i = 0; i<nLinks; i++)
			{
				if (d[l][i]>0 && d[l][i]<max_dist)
				{
					VorbiddenFreq[i].insert(fBest);
				}
			}

			y[fBest] = 1;
			x[l][fBest] = 1;
			assignedFreq[l]=fBest;
			numUnassignedLinks--;

		}

		IloNumVarArray startVars(getEnv());
		IloNumArray startVals(getEnv());
		for (int i = 0; i < nLinks; ++i)
		{
			for (int j = 0; j < nFreq; ++j)
			{
				startVars.add(varsX[i][j]);
				startVals.add(x[i][j]);
			}
		}

		for (int j = 0; j < nFreq; ++j)
		{
			startVars.add(varsY[j]);
			startVals.add(y[j]);
		}

		setSolution(startVars, startVals);
	}
	catch (...)
	{
		//feasX.end();
		//feasY.end();
		x.end();
		y.end();
		throw;
	}

	//feasX.end();
	//feasY.end();
	x.end();
	y.end();
}

/*
 * Cut Callback
 */

/*
 * Branch Selection Callback
 */
ILOBRANCHCALLBACK1(MyBranch, IloNumVarArray, vars)
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
	}
	catch (...)
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
 */
ILONODECALLBACK0(MySelect)
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
 ILOCPLEXGOAL1(MyBranchGoal, IloNumVarArray, vars)
 {
 IloNumArray x;
 IloNumArray obj;
 IntegerFeasibilityArray feas;

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
 if ( feas[j] == Infeasible )
 {
 IloNum xj_inf = x[j] - IloFloor (x[j]);
 if ( xj_inf > 0.5 )
 xj_inf = 1.0 - xj_inf;
 if ( xj_inf >= maxinf &&
 (xj_inf > maxinf || IloAbs (obj[j]) >= maxobj) )
 {
 bestj = j;
 maxinf = xj_inf;
 maxobj = IloAbs (obj[j]);
 }
 }
 }

 IloCplex::Goal res;
 if ( bestj >= 0 )
 {
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
