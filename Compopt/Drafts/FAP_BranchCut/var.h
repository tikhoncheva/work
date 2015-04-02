#ifndef VAR_H_
#define VAR_H_

#include <vector>
#include <set>
 

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> MatrixInt;


typedef std::vector<double> VectorDouble;
typedef std::vector<VectorDouble> MatrixDouble;


typedef IloArray<IloIntVarArray> MatrixIloIntVar;
typedef IloArray<IloNumArray> MatrixIloNum;

int max_dist;
std::vector<std::vector<int> > d;

struct SInfo
{
	int numConstr;
	int numVar;
	int numRedundantVar;

	VectorInt InterferenceVertices; // Indices of the vertices, that interferent to eachother
};

SInfo info;


#endif /* VAR_H_ */
