//#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include "var.h"
#include "AlgBronKerbosch.cpp"

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

/*
 * Heuristic callback
 */
/*
ILOHEURISTICCALLBACK2(MyHeuristic, MatrixIloIntVar, varsX,
		IloNumVarArray, varsY)
{
	//IloArray<IntegerFeasibilityArray> feasX;
	//IntegerFeasibilityArray feasY;

	MatrixIloNum x;
	IloNumArray y;
	const IloIntVar lambda;

	try
	{
		//std::cout << "\nHeurestik solution"<<std::endl;
		//IloNum lambda = getObjValue();

		IloInt nLinks = varsX.getSize();
		IloInt nFreq = varsX[0].getSize();

		//
		// feasX = IloArray<IntegerFeasibilityArray>(getEnv(), nLinks);
		// for (int i = 0; i < feasX.getSize(); i++)
		// {
		// feasX[i] = IntegerFeasibilityArray(getEnv(), nFreq);
		// getFeasibilities(feasX[i], varsX[i]);
		// }

		// feasY = IntegerFeasibilityArray(getEnv());
		// getFeasibilities(feasY, varsY);
		//

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
			unsigned int minNumOfFreq;
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

		//IloNum newObj = IloMax(y);

		setSolution(startVars, startVals);//, newObj);
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
*/
/*
 * Cut Callback
 */

// add user cuts to the cut table before B&C process
void makeCuts(const IloEnv env, IloRangeArray& cuts,
		const MatrixIloIntVar& varsX, std::vector<std::set<int> > cliqueList)
{

	IloInt nFreq = varsX[0].getSize();

	for (unsigned int i = 0; i < cliqueList.size(); i++)
	{
		IloExpr sum_x_inClique(env);
		for (int f = 0; f < nFreq; f++)
		{
			std::set<int>::iterator itC;
			std::set<int> C = cliqueList[i];

			for (itC = C.begin(); itC != C.end(); itC++)
			{
				int ind = *itC;
				sum_x_inClique += varsX[ind][f];
			}
		}
		cuts.add(sum_x_inClique <= 1);
		sum_x_inClique.end();
	}
}

void makeCuts2(const IloEnv env, IloExprArray lhs, const MatrixIloIntVar& varsX,
		std::vector<std::set<int> > cliqueList)
{
	IloInt nFreq = varsX[0].getSize();

	for (unsigned int i = 0; i < cliqueList.size(); i++)
	{
		IloExpr sum_x_inClique(env);

		for (int f = 0; f < nFreq; f++)
		{

			std::set<int>::iterator itC;
			std::set<int> C = cliqueList[i];

			for (itC = C.begin(); itC != C.end(); itC++)
			{
				int ind = *itC;
				sum_x_inClique += varsX[ind][f];
			}
		}
		lhs.add(sum_x_inClique);
		//sum_x_inClique.end();
	}
}

void makeCuts3(const IloEnv env, IloRangeArray& cuts,
		const MatrixIloIntVar& varsX, std::vector<std::set<int> > cliqueList)
{
	IloInt nFreq = varsX[0].getSize();

	for (unsigned int i = 0; i < cliqueList.size(); i++)
	{
		IloExpr sum_x_inClique(env);
		std::set<int>::iterator itC;
		std::set<int> C = cliqueList[i];

		for (itC = C.begin(); itC != C.end(); itC++)
		{
			int ind = *itC;

			//double c_i = 0.;
			for (int f = 0; f < nFreq; f++)
			{

				sum_x_inClique += varsX[ind][f] * f;

			}
		}

		cuts.add(sum_x_inClique <= 1);
		sum_x_inClique.end();
	}

}

/*
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
					if (d[i][j]==max_dist)
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
*/
/*
 * Branch Selection Callback
 */
ILOBRANCHCALLBACK2(MyBranch, MatrixIloIntVar, varsX,
		IloNumVarArray, varsY)
{
//IloArray<IntegerFeasibilityArray> feasX;
//IntegerFeasibilityArray feasY;

	MatrixIloNum x;
	IloNumArray y;

	try
	{
//IloInt nLinks = varsX.getSize();
		int nLinks = varsX.getSize();
//IloInt nFreq = varsX[0].getSize();
		int nFreq = varsX[0].getSize();

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

// get current relaxation of the problem
		x = MatrixIloNum(getEnv(), nLinks);
		for (int i = 0; i < x.getSize(); i++)
		{
			x[i] = IloNumArray(getEnv(), nFreq);
			for (int f = 0; f < x[i].getSize(); f++)
			{
				x[i][f] = getValue(varsX[i][f]);
				if (x[i][f]<0.000001) x[i][f] = 0.;
			}
		}

		y = IloNumArray(getEnv());
		getValues(y, varsY);
		for (int f = 0; f < y.getSize(); f++)
		{
			if (y[f]<0.000001) y[f]=0.0;
		}

// find variable y with value closest to 1

		double maxFractY = 0.;
		int indMaxFractY = -1;

		for (int f = 0; f<nFreq; f++)
		{
			//std::cout << y[f];
			if (y[f]>maxFractY && (y[f]-std::floor(y[f])>0) )
			{
				maxFractY = y[f];
				indMaxFractY = f;
			}
		}

		if (indMaxFractY<0) // all variables y are integer
		{
			// find x[i][f] that will force the mist other variables to zero
			int ii = -1;
			int ff = -1;
			int maxVorbiddenFreq = 0;

			for (int i=0; i<nLinks; i++)
			{
				for (int f=0; f<nFreq; f++)
				{
					// calculate number of frequencies, that will be vorbidden
					// for all other x-s if x[i][f] = 1;
					int sumVorbidden = 0;
					for (unsigned int j = 0; j < info.InterferenceVertices.size() / 2; j++)
					{
						unsigned int u = info.InterferenceVertices[j * 2];
						unsigned int v = info.InterferenceVertices[j * 2 + 1];

						if (j == u || j == v )
						{
							for (int g = std::max(f-d[u][v],0);
									g <= std::min(f+d[u][v], nFreq );
									g++)
							{ // x[u][f] + x[v][g] <= 1
								sumVorbidden ++;
							}
						}

					}

					if (sumVorbidden > maxVorbiddenFreq && (x[i][f]<1 && x[i][f]>0 ))
					{
						ii = i;
						ff = f;
						maxVorbiddenFreq = sumVorbidden;
					}

				}
			}

			// branch on the x[ii][ff]
			if (ii>=0 && ff>=0)
			{
				//std::cout << "\nbranch on x[" << ii << "][" << ff << "]  " << x[ii][ff]<< std::endl;
				makeBranch(varsX[ii][ff], 0, IloCplex::BranchDown, getObjValue());
				makeBranch(varsX[ii][ff], 1, IloCplex::BranchUp, getObjValue());
			}
		}

		else
		{ // branch on the found vertex y[indMaxFractY]

			//std::cout << "\nbranch on y[" << indMaxFractY << "]  " << y[indMaxFractY] << std::endl;

			makeBranch(varsY[indMaxFractY], 0, IloCplex::BranchDown, getObjValue());
			makeBranch(varsY[indMaxFractY], 1, IloCplex::BranchUp, getObjValue());
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

/*
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

 }
 */

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

