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
