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


double abs_double(double a) {
    return (a<0.0) ? -a : a;
}

struct SSolutionInfo {
	double time;
	int nodeleftcnt;

};

class CProblem{
private:
	std::string fileName;
	int setProblem(); // read data from file
	void coordTOdist();
public:
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

	CProblem (std::string _fileName){
		fileName=_fileName;
		numconst = 0;
		numvar = 0;


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
	max_d = 3 + 1;
	d.resize(n);
	for (unsigned int u=0; u<n; u++) {
		d[u].resize(n);
		for (unsigned int v=0; v<u; v++) {
			if (distances[u][v]-1 <= 0.01) d[u][v] = 3.;
			else
				if (distances[u][v]-2 <= 0.01) d[u][v] = 2.;
				else
					if (distances[u][v]-3 <= 0.01) d[u][v] = 1.;
					else d[u][v] = 0.;
			d[v][u] = d[u][v];
		}
		d[u][u] = max_d;
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

	max_d = diameter+1;
	max_distance = max_d;

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

//		cplex.setParam(IloCplex::Threads, 1);
		cplex.setParam(IloCplex::ClockType , 1 ); // CPU time
		
		IloTimer timer(env);
		timer.start();
		const double startt = cplex.getTime();
		int solveError = cplex.solve();
		const double stopt = cplex.getTime() - startt;
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
		env.out() << "Time cplex.getTime() : " << stopt << "\n";
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

	outFileNameSolve += "_TrepenFunkt3_";

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
	if (argc!=2) {
		std::cerr << "LLabeling <file name to open> \n";
		return 1;
	}
	std::string fileName=argv[1];
	std::cout << "Start with the file " << fileName << "\n";

	CProblem newProblem(fileName);
	newProblem.print();

	newProblem.setModel();

	newProblem.save_result(fileName);

	return 0;
}  // END main


