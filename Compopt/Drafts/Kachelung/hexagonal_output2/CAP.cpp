#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <ilcplex/ilocplex.h>	/* CPLEX */

ILOSTLBEGIN

typedef std::vector<int> VectorInt;
typedef std::vector<VectorInt> MatrixInt;

typedef std::vector<double> VectorDouble;
typedef std::vector<VectorDouble> MatrixDouble;

struct SSolutionInfo
{
	double time;

	int numAddVar;
	VectorInt ConstrIndex;
	int numConstr;
	int numVar;

};

MatrixDouble hexagonalLattice(int n, int n1)
{
	MatrixDouble coord;
	int i = 0, l = 0, j = 0;
	int n_pro_level = n1;
	int sum = n1;
	int dx = 0;
	double y = 0;

	coord.resize(n);
	while (i < n)
	{
		if (i == (sum))
		{ // Uebergang zu dem naechsten Level
			l++;
			if (sum <= n / 2)
			{
				dx += l % 2;
				n_pro_level = n_pro_level + l % 2;
			}
			else
			{
				dx -= l % 2;
				n_pro_level = n_pro_level - l % 2;
			}
			sum += n_pro_level;
			y = y + (1 + (l + 1) % 2) * 1 / 2.;
		}

		coord[i].resize(2);
		coord[i][0] = -dx * sqrt(3) / 2. + j * sqrt(3);
		coord[i][1] = y;

		j = (j + 1) % n_pro_level;
		i++;
	}

	return coord;
}
MatrixDouble triangularLattice(int n, int n1)
{
	MatrixDouble coord;
	int i = 0, l = 0, j = 0;
	int n_pro_level = n1;
	int sum = n1;
	double y = 0, dx = 0;

	coord.resize(n);

	std::cout << n_pro_level << n1 << "\n";

	while (i < n)
	{
		if (i == (sum))
		{ // Uebergang zu dem naechsten Level
			l++;
			dx = 0.5 * (l % 2);
			n_pro_level = n_pro_level + (l + 1) % 2 - l % 2;
			sum += n_pro_level;
			y = y + sqrt(3) / 2.;
		}

		coord[i].resize(2);
		coord[i][0] = dx + j;
		coord[i][1] = y;

		j = (j + 1) % n_pro_level;
		i++;
	}

	return coord;
}

MatrixDouble squareLattice(int n)
{
	MatrixDouble coord;

	coord.resize(n);

	for (int i = 0; i < sqrt(n); i++)
	{
		for (int j = 0; j < sqrt(n); j++)
		{
			coord[i * sqrt(n) + j].resize(2);

			coord[i * sqrt(n) + j][0] = j;
			coord[i * sqrt(n) + j][1] = i;
		}
	}

	return coord;
}

class CProblem
{
private:
	std::string fileName;
	int
	setProblem(); // read data from file
	void
	coordTOdist();
public:
	int lattice;
	MatrixDouble distances;
	MatrixDouble coordinates;
	MatrixDouble d; // use simply line-function
	VectorDouble C; // labeling
	VectorInt Z; // additional variables
	unsigned int n; // number of vertices
	unsigned int diameter; // diameter
	double lambda_G_d; // lambda(G, d_1, d_2, .., d_s)
	double UB; // lambda <= UB

	double max_distance;
	SSolutionInfo Info;

	void
	print();
	void
	set_d_function();
	void
	set_d();
	int
	setModel();
	int
	save_result(std::string fileName);

	CProblem(int _max_d, int _lattice, std::string _fileName)
	{
		lattice = _lattice;

		fileName = _fileName;
		Info.numAddVar = 0;
		Info.numConstr = 0;
		Info.numVar = 1 + n; // lambda + all c;

		max_distance = _max_d;

		setProblem();

	}
	;

	double dist_linear(double x)
	{
		double value = 1 + max_distance - x;
		return (value < 0) ? 0 : value; /* linear function of distances*/
		//return value;
	}
};
void CProblem::coordTOdist()
{
	distances.resize(n);
	//max_distance = 0.0;
	for (unsigned int u = 0; u < n; u++)
	{
		distances[u].resize(n);
		for (unsigned int v = 0; v < u; v++)
		{
			distances[u][v] = (sqrt(
					(coordinates[u][0] - coordinates[v][0])
							* (coordinates[u][0] - coordinates[v][0])
							+ (coordinates[u][1] - coordinates[v][1])
									* (coordinates[u][1] - coordinates[v][1])));
			distances[v][u] = distances[u][v];
			//if (distances[u][v] > max_distance) max_distance = distances[u][v];
		}

		distances[0][0] = 0;
	}
}
void CProblem::set_d_function()
{
	d.resize(n);
	for (unsigned int u = 0; u < n; u++)
	{
		d[u].resize(n);
		for (unsigned int v = 0; v < u; v++)
		{
			d[u][v] = dist_linear(distances[u][v]);
			d[v][u] = d[u][v];
			if (d[u][v] <= max_distance && u != v)
			{ /* save vertices for which additional constraints to be created*/
				Info.numAddVar++;
				Info.ConstrIndex.push_back(u);
				Info.ConstrIndex.push_back(v);
			}
		}
	}
	Info.numVar += Info.numAddVar;
	std::cout << " numAddVar " << Info.numAddVar << "\n";
}

int CProblem::setProblem()
{
	std::string line;
	std::fstream oFile;
	std::stringstream buf;
	//int bufInt;

	switch (lattice)
	{
	case 0:
		double bufDouble;
		oFile.open(fileName.c_str());
		if (!oFile.is_open())
		{
			std::cerr << "Cannot open file!\n";
			exit(1);
		}

		std::getline(oFile, line); /* number of vertices and diameter */
		std::getline(oFile, line);
		buf << line;
		buf >> n;
		buf >> diameter;
		buf.clear();

		std::getline(oFile, line); /* coordinates */
		coordinates.resize(n);
		for (unsigned int i = 0; i < n; i++)
		{
			coordinates[i].resize(2);
			std::getline(oFile, line);
			buf << line;
			buf >> bufDouble; /* because first number in line is the number of the vertex */
			buf >> bufDouble;
			coordinates[i][0] = bufDouble;
			buf >> bufDouble;
			coordinates[i][1] = bufDouble;
			buf.clear();
		}
		oFile.close();
		break;

	case 1:
		n = 24;
		Info.numVar = 1 + n; // lambda + all c;
		coordinates = hexagonalLattice(n, 2); // number of vertices, number of vertices in first line
		diameter = 7;
		fileName = "hexagonal_lattice.dat";
		break;

	case 2:
		n = 23;
		Info.numVar = 1 + n; // lambda + all c;
		coordinates = triangularLattice(n, 5); // number of vertices, number of vertices in first line
		diameter = 6;
		fileName = "triangular_lattice.dat";
		break;

	case 3:
		n = 25;
		diameter = 8;
		//n = 4;
		//diameter = 2;
		Info.numVar = 1 + n; // lambda + all c;
		coordinates = squareLattice(n);
		fileName = "square_lattice.dat";
		break;

	default:
		std::cerr << "Wrong lattice type\n";
		exit(-1);
	} //switch

	coordTOdist();

	set_d_function();

	return 0;
}

void CProblem::print()
{
	std::cout << "\n";
	std::cout << "Graph (V,E) with |V|= " << n << " and diameter = " << diameter
			<< "\n";
	std::cout << "\n";

	std::cout << "Coordinates: " << "\n";
	for (unsigned int u = 0; u < n; u++)
	{
		std::cout << "vertex " << u + 1 << " : " << coordinates[u][0] << " "
				<< coordinates[u][1] << "\n";
	}
	std::cout << "\n";

	std::cout << "Distances: " << "\n";
	for (unsigned int u = 0; u < n; u++)
	{
		for (unsigned int v = 0; v < n; v++)
		{
			std::cout << std::setw(3) << distances[u][v] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "d: " << "\n";
	for (unsigned int u = 0; u < d.size(); u++)
	{
		for (unsigned int v = 0; v < d[u].size(); v++)
		{
			std::cout << std::setw(3) << d[u][v] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "max distance : " << max_distance;
	std::cout << "\n";
}

int CProblem::setModel()
{
	//time_t start, end;

	IloEnv env;
	try
	{
		IloModel model(env);
		IloCplex cplex(env);

		/*Variables*/
		IloNumVar lambda(env, "lambda");
		IloNumVarArray c(env, n); //
		for (unsigned int u = 0; u < n; u++)
		{
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u] = IloNumVar(env, str.c_str());
		}

		IloIntVarArray z0(env, Info.numAddVar);
		for (int i = 0; i < Info.numAddVar; i++)
		{
			std::stringstream ss;
			ss << i;
			std::string str = "z0_" + ss.str();
			z0[i] = IloIntVar(env, 0, 1, str.c_str());
		}

		/*  Objective*/
		model.add(IloMinimize(env, lambda));
		/*Constrains*/
		/* d=function of the distance */
		IloArray<IloNumArray> Par_d(env, n);
		for (unsigned int u = 0; u < n; u++)
		{
			Par_d[u] = IloNumArray(env, n);
			for (unsigned int v = 0; v < n; v++)
			{
				Par_d[u][v] = d[u][v];
			}
		}

		int M = (max_distance + 1) * n;
		for (int i = 0; i < Info.numAddVar; i++)
		{
			int u = Info.ConstrIndex[i * 2];
			int v = Info.ConstrIndex[i * 2 + 1];

			model.add(c[u] - c[v] + M * (z0[i]) >= Par_d[u][v]);
			model.add(c[v] - c[u] + M * (1 - z0[i]) >= Par_d[u][v]);

		}

		
		// d(x) = 3 - x
		if (max_distance == 2) 
		{
		  // 2 Trapeze, die ein mit der Seitenlaenge 1 bilden
		  model.add(c[0]+c[2] +c[5] +c[9] >=7.0718);
		  model.add(c[0]+c[3] +c[6] +c[9] >=7.0718);
		  
		  model.add(c[1]+c[3] +c[6] +c[10] >=7.0718);
		  model.add(c[1]+c[4] +c[7] +c[10] >=7.0718);
		  
		  model.add(c[5]+c[8] +c[12] +c[16] >=7.0718);
		  model.add(c[5]+c[9] +c[13] +c[16] >=7.0718);
		  
		  model.add(c[6]+c[9] +c[13] +c[17] >=7.0718);
		  model.add(c[6]+c[10] +c[14] +c[17] >=7.0718);
		  
		  model.add(c[7]+c[10] +c[14] +c[18] >=7.0718);
		  model.add(c[7]+c[11] +c[15] +c[18] >=7.0718);
		  
		  model.add(c[13]+c[16] +c[19] +c[22] >=7.0718);
		  model.add(c[13]+c[17] +c[20] +c[22] >=7.0718);
		  
		  model.add(c[14]+c[17] +c[20] +c[23] >=7.0718);
		  model.add(c[14]+c[18] +c[21] +c[23] >=7.0718);
		  //
		}
		
		//d(x) = 4 - x	
		if (max_distance == 3) 
		{
		  // 2 Trapeze, die ein mit der Seitenlaenge 1 bilden
		  model.add(c[0]+c[2] +c[5] +c[9] >=13.0718);
		  model.add(c[0]+c[3] +c[6] +c[9] >=13.0718);
		  
		  model.add(c[1]+c[3] +c[6] +c[10] >=13.0718);
		  model.add(c[1]+c[4] +c[7] +c[10] >=13.0718);
		  
		  model.add(c[5]+c[8] +c[12] +c[16] >=13.0718);
		  model.add(c[5]+c[9] +c[13] +c[16] >=13.0718);
		  
		  model.add(c[6]+c[9] +c[13] +c[17] >=13.0718);
		  model.add(c[6]+c[10] +c[14] +c[17] >=13.0718);
		  
		  model.add(c[7]+c[10] +c[14] +c[18] >=13.0718);
		  model.add(c[7]+c[11] +c[15] +c[18] >=13.0718);
		  
		  model.add(c[13]+c[16] +c[19] +c[22] >=13.0718);
		  model.add(c[13]+c[17] +c[20] +c[22] >=13.0718);
		  
		  model.add(c[14]+c[17] +c[20] +c[23] >=13.0718);
		  model.add(c[14]+c[18] +c[21] +c[23] >=13.0718);
		  //
		}
		
		

		for (unsigned int v = 0; v < n; v++)
		{
			model.add(c[v] <= lambda);
			model.add(c[v] >= 0);
		}

		std::cout << "Number of variables " << Info.numVar << "\n";

		/* solve the Model*/
		cplex.extract(model);
		cplex.exportModel("L-Labeling.lp");

		IloTimer timer(env);
		timer.start();
		int solveError = cplex.solve();
		timer.stop();

		if (!solveError)
		{
			std::cout << "STATUS : " << cplex.getStatus() << "\n";
			env.error() << "Failed to optimize LP \n";
			exit(1);
		}
		//Info.time = (double)(end-start)/(double)CLOCKS_PER_SEC;
		Info.time = timer.getTime();

		std::cout << "STATUS : " << cplex.getStatus() << "\n";
		/* get the solution*/
		env.out() << "Solution status = " << cplex.getStatus() << "\n";
		Info.numConstr = cplex.getNrows();
		env.out() << " Number of constraints = " << Info.numConstr << "\n";
		lambda_G_d = cplex.getObjValue();
		env.out() << "Solution value  = " << lambda_G_d << "\n";
		for (unsigned int u = 0; u < n; u++)
		{
			C.push_back(cplex.getValue(c[u]));
			std::cout << "c(" << u << ")=" << C[u] << " ";
		}

	} // end try
	catch (IloException& e)
	{
		std::cerr << "Concert exception caught: " << e << std::endl;
	} catch (...)
	{
		std::cerr << "Unknown exception caught" << std::endl;
	}
	env.end();
	return 0;
}

int CProblem::save_result(std::string _fileName)
{
	std::ofstream outFile;

	std::string outFileNameSolve = "results/resultsSolve_";
	std::string outFileName;

	std::stringstream ss;
	ss << max_distance + 1;
	std::string str = "d" + ss.str();
	outFileNameSolve += str;

	outFileNameSolve += "_function_";

	outFileName = outFileNameSolve;

	unsigned found = fileName.find_last_of("/");
	std::string line = fileName.substr(found + 1,
			fileName.length() - 1 - found);

	outFileName = outFileName + line;

	outFile.open(outFileName.c_str());
	if (!outFile.is_open())
	{
		std::cerr << "Cannot open file to save a result!\n";
		exit(1);
	}

	outFile << "Name of file:\t" << _fileName << "\n";
	outFile << "Optimal value:\t" << lambda_G_d << "\n";
	outFile << "Number of constraints: " << Info.numConstr << "\n";
	outFile << "Number of variables: " << Info.numVar << "\n";
	outFile << "Used time: " << Info.time << "sec\n";
	outFile << "Labeling:\n";
	for (unsigned int u = 0; u < C.size(); u++)
		outFile << "c[" << u + 1 << "]=" << C[u] << " ";

	outFile.close();
	return 0;
}

int main(int argc, char **argv)
{
	std::string fileName = "dummy.txt";

	if (argc > 4 && argc < 2)
	{
		std::cerr << "LLabeling <d max> <lattice> <file name to open> \n";
		std::cerr
				<< "lattice: 0 read from file; 1 hexagonal lattice; 2 triangular lattice; 3 square lattice";
		return 1;
	}

	if (atoi(argv[2]) == 0)
	{
		fileName = argv[3];
		std::cout << "Start with the file " << fileName << "\n";
	}

	CProblem newProblem(atoi(argv[1]), atoi(argv[2]), fileName);

	newProblem.print();

	newProblem.setModel();

	newProblem.save_result(fileName);

	return 0;
} // END main

