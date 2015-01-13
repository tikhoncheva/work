#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

static void usage (const char *progname)
{
  
}


int main (int argc, char **argv) {
  
     IloEnv env;
    
     try {
        IloModel model(env);
        IloCplex cplex(env);
  
        if (argc != 2) {
	   usage (argv[0]);
           throw(-1);
        }
   
        IloObjective   obj;
        IloNumVarArray var(env);
        IloRangeArray  rng(env);
        cplex.importModel(model, argv[1], obj, var, rng);
	
	cplex.setParam(IloCplex::Threads,1);
	cplex.setParam(IloCplex::ClockType,1);
  
        cplex.extract(model);
	
	
	IloTimer timer(env);
	timer.start();
	int solveError = cplex.solve();
	timer.stop();

	if ( !solveError ) {
	    std::cout << "STATUS : "<< cplex.getStatus() << "\n";
	    env.error() << "Failed to optimize LP \n";
	    exit(1);
	}
	
  
        IloNumArray vals(env);
        cplex.getValues(vals, var);
        env.out() << "Solution status = " << cplex.getStatus() << endl;
        env.out() << "Solution value  = " << cplex.getObjValue() << endl;
        env.out() << "Solution vector = " << vals << endl;
	env.out() << "Solution time = " << timer.getTime() << endl;
  
        try {     // basis may not exist
           IloCplex::BasisStatusArray cstat(env);
           cplex.getBasisStatuses(cstat, var);
           env.out() << "Basis statuses  = " << cstat << endl;
        } catch (...) {
        }
  
        env.out() << "Maximum bound violation = "
                  << cplex.getQuality(IloCplex::MaxPrimalInfeas) << endl;
     }
     catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
     }
     catch (...) {
        cerr << "Unknown exception caught" << endl;
     }
  
     env.end();
     return 0;
     

}  // END main


