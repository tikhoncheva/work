#include "var.h"

/*
 * Node Selection Callback

 */

ILONODECALLBACK1(MyNodeSelect,MatrixInt, vDeg)
{
	IloInt remainingNodes = getNremainingNodes();
	IloInt bestnode = -1;
	IloInt maxdepth = -1;
	IloNum maxiisum = 0.0;
	for (IloInt i = 0; i < remainingNodes; i++)
	{
		IloInt depth = getDepth(i);
		IloNum iisum = getInfeasibilitySum(i);
		if ( (depth >= maxdepth) &&
				(depth > maxdepth || iisum > maxiisum) )
		{
			bestnode = i;
			maxdepth = depth;
			maxiisum = iisum;
		}
	}
	if ( bestnode >= 0 ) selectNode(bestnode);
}

ILONODECALLBACK1(MyNodeSelect1, IloIntVarArray, varsC)
{
	/*
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

		int globalMin = 0;
		int pivotLine = 0;

		std::vector<std::vector<int> > conflictMatrix = getConflictMatrix(c, pivotLine, globalMin);

		int selectNode = -1;
		int glMAX = 0;
		for (int i=0; i<n; i++)
		{
		    auto rowMAX = std::max_element( conflictMatrix[i].begin(), conflictMatrix[i].end() );
		    if (glMAX < *rowMAX)
		    {
		    	glMAX = rowMAX;
		    	selectNode = i;
		    }
		}

		if ( selectNode >= 0 ) selectNode(bestnode);

	}
	catch (...)
	{
		c.end();
		feasC.end();
		throw;
	}
	c.end();
	feasC.end();
*/
}
