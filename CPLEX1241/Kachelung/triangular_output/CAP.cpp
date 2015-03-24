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

MatrixDouble hexagonalLattice(int n, int n1) {
	MatrixDouble coordHex;
	int i=0, l=0, j =0;
	int n_pro_level = n1;
	int sum = n1;
	int dx=0;
	double y=0;

	coordHex.resize(n);
	while (i<n) {
		if (i==(sum)) {	// Uebergang zu dem naechsten Level
			l++;
			if (sum<=n/2) {
				dx += l%2;
				n_pro_level = n_pro_level + l%2;
			}
			else {
				dx -= l%2;
				n_pro_level = n_pro_level - l%2;
			}
			sum += n_pro_level;
			y = y+ (1+(l+1)%2)*1/2.;
		}

		coordHex[i].resize(2);
		coordHex[i][0] = -dx*sqrt(3)/2.+j* sqrt(3);
		coordHex[i][1] = y;

		j = (j+1)%n_pro_level;
		i++;
	}

	return coordHex;
}

MatrixDouble triangularLattice(int n, int n1) {
	MatrixDouble coordHex;
	int i=0, l=0, j =0;
	int n_pro_level = n1;
	int sum = n1;
	double y=0, dx=0;

	coordHex.resize(n);

	std::cout << n_pro_level << n1 << "\n";

	while (i<n) {
		if (i==(sum)) {	// Uebergang zu dem naechsten Level
			l++;
			dx = 0.5*(l%2) ;
			n_pro_level = n_pro_level + (l+1)%2 - l%2;
			sum += n_pro_level;
			y = y+sqrt(3)/2.;
			std::cout << n_pro_level << n1 << "\n";
		}

		coordHex[i].resize(2);
		coordHex[i][0] = dx +j;
		coordHex[i][1] = y;

		j = (j+1)%n_pro_level;
		i++;
	}

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

	CProblem (int _max_d, int _lattice, std::string _fileName){
		lattice = _lattice;

		fileName=_fileName;
		numconst = 0;
		numvar = 0;


		max_d = _max_d;
		max_distance = max_d;

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


int CProblem::setProblem() {
	std::string line;
	std::fstream oFile;
	std::stringstream buf;
	//int bufInt;

	switch (lattice) {
		case 0:
			std::cout << "LATTICE " << lattice << " Open File \n";
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

		case 1:
			std::cout << "LATTICE " << lattice << " Hexagonal Lattice \n";
			n=24;
			coordinates = hexagonalLattice(n, 2); // number of vertices, number of vertices in first line
			diameter = 7;
			fileName = "hexagonal_lattice.dat";
			break;

		case 2:
			std::cout << "LATTICE " << lattice << "Triangular Lattice\n";
			n=23;
			coordinates = triangularLattice(n,5); // number of vertices, number of vertices in first line
			diameter = 6;
			fileName = "triangular_lattice.dat";
			break;

		case 3:
			std::cout << "LATTICE " << lattice << "Square Lattice \n";
			n=25;
			coordinates = squareLattice(n);
			diameter = 8;
			fileName = "square_lattice.dat";
			break;

		default:std::cerr << "Wrong lattice type\n"; exit (-1);
	}	//switch

	coordTOdist();

	set_d_function();

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

		// d(x) = 3 - x
		if (max_distance == 2) 
		{
		   // Dreiecke mit Seitenlaenge 1
		  for (int i=0; i<n-6; i++)
		  {
		    
		    if (i!= 4 && i!=8 && i!=13 && i!=17)
		    {
		      model.add(c[i]+c[i+1]+c[i+5] >= 6); 
		    }
		  }
		  for (int i=0; i<n-6; i++)
		  {
		    if (i!= 4 && i!=8 && i!=13 && i!=17)
		    {
		      model.add(c[i+1]+c[i+5]+c[i+6] >= 6); 
		    }
		  }
		  //
		}
		
		//d(x) = 4 - x
		if (max_distance == 3) 
		{
		  // Dreiecke mit Seitenlaenge 1
		  for (int i=0; i<n-6; i++)
		  {
		    if (i!= 4 && i!=8 && i!=13 && i!=17)
		    {
		      model.add(c[i]+c[i+1]+c[i+5] >= 9); 
		    }
		  }
		  for (int i=0; i<n-6; i++)
		  {
		    if (i!= 4 && i!=8 && i!=13 && i!=17)
		    {
		      model.add(c[i+1]+c[i+5]+c[i+6] >= 9); 
		    }
		  }
		  //
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
		
		cplex.setParam(IloCplex::ClockType , 1 ); // CPU time

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


	std::string outFileNameSolve="results/resultsSolve_";
	std::string outFileName;

	std::stringstream ss;
	ss << max_d;
	std::string str = "d" + ss.str();
	outFileNameSolve += str;

	outFileNameSolve += "_function_";

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
	std::string fileName = "dummy.txt";

	if (argc > 4 && argc < 2) {
		std::cerr << "LLabeling <d max> <lattice> <file name to open> \n";
		std::cerr << "lattice: 0 read from file; 1 hexagonal lattice; 2 triangular lattice; 3 square lattice";
		return 1;
	}

	if (atoi(argv[2])==0) {
		fileName=argv[3];
		std::cout << "Start with the file " << fileName << "\n";
	}

	CProblem newProblem(atoi(argv[1]), atoi(argv[2]), fileName);

	//newProblem.print();

	newProblem.setModel();

	newProblem.save_result(fileName);

	return 0;
}  // END main

