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

struct SSolutionInfo {
	double time;
	int nodeleftcnt;

};

MatrixDouble hexagonalLattice(int n, int n1) {
	MatrixDouble coord;
	int i = 0, l = 0, j = 0;
	int n_pro_level = n1;
	int sum = n1;
	int dx = 0;
	double y = 0;

	coord.resize(n);
	while (i < n) {
		if (i == (sum)) { // Uebergang zu dem naechsten Level
			l++;
			if (sum <= n / 2) {
				dx += l % 2;
				n_pro_level = n_pro_level + l % 2;
			} else {
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
MatrixDouble hexagonalLattice2(int n, int n1) {
	MatrixDouble coord;

	coord.resize(n);

	coord[0].resize(2);
	coord[0][0] = 0;
	coord[0][1] = 0;
	coord[1].resize(2);
	coord[1][0] = sqrt(3);
	coord[1][1] = 0;

	coord[2].resize(2);
	coord[2][0] = -sqrt(3) / 2.;
	coord[2][1] = 0.5;
	coord[3].resize(2);
	coord[3][0] = sqrt(3) / 2.;
	coord[3][1] = 0.5;
	coord[4].resize(2);
	coord[4][0] = 3 * sqrt(3) / 2.;
	coord[4][1] = 0.5;

	coord[5].resize(2);
	coord[5][0] = -sqrt(3) / 2.;
	coord[5][1] = 1.5;
	coord[6].resize(2);
	coord[6][0] = sqrt(3) / 2.;
	coord[6][1] = 1.5;
	coord[7].resize(2);
	coord[7][0] = 3 * sqrt(3) / 2.;
	coord[7][1] = 1.5;

	coord[8].resize(2);
	coord[8][0] = -sqrt(3);
	coord[8][1] = 2;
	coord[9].resize(2);
	coord[9][0] = 0;
	coord[9][1] = 2;
	coord[10].resize(2);
	coord[10][0] = sqrt(3);
	coord[10][1] = 2;
	coord[11].resize(2);
	coord[11][0] = 2 * sqrt(3);
	coord[11][1] = 2;

	coord[12].resize(2);
	coord[12][0] = -sqrt(3);
	coord[12][1] = 3;
	coord[13].resize(2);
	coord[13][0] = 0;
	coord[13][1] = 3;
	coord[14].resize(2);
	coord[14][0] = sqrt(3);
	coord[14][1] = 3;
	coord[15].resize(2);
	coord[15][0] = 2 * sqrt(3);
	coord[15][1] = 3;

	coord[16].resize(2);
	coord[16][0] = -sqrt(3) / 2.;
	coord[16][1] = 3.5;
	coord[17].resize(2);
	coord[17][0] = sqrt(3) / 2.;
	coord[17][1] = 3.5;
	coord[18].resize(2);
	coord[18][0] = 3 * sqrt(3) / 2.;
	coord[18][1] = 3.5;

	coord[19].resize(2);
	coord[19][0] = -sqrt(3) / 2.;
	coord[19][1] = 4.5;
	coord[20].resize(2);
	coord[20][0] = sqrt(3) / 2.;
	coord[20][1] = 4.5;
	coord[21].resize(2);
	coord[21][0] = 3 * sqrt(3) / 2.;
	coord[21][1] = 4.5;

	coord[22].resize(2);
	coord[22][0] = 0;
	coord[22][1] = 5;
	coord[23].resize(2);
	coord[23][0] = sqrt(3);
	coord[23][1] = 5;

	return coord;
}

MatrixDouble triangularLattice(int n, int n1) {
	MatrixDouble coord;
	int i = 0, l = 0, j = 0;
	int n_pro_level = n1;
	int sum = n1;
	double y = 0, dx = 0;

	coord.resize(n);

	std::cout << n_pro_level << n1 << "\n";

	while (i < n) {
		if (i == (sum)) { // Uebergang zu dem naechsten Level
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

MatrixDouble triangularLattice2(int n, int n1) {
	MatrixDouble coord;

	coord.resize(n);

	coord[0].resize(2);
	coord[0][0] = 0;
	coord[0][1] = 0;
	coord[1].resize(2);
	coord[1][0] = 1;
	coord[1][1] = 0;
	coord[2].resize(2);
	coord[2][0] = 2;
	coord[2][1] = 0;
	coord[3].resize(2);
	coord[3][0] = 3;
	coord[3][1] = 0;
	coord[4].resize(2);
	coord[4][0] = 4;
	coord[4][1] = 0;

	coord[5].resize(2);
	coord[5][0] = 0.5;
	coord[5][1] = sqrt(3) / 2.;
	coord[6].resize(2);
	coord[6][0] = 1.5;
	coord[6][1] = sqrt(3) / 2.;
	coord[7].resize(2);
	coord[7][0] = 2.5;
	coord[7][1] = sqrt(3) / 2.;
	coord[8].resize(2);
	coord[8][0] = 3.5;
	coord[8][1] = sqrt(3) / 2.;

	coord[9].resize(2);
	coord[9][0] = 0;
	coord[9][1] = sqrt(3);
	coord[10].resize(2);
	coord[10][0] = 1;
	coord[10][1] = sqrt(3);
	coord[11].resize(2);
	coord[11][0] = 2;
	coord[11][1] = sqrt(3);
	coord[12].resize(2);
	coord[12][0] = 3;
	coord[12][1] = sqrt(3);
	coord[13].resize(2);
	coord[13][0] = 4;
	coord[13][1] = sqrt(3);

	coord[14].resize(2);
	coord[14][0] = 0.5;
	coord[14][1] = 3 * sqrt(3) / 2.;
	coord[15].resize(2);
	coord[15][0] = 1.5;
	coord[15][1] = 3 * sqrt(3) / 2.;
	coord[16].resize(2);
	coord[16][0] = 2.5;
	coord[16][1] = 3 * sqrt(3) / 2.;
	coord[17].resize(2);
	coord[17][0] = 3.5;
	coord[17][1] = 3 * sqrt(3) / 2.;

	coord[18].resize(2);
	coord[18][0] = 0;
	coord[18][1] = 2 * sqrt(3);
	coord[19].resize(2);
	coord[19][0] = 1;
	coord[19][1] = 2 * sqrt(3);
	coord[20].resize(2);
	coord[20][0] = 2;
	coord[20][1] = 2 * sqrt(3);
	coord[21].resize(2);
	coord[21][0] = 3;
	coord[21][1] = 2 * sqrt(3);
	coord[22].resize(2);
	coord[22][0] = 4;
	coord[22][1] = 2 * sqrt(3);

	return coord;
}

MatrixDouble squareLattice(int n) {
	MatrixDouble coord;

	coord.resize(n);

	for (int i = 0; i < sqrt(n); i++) {
		for (int j = 0; j < sqrt(n); j++) {
			coord[i * sqrt(n) + j].resize(2);

			coord[i * sqrt(n) + j][0] = j;
			coord[i * sqrt(n) + j][1] = i;
		}
	}

	return coord;
}

MatrixDouble squareLattice2(int n) {
	MatrixDouble coord;

	coord.resize(n);

	coord[0].resize(2);
	coord[0][0] = 0;
	coord[0][1] = 0;
	coord[1].resize(2);
	coord[1][0] = 1;
	coord[1][1] = 0;
	coord[2].resize(2);
	coord[2][0] = 2;
	coord[2][1] = 0;
	coord[3].resize(2);
	coord[3][0] = 3;
	coord[3][1] = 0;
	coord[4].resize(2);
	coord[4][0] = 4;
	coord[4][1] = 0;

	coord[5].resize(2);
	coord[5][0] = 0;
	coord[5][1] = 1;
	coord[6].resize(2);
	coord[6][0] = 1;
	coord[6][1] = 1;
	coord[7].resize(2);
	coord[7][0] = 2;
	coord[7][1] = 1;
	coord[8].resize(2);
	coord[8][0] = 3;
	coord[8][1] = 1;
	coord[9].resize(2);
	coord[9][0] = 4;
	coord[9][1] = 1;

	coord[10].resize(2);
	coord[10][0] = 0;
	coord[10][1] = 2;
	coord[11].resize(2);
	coord[11][0] = 1;
	coord[11][1] = 2;
	coord[12].resize(2);
	coord[12][0] = 2;
	coord[12][1] = 2;
	coord[13].resize(2);
	coord[13][0] = 3;
	coord[13][1] = 2;
	coord[14].resize(2);
	coord[14][0] = 4;
	coord[14][1] = 2;

	coord[15].resize(2);
	coord[15][0] = 0;
	coord[15][1] = 3;
	coord[16].resize(2);
	coord[16][0] = 1;
	coord[16][1] = 3;
	coord[17].resize(2);
	coord[17][0] = 2;
	coord[17][1] = 3;
	coord[18].resize(2);
	coord[18][0] = 3;
	coord[18][1] = 3;
	coord[19].resize(2);
	coord[19][0] = 4;
	coord[19][1] = 3;

	coord[20].resize(2);
	coord[20][0] = 0;
	coord[20][1] = 4;
	coord[21].resize(2);
	coord[21][0] = 1;
	coord[21][1] = 4;
	coord[22].resize(2);
	coord[22][0] = 2;
	coord[22][1] = 4;
	coord[23].resize(2);
	coord[23][0] = 3;
	coord[23][1] = 4;
	coord[24].resize(2);
	coord[24][0] = 4;
	coord[24][1] = 4;

	return coord;
}

class CProblem {
private:
	std::string fileName;
	int setProblem(); // read data from file
	void coordTOdist();
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
	double max_d;
	SSolutionInfo Info;

	int numconst;
	int numvar;

	void print();
	void set_d_function();
	void set_d();
	int setModel();
	int save_result(std::string fileName);

	CProblem(int _max_d, int _lattice, std::string _fileName) {
		lattice = _lattice;

		fileName = _fileName;
		numconst = 0;
		numvar = 0;

		max_d = _max_d;
		max_distance = max_d;

		setProblem();

	}
	;

	double dist_linear(double x) {
		double value = 1 + max_distance - x;
		return (value < 0) ? 0 : value; /* linear function of distances*/
		//return value;
	}
};
void CProblem::coordTOdist() {
	distances.resize(n);
	//max_distance = 0.0;
	for (unsigned int u = 0; u < n; u++) {
		distances[u].resize(n);
		for (unsigned int v = 0; v < u; v++) {
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
void CProblem::set_d_function() {
	max_d = dist_linear(0);
	d.resize(n);
	for (unsigned int u = 0; u < n; u++) {
		d[u].resize(n);
		for (unsigned int v = 0; v < n; v++) {
			d[u][v] = dist_linear(distances[u][v]);
		}
	}
}

int CProblem::setProblem() {
	std::string line;
	std::fstream oFile;
	std::stringstream buf;
	//int bufInt;

	switch (lattice) {
	case 0:
		double bufDouble;
		oFile.open(fileName.c_str());
		if (!oFile.is_open()) {
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
		for (unsigned int i = 0; i < n; i++) {
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
		coordinates = hexagonalLattice(n, 1); // number of vertices, number of vertices in first line
		diameter = 7;
		fileName = "hexagonal_lattice.dat";
		break;

	case 2:
		n = 23;
		coordinates = triangularLattice(n, 5); // number of vertices, number of vertices in first line
		diameter = 6;
		fileName = "triangular_lattice.dat";
		break;

	case 3:
		n = 25;
		//n = 4;
		//diameter = 2;
		coordinates = squareLattice(n);
		diameter = 8;
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

void CProblem::print() {
	std::cout << "\n";
	std::cout << "Graph (V,E) with |V|= " << n << " and diameter = " << diameter
			<< "\n";
	std::cout << "\n";

	std::cout << "Coordinates: " << "\n";
	for (unsigned int u = 0; u < n; u++) {
		std::cout << "vertex " << u + 1 << " : " << coordinates[u][0] << " "
				<< coordinates[u][1] << "\n";
	}
	std::cout << "\n";

	std::cout << "Distances: " << "\n";
	for (unsigned int u = 0; u < n; u++) {
		for (unsigned int v = 0; v < n; v++) {
			std::cout << std::setw(3) << distances[u][v] << " ";
		}
		std::cout << "\n";
	}

	std::cout << "d: " << "\n";
	for (unsigned int u = 0; u < d.size(); u++) {
		for (unsigned int v = 0; v < d[u].size(); v++) {
			std::cout << std::setw(3) << d[u][v] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "max distance : " << max_distance;
	std::cout << "\n";
}

int CProblem::setModel() {
	//time_t start, end;

	numvar = 1 + n; // lambda + all c;

	IloEnv env;
	try {
		IloModel model(env);
		IloCplex cplex(env);

		/*Variables*/
		IloNumVar lambda(env, "lambda");
		IloNumVarArray c(env, n); //
		for (unsigned int u = 0; u < n; u++) {
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u] = IloNumVar(env, str.c_str());
		}

		IloArray<IloIntVarArray> z(env, n);
		for (unsigned int u = 0; u < n; u++) {
			z[u] = IloIntVarArray(env, n);
			for (unsigned int v = 0; v < n; v++) {
				std::stringstream ss;
				ss << u;
				ss << v;
				std::string str = "z" + ss.str();
				z[u][v] = IloIntVar(env, 0, 1, str.c_str());
			}
		}

		/* Constant M*/

		int M = n * max_d;
		UB = M;

		/*  Objective*/
		model.add(IloMinimize(env, lambda));
		//model.add(IloMinimize(env, IloSum(c)));
		/*Constrains*/
		model.add(lambda - UB <= 0);

		/* d=function of the distance */
		IloArray<IloNumArray> Par_d(env, n);
		for (unsigned int u = 0; u < n; u++) {
			Par_d[u] = IloNumArray(env, n);
			for (unsigned int v = 0; v < n; v++) {
				Par_d[u][v] = d[u][v];
			}
		}

		for (unsigned u = 0; u < n; u++) {
			for (unsigned v = 0; v < u; v++) {
				model.add(c[v] - c[u] + M * z[u][v] >= Par_d[u][v]);
				model.add(c[u] - c[v] + M * (1 - z[u][v]) >= Par_d[u][v]);
				model.add(z[u][v] = z[v][u]);
				numvar++; // + z[u][v]
			}
		}

		/*
		 for (unsigned i=0; i<sqrt(n)-1; i=i+1) {
		 for (unsigned j=0; j<sqrt(n)-1; j=j+1) {
		 //square lattice
		 model.add (c[i*sqrt(n)+j]     +c[i*sqrt(n)+j+1] +
		 c[(i+1)*sqrt(n)+j] +c[(i+1)*sqrt(n)+j+1]>= 16-4*sqrt(2));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 3-x
		 //
		 // triangular lattice
		 //					model.add (c[i*5+j]+c[i*5+j+1] +
		 //					           c[(i+1)+j] >= 4);  // Bedingung fuer Quadratischen Gridgraph und d(x) = 3-x
		 //					model.add (c[i*sqrt(n)+j]+c[i*sqrt(n)+j+1] +
		 //					           c[(i+1)*sqrt(n)+j]+c[(i+1)*sqrt(n)+j+1] >= 22 - 4*sqrt(2));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 4-x

		 }
		 }
		 */

		/*
		 for (unsigned i=0; i<sqrt(n)-2; i+=3) {
		 for (unsigned j=0; j<sqrt(n)-2; j+=3) {
		 //					model.add (c[i*sqrt(n)+j]    + c[i*sqrt(n)+j+1]    + c[i*sqrt(n)+j+2] +
		 //					           c[(i+1)*sqrt(n)+j]+ c[(i+1)*sqrt(n)+j+1]+ c[(i+1)*sqrt(n)+j+2] +
		 //					           c[(i+2)*sqrt(n)+j]+ c[(i+2)*sqrt(n)+j+1]+ c[(i+2)*sqrt(n)+j+2]
		 //					           >= 60-17*sqrt(2)-3*sqrt(5));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 3-x
		 //					model.add (c[i*sqrt(n)+j]    + c[i*sqrt(n)+j+1]    + c[i*sqrt(n)+j+2] +
		 //					           c[(i+1)*sqrt(n)+j]+ c[(i+1)*sqrt(n)+j+1]+ c[(i+1)*sqrt(n)+j+2] +
		 //					           c[(i+2)*sqrt(n)+j]+ c[(i+2)*sqrt(n)+j+1]+ c[(i+2)*sqrt(n)+j+2]
		 //					           >= 82-8*sqrt(2)-2*sqrt(5));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 4-x
		 }
		 }

		 */
		for (unsigned int v = 0; v < n; v++) {
			IloExpr expr;
			model.add(c[v] <= lambda);
			model.add(c[v] >= 0);
			expr.end();
		}

		std::cout << "Number of variables " << numvar << "\n";

		/* solve the Model*/
		cplex.extract(model);
		cplex.exportModel("L-Labeling.lp");

		/*
		 start = clock();
		 int solveError = cplex.solve();
		 end = clock ();
		 */

		IloTimer timer(env);
		timer.start();
		int solveError = cplex.solve();
		timer.stop();

		if (!solveError) {
			std::cout << "STATUS : " << cplex.getStatus() << "\n";
			env.error() << "Failed to optimize LP \n";
			exit(1);
		}
		//Info.time = (double)(end-start)/(double)CLOCKS_PER_SEC;
		Info.time = timer.getTime();

		std::cout << "STATUS : " << cplex.getStatus() << "\n";
		/* get the solution*/
		env.out() << "Solution status = " << cplex.getStatus() << "\n";
		numconst = cplex.getNrows();
		env.out() << " Number of constraints = " << numconst << "\n";
		lambda_G_d = cplex.getObjValue();
		env.out() << "Solution value  = " << lambda_G_d << "\n";
		for (unsigned int u = 0; u < n; u++) {
			C.push_back(cplex.getValue(c[u]));
			std::cout << "c(" << u << ")=" << C[u] << " ";
		}
		std::cout << "\n";
		/*
		 for (unsigned int u=0; u<n; u++) {
		 for (unsigned int v=0; v<u; v++) {
		 std::cout<< "z[" << u <<"][" << v << "]="<< cplex.getValue( z[u][v]) << " ";
		 Z.push_back(cplex.getValue( z[u][v]));
		 }
		 std::cout << "\n";
		 }
		 std::cout <<"\n";
		 */
	} // end try
	catch (IloException& e) {
		std::cerr << "Concert exception caught: " << e << std::endl;
	} catch (...) {
		std::cerr << "Unknown exception caught" << std::endl;
	}
	env.end();
	return 0;
}

int CProblem::save_result(std::string _fileName) {
	std::ofstream outFile;

	std::string outFileNameSolve = "results/lineFunction/resultsSolve_";
	std::string outFileName;

	std::stringstream ss;
	ss << max_d;
	std::string str = "d" + ss.str();
	outFileNameSolve += str;

	outFileNameSolve += "_function_";

	outFileName = outFileNameSolve;

	unsigned found = fileName.find_last_of("/");
	std::string line = fileName.substr(found + 1,
			fileName.length() - 1 - found);

	outFileName = outFileName + line;

	outFile.open(outFileName.c_str());
	if (!outFile.is_open()) {
		std::cerr << "Cannot open file to save a result!\n";
		exit(1);
	}

	outFile << "Name of file:\t" << _fileName << "\n";
	outFile << "Optimal value:\t" << lambda_G_d << "\n";
	outFile << "Number of constraints: " << numconst << "\n";
	outFile << "Number of variables: " << numvar << "\n";
	outFile << "Used time: " << Info.time << "sec\n";
	outFile << "Labeling:\n";
	for (unsigned int u = 0; u < C.size(); u++)
		outFile << "c[" << u + 1 << "]=" << C[u] << " ";

	outFile.close();
	return 0;
}

int main(int argc, char **argv) {
	std::string fileName = "dummy.txt";

	if (argc > 4 && argc < 2) {
		std::cerr << "LLabeling <d max> <lattice> <file name to open> \n";
		std::cerr
				<< "lattice: 0 read from file; 1 hexagonal lattice; 2 triangular lattice; 3 square lattice";
		return 1;
	}

	if (atoi(argv[2]) == 0) {
		fileName = argv[3];
		std::cout << "Start with the file " << fileName << "\n";
	}

	CProblem newProblem(atoi(argv[1]), atoi(argv[2]), fileName);

	newProblem.print();

	newProblem.setModel();

	newProblem.save_result(fileName);

	return 0;
} // END main

