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

typedef std::vector <int> VectorInt;
typedef std::vector <VectorInt > MatrixInt;

typedef std::vector <double> VectorDouble;
typedef std::vector <VectorDouble > MatrixDouble;

struct SSolutionInfo {
	double time;
	int nodeleftcnt;

};

MatrixDouble hexagonalLattice() {
	MatrixDouble coordHex;
	int n = 6;
	coordHex.resize(n);

	for (int i=0; i<n; i++) {
		coordHex[i].resize(2);

	}
	coordHex[0][0] = 0; coordHex[0][1] = 0;
	coordHex[1][0] = -sqrt(3)/2.; coordHex[1][1] = 1/2.;
	coordHex[2][0] = +sqrt(3)/2.; coordHex[2][1] = 1/2.;
	coordHex[3][0] = -sqrt(3)/2.; coordHex[3][1] = 3/2.;
	coordHex[4][0] = +sqrt(3)/2.; coordHex[4][1] = 3/2.;
	coordHex[5][0] = 0; coordHex[5][1] = 2.;
		/*
	coordHex[5][0] = 0; coordHex[5][1] = 0;
	coordHex[6][0] = 0; coordHex[6][1] = 0;
	coordHex[7][0] = 0; coordHex[7][1] = 0;
	coordHex[8][0] = 0; coordHex[8][1] = 0;
	coordHex[9][0] = 0; coordHex[9][1] = 0;
	coordHex[10][0] = 0; coordHex[10][1] = 0;
	coordHex[11][0] = 0; coordHex[11][1] = 0;
	coordHex[12][0] = 0; coordHex[12][1] = 0;
	coordHex[13][0] = 0; coordHex[13][1] = 0;
	coordHex[14][0] = 0; coordHex[14][1] = 0;
	coordHex[15][0] = 0; coordHex[15][1] = 0;
	coordHex[16][0] = 0; coordHex[16][1] = 0;
	coordHex[17][0] = 0; coordHex[17][1] = 0;
	coordHex[18][0] = 0; coordHex[18][1] = 0;
	coordHex[19][0] = 0; coordHex[19][1] = 0;
	coordHex[20][0] = 0; coordHex[20][1] = 0;
	coordHex[21][0] = 0; coordHex[21][1] = 0;
	coordHex[22][0] = 0; coordHex[22][1] = 0;
	coordHex[23][0] = 0; coordHex[23][1] = 0;
*/

	return coordHex;
}

MatrixDouble triangularLattice() {
	MatrixDouble coordHex;
	int n = 6;
	coordHex.resize(n);

	for (int i=0; i<n; i++) {
		coordHex[i].resize(2);

	}
	coordHex[0][0] = 0; coordHex[0][1] = 0;


	return coordHex;
}

MatrixDouble squareLattice(int n) {
	MatrixDouble coord;

	coord.resize(n);

	for (int i=0; i<sqrt(n); i++) {
		for (int j=0; j< sqrt(n); j++) {
			coord[i*sqrt(n)+j].resize(2);

			coord[i*sqrt(n)+j][0] = i;
			coord[i*sqrt(n)+j][1] = j;
		}
	}



	return coord;
}


class CProblem{
private:
	std::string fileName;
	int setProblem(); // read data from file
	void coordTOdist();
public:
	int lattice;
	MatrixDouble distances;
	MatrixDouble coordinates;
	MatrixDouble d;	// use simply line-function
	VectorDouble C;	// labeling
	VectorInt Z;	// additional variables
	unsigned int n;		// number of vertices
	unsigned int diameter;		// diameter
	double lambda_G_d;	// lambda(G, d_1, d_2, .., d_s)
	double UB; // lambda <= UB

	int usefunction;

	double max_distance;
	double max_d;
	SSolutionInfo Info;

	int numconst ;
	int numvar;

	void print();
	void set_d_function();
	void set_d();
	int setModel();
	int save_result(std::string fileName);

	CProblem (int _lattice,std::string _fileName, int _usefunction, int _max_d){
		lattice = _lattice;

		fileName=_fileName;
		numconst = 0;
		numvar = 0;

		usefunction = _usefunction;
		max_d = _max_d;
		max_distance = max_d;
		if (usefunction) std::cout << " \n For d will be linear function of distances used \n";
		else std::cout << " we considering the case L(" << max_d << ", " << max_d -1 << ", ..., 1)\n";

		setProblem();

	};

	double dist_linear (double x) {
		double value = 1+max_distance - x ;
		return (value<0) ? 0 : value;			/* linear function of distances*/
		//return value;
	}
};
void CProblem::coordTOdist () {
	distances.resize(n);
	//max_distance = 0.0;
	for (unsigned int u=0; u< n; u++) {
		distances[u].resize(n);
		for (unsigned int v=0; v<u; v++) {
			distances[u][v] = (sqrt( (coordinates[u][0]-coordinates[v][0])*(coordinates[u][0]-coordinates[v][0])
								          +(coordinates[u][1]-coordinates[v][1])*(coordinates[u][1]-coordinates[v][1]) )
							  );
			distances[v][u] = distances[u][v];
			//if (distances[u][v] > max_distance) max_distance = distances[u][v];
		}

		distances[0][0] = 0;
	}
}
void CProblem::set_d_function(){
	max_d = dist_linear(0);
	d.resize(n);
	for (unsigned int u=0; u<n; u++) {
		d[u].resize(n);
		for (unsigned int v=0; v<n; v++) {
			d[u][v] = dist_linear(distances[u][v]);
		}
	}
}

void CProblem::set_d(){
	d.resize(max_d);
	for (unsigned int i=0; i<max_d; i++) {
		d[i].resize(1);
		d[i][0]= max_d-i;
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
			std::cerr<< "Cannot open file!\n";
			exit(1);
		}

		std::getline(oFile, line);	/* number of vertices and diameter */
		std::getline(oFile, line);
		buf<<line;
		buf >> n;
		buf >> diameter;
		buf.clear();

		std::getline(oFile, line);	/* coordinates */
		coordinates.resize(n);
		for (unsigned int i=0; i< n; i++) {
			coordinates[i].resize(2);
			std::getline(oFile, line);
			buf << line;
			buf >> bufDouble;	/* because first number in line is the number of the vertex */
			buf >> bufDouble;
			coordinates[i][0] = bufDouble;
			buf >> bufDouble;
			coordinates[i][1] = bufDouble;
			buf.clear();
		}
		oFile.close();
		break;

	case 1: coordinates = hexagonalLattice();break;

	case 2: coordinates = triangularLattice();break;

	case 3: coordinates = squareLattice(25);break;

	default:std::cerr << "Wrong lattice type\n"; exit (-1);
	}	//switch
	coordTOdist();

	if (usefunction) set_d_function();
	  else set_d();

	return 0;
}

void CProblem::print(){
	std::cout << "\n";
	std::cout << "Graph (V,E) with |V|= " << n <<" and diameter = "<< diameter <<"\n";
	std::cout << "\n";

	std::cout << "Coordinates: "<<"\n";
	for (unsigned int u=0; u<n; u++) {
		std::cout << "vertex " << u+1 << " : "<< coordinates[u][0] << " " << coordinates[u][1] << "\n";
	}
	std::cout << "\n";


	std::cout << "Distances: "<<"\n";
	for (unsigned int u=0; u<n; u++) {
		for (unsigned int v=0; v<n; v++) {
			std::cout << std::setw( 3 )  << distances[u][v] << " ";
		}
		std::cout << "\n";
	}


	std::cout << "d: "<<"\n";
	for (unsigned int u=0; u<d.size(); u++) {
		for (unsigned int v=0; v<d[u].size(); v++) {
			std::cout << std::setw( 3 )  << d[u][v] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n";
	std::cout << "max distance : " << max_distance;
	std::cout << "\n";
}

int CProblem::setModel() {
	//time_t start, end;

	numvar = 1+n; // lambda + all c;

	IloEnv  env;
	try {
		IloModel model(env);
		IloCplex cplex(env);

		/*Variables*/
		IloNumVar lambda(env, "lambda");
		IloNumVarArray c(env, n);//
		for (unsigned int u=0; u<n; u++) {
			std::stringstream ss;
			ss << u;
			std::string str = "c" + ss.str();
			c[u]=IloNumVar(env, str.c_str());
		}

		IloArray<IloIntVarArray> z(env,n);
		for (unsigned int u=0; u<n; u++) {
			z[u]= IloIntVarArray(env, n);
			for (unsigned int v=0; v<n; v++) {
				std::stringstream ss;
				ss << u;
				ss << v;
				std::string str = "z" + ss.str();
				z[u][v] = IloIntVar(env, 0, 1, str.c_str());
			}
		}

		/* Constant M*/
		int M=n*max_d;
		UB = M;

		/*  Objective*/
		model.add(IloMinimize(env, lambda));

		/*Constrains*/
		model.add(lambda - UB <= 0);

		if (usefunction) {
			/* d=function of the distance */
			IloArray<IloNumArray> Par_d(env,n);
			for (unsigned int u=0; u<n; u++) {
				Par_d[u]=IloNumArray(env,n);
				for (unsigned int v=0; v<n; v++) {
					Par_d[u][v]=d[u][v];
				}
			}

			for (unsigned u=0; u<n; u++) {
				for (unsigned v=0; v<u; v++) {
					model.add(c[v]-c[u]+M*   z[u][v]  >= Par_d[u][v] );
					model.add(c[u]-c[v]+M*(1-z[u][v]) >= Par_d[u][v]);
					numvar++; // + z[u][v]
				}
			}

/*
			for (unsigned i=0; i<sqrt(n)-1; i=i+2) {
				for (unsigned j=0; j<sqrt(n)-1; j=j+2) {
//					model.add (c[i*sqrt(n)+j]+c[i*sqrt(n)+j+1] +
//					           c[(i+1)*sqrt(n)+j]+c[(i+1)*sqrt(n)+j+1] >= 16-4*sqrt(2));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 3-x
					model.add (c[i*sqrt(n)+j]+c[i*sqrt(n)+j+1] +
					           c[(i+1)*sqrt(n)+j]+c[(i+1)*sqrt(n)+j+1] >= 22 - 4*sqrt(2));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 4-x

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
					model.add (c[i*sqrt(n)+j]    + c[i*sqrt(n)+j+1]    + c[i*sqrt(n)+j+2] +
					           c[(i+1)*sqrt(n)+j]+ c[(i+1)*sqrt(n)+j+1]+ c[(i+1)*sqrt(n)+j+2] +
					           c[(i+2)*sqrt(n)+j]+ c[(i+2)*sqrt(n)+j+1]+ c[(i+2)*sqrt(n)+j+2]
					           >= 82-8*sqrt(2)-2*sqrt(5));  // Bedingung fuer Quadratischen Gridgraph und d(x) = 3-x
				}
			}
*/

		} else {
			/* dieser Fall wurde geschrieben nur um die Situation, wann Distanzen gerundet und mit 10 meltipliziert werden und Labelings L(40, 39, 38 ...)
			 L(30,29,...) und L(20,19,18,..) betrachtet werden, am einfachsten berechen zu koennen.
			 Es muss zusaetzlich noch die Berechnunge von Distanzen entsprechend angepasst werden*/


			/* distance Matrix*/
			IloArray<IloNumArray> Par_dist(env,n);
			for (unsigned int u=0; u<n; u++) {
				Par_dist[u]=IloNumArray(env,n);
				for (unsigned int v=0; v<n; v++) {
					Par_dist[u][v]=distances[u][v];
				}
			}
			/* d=(max_d, max_d-1, max_d-2, ..., 1) */
			IloIntArray Par_d(env,max_d);
			for (unsigned int i=0; i<max_d; i++) {
				Par_d[i]=d[i][0];
			}
			for (unsigned u=0; u<n; u++) {
				for (unsigned v=0; v<u; v++) {
					if (Par_dist[u][v] <=max_d) {
						model.add(c[v]-c[u]+M*z[u][v] >= Par_d[Par_dist[u][v]-1]);
						model.add(c[u]-c[v]+M*(1-z[u][v]) >= Par_d[Par_dist[u][v]-1]);
					}
				}
			}

		}

		for (unsigned int v=0; v<n; v++) {
			IloExpr expr;
			model.add (c[v] <= lambda);
			model.add (c[v] >= 0);
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

		if ( !solveError ) {
			std::cout << "STATUS : "<< cplex.getStatus() << "\n";
			env.error() << "Failed to optimize LP \n";
			exit(1);
		}
		//Info.time = (double)(end-start)/(double)CLOCKS_PER_SEC;
		Info.time = timer.getTime();

		std::cout << "STATUS : "<< cplex.getStatus() << "\n";
		/* get the solution*/
		env.out() << "Solution status = " << cplex.getStatus() << "\n";
		numconst = cplex.getNrows();
		env.out() << " Number of constraints = " << numconst << "\n";
		lambda_G_d=cplex.getObjValue();
		env.out() << "Solution value  = " << lambda_G_d << "\n";
		for (unsigned int u=0; u<n; u++) {
			C.push_back(cplex.getValue(c[u]));
			std::cout << "c(" << u << ")=" << C[u]<< " ";
		}
		std::cout <<"\n";
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
	}	// end try
	catch (IloException& e) {
		std::cerr << "Concert exception caught: " << e << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception caught" << std::endl;
	}
	env.end();
	return 0;
}

int CProblem::save_result(std::string _fileName){
	std::ofstream outFile;


	std::string outFileNameSolve="results/lineFunction/resultsSolve_";
	std::string outFileName;

	std::stringstream ss;
	ss << max_d;
	std::string str = "d" + ss.str();
	outFileNameSolve += str;

	if (usefunction) outFileNameSolve += "_function_";

	outFileName = outFileNameSolve;

	unsigned found = fileName.find_last_of("/");
		std::string line=fileName.substr(found+1, fileName.length()-1-found);

	outFileName=outFileName+line;



	outFile.open(outFileName.c_str());
	if (!outFile.is_open()) {
		std::cerr<< "Cannot open file to save a result!\n";
		exit(1);
	}

	outFile<< "Name of file:\t"<<_fileName<<"\n";
	outFile<< "Optimal value:\t"<<lambda_G_d<<"\n";
	outFile << "Number of constraints: " << numconst << "\n";
	outFile << "Number of variables: " << numvar << "\n";
	outFile << "Used time: " << Info.time << "sec\n";
	outFile<< "Labeling:\n";
	for (unsigned int u=0; u<C.size(); u++)
		outFile << "c[" << u+1 << "]=" << C[u] <<" ";

	outFile.close();
	return 0;
}

int main (int argc, char **argv) {

	if (argc!=4) {
		std::cerr << "LLabeling <lattice> <file name to open> <number>\n";
		std::cerr << "lattice: 0 read from file; 1 hexagonal lattice; 2 triangular lattice; 3 square lattice";
		return 1;
	}

	std::string fileName=argv[2];
	std::cout << "Start with the file " << fileName << "\n";

//	CProblem newProblem(fileName, atoi(argv[2]),  atoi(argv[3]));
	CProblem newProblem(atoi(argv[1]), fileName, 1,  atoi(argv[3]));
	newProblem.print();

	newProblem.setModel();

	newProblem.save_result(fileName);

	return 0;
}  // END main


 
