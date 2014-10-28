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

std::vector<std::vector<int> > getConflictMatrix(IloNumArray c, int& pivotLine,
		int& globalMin)
{
	int n = c.getSize();

	std::vector<std::vector<int> > conflictMatrix;

	conflictMatrix.resize(n);

	for (IloInt i = 0; i < n; i++)
	{
		int maxNegError = 0;
		int sumNegError = 0;

		conflictMatrix[i].resize(n + 2);

		for (IloInt j = 0; j < n; j++)
		{
			conflictMatrix[i][j] = 0;
			if (IG[i][j] > 0)
			{
				conflictMatrix[i][j] = std::abs(c[i] - c[j]) - IG[i][j];
				if (conflictMatrix[i][j] < maxNegError)
					sumNegError += std::min(0, conflictMatrix[i][j]);
			}
			else
				conflictMatrix[i][j] = INT_MAX;
		}
		int localMin = *std::min_element(conflictMatrix[i].begin(),
				conflictMatrix[i].end());

		conflictMatrix[i][n] = localMin;
		if (localMin < globalMin)
			globalMin = localMin;

		conflictMatrix[i][n + 1] = sumNegError;

		if (i > 0 && sumNegError < conflictMatrix[pivotLine][n + 1])
			pivotLine = i;
	}

	return conflictMatrix;

}

/*
 * Heuristic callback
 */

ILOHEURISTICCALLBACK2(MyHeuristic, IloIntVarArray, varsC, const IloArray<IloNumArray> &, dist)
{
	std::cout << "Primal Heuristik Callback" << std::endl;

	IloNumArray c;
	IntegerFeasibilityArray feasC;

	try
	{

		IloInt n = varsC.getSize();
		feasC = IntegerFeasibilityArray(getEnv());
		c = IloNumArray(getEnv());
		getValues(c, varsC);
		getFeasibilities(feasC, varsC);

		IloNum objval = getObjValue();

		for (IloInt i = 0; i < n; i++)
		{
			// Round the fractional variable to zero
			if ( feasC[i] == Infeasible )
			{
				c[i] = std::floor(c[i]);
			}
		}
/*
		std::cout << "Current Solution: ";
		for (IloInt i=0; i<n; i++)
		{
			std::cout << " " << c[i];
		}
		std::cout << std::endl;
*/
		int globalMin = INT_MIN;
		int pivotLine;


		while(globalMin<0)
		{
			globalMin = 0;
			pivotLine = 0;
			std::vector<std::vector<int> > conflictMatrix = getConflictMatrix(c, pivotLine, globalMin);
/*
			std::cout << "Conflict Matrix:\n";
			for (IloInt i=0; i<n; i++)
			{
				for (IloInt j=0; j<n; j++)
				{
					std::cout << " " << conflictMatrix[i][j];
				}
				std::cout << "| " << conflictMatrix[i][n];
				std::cout << " " << conflictMatrix[i][n+1];
				std::cout << std::endl;
			}
			std::cout << std::endl;
			std::cout << "Pivot Line " << pivotLine << std::endl;
*/

			// Start Heuristik: compensate a Lable of Pivot Node (Element with the Index of Pivot Line)
			int diff = -conflictMatrix[pivotLine][n];
			std::vector<int> diffVector;

			diffVector.push_back(diff);

			for (int j=0; j<n; j++)
			{
				if (conflictMatrix[pivotLine][j]>= 0 && conflictMatrix[pivotLine][j]< diff && pivotLine!=j)
				{
					diffVector.push_back(c[j]+IG[pivotLine][j]-c[pivotLine]);
				}
			}

/*
			std::cout << "diffVector: ";
			for (int j=0; j< diffVector.size(); j++)
			std::cout <<diffVector[j] << " ";
*/
			diff = *std::max_element(diffVector.begin(), diffVector.end());
//			std::cout << "\t c[" << pivotLine+1 << "]+" << diff << std::endl << std::endl;

			c[pivotLine]+=diff;

		}	//while globalMin<0

		std::cout << "Heuristik Solution: ";
		for (IloInt i=0; i<n; i++)
		{
			std::cout << " " << c[i];
		}
		std::cout << std::endl;


		objval = IloMax(c);

		setSolution(varsC, c, objval);

		/*
		 // find assigned Links
		 std::vector <int> assignedFreq(nLinks);

		 MatrixIloNum x;
		 IloNumArray y;
		 const IloIntVar lambda;

		 try
		 {
		 //std::cout << "\nHeurestik solution"<<std::endl;
		 //IloNum lambda = getObjValue();

		 IloInt nLinks = varsX.getSize();
		 IloInt nFreq = varsX[0].getSize();


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
		 */
	}
	catch (...)
	{
		feasC.end();
		c.end();
		throw;
	}

	feasC.end();
	c.end();

}
