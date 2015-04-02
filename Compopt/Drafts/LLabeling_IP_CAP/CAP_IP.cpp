#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

/*  CPLEX*/
#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

typedef std::vector<unsigned int> Vector;
typedef std::vector<Vector> Matrix;

struct SSolutionInfo
{
  double time;
  int nodeleftcnt;

  int numconst;
  int numvar;

};

class CProblem
{
private:
  std::string fileName;
  int
  setProblem(); // read data from file
public:
  Matrix distanceMatrix;
  Vector distanceVektor;
  Vector d; // weights
  Vector C; // labeling
  unsigned int n; // matrix m x n
  unsigned int max_d; // dimension of weight-vector
  int lambda_G_d; // lambda(G, d_1, d_2, .., d_s)

  SSolutionInfo Info;

  CProblem(std::string _fileName)
  {
    fileName = _fileName;
    setProblem();
  }
  ;
  void
  print();
  int
  setModel();
  int
  save_result(std::string _fileName);
};

int
CProblem::setProblem()
{
  std::string line;
  std::fstream oFile;
  std::stringstream buf;

  oFile.open(fileName.c_str());
  if (!oFile.is_open())
    {
      std::cerr << "Cannot open file!\n";
      exit(1);
    }
  // read n and max_d
  std::getline(oFile, line);
  buf << line;
  buf >> n;
  buf >> max_d;
  buf.clear(); //clear buf

  std::getline(oFile, line);
  d.resize(max_d);
  for (unsigned i = 0; i < max_d; i++)
    d[i] = max_d - i;
  /*
   buf<<line;

   int buf_int;
   while (buf>>buf_int) {
   d.push_back(buf_int);
   }

   if (max_d != d.size()) {
   std::cerr << "#Error in second line!\n";
   exit(1);
   }
   buf.clear();

   */
  distanceMatrix.resize(n);

  for (unsigned int i = 0; i < n; i++)
    {
      distanceMatrix[i].resize(n);
      distanceMatrix[i][i] = 0;

      std::getline(oFile, line);

      std::stringstream buf(line);
      int int_buf;

      for (unsigned int j = 0; j < i; j++)
        {
          buf >> int_buf;
          distanceVektor.push_back(int_buf);
          distanceMatrix[i][j] = int_buf;
          distanceMatrix[j][i] = distanceMatrix[i][j];
        }
      buf.clear(); //clear buf

    }

  oFile.close();
  return 0;
}

void
CProblem::print()
{
  std::cout << "" << "\n";
  std::cout << "Graph (V,E) with |V|= " << n << "\n";

  std::cout << "Distance matrix: " << "\n";
  for (unsigned int i = 0; i < n; i++)
    {
      for (unsigned int j = 0; j < n; j++)
        {
          std::cout << distanceMatrix[i][j] << "  ";
        }
      std::cout << "\n";
    }

  std::cout << "Labeling L(d_1, d_2, ..., d_s) max_d=" << max_d << "\n";
  for (unsigned int i = 0; i < max_d; i++)
    {
      std::cout << d[i] << "  ";
    }
  std::cout << "\n";
}

int
CProblem::setModel()
{

  IloEnv env;
  try
    {
      IloModel model(env);
      IloCplex cplex(env);

      /*Variables*/
      IloIntVar lambda(env);
      IloNumVarArray c(env, n); //
      for (unsigned int i = 0; i < n; i++)
        {
          c[i] = IloIntVar(env);
        }

      IloArray<IloIntVarArray> z(env, n);
      for (unsigned int i = 0; i < n; i++)
        {
          z[i] = IloIntVarArray(env, n, 0, 1);
        }

      Info.numvar = 1 + n; // lambda + all c;
      /* Parameter */

      /* distance Matrix*/
      IloArray<IloIntArray> Par_dist(env, n);
      for (unsigned int i = 0; i < n; i++)
        {
          Par_dist[i] = IloIntArray(env, n);
          for (unsigned int j = 0; j < n; j++)
            {
              Par_dist[i][j] = distanceMatrix[i][j];
            }
        }
      /* d=(d_1, d_2, ..., d_s) */
      IloIntArray Par_d(env, max_d);
      for (unsigned int i = 0; i < max_d; i++)
        {
          Par_d[i] = d[i];
        }
      /* Constant M*/
      int M = n * Par_d[0];
      /*
       *  Objective*/
      model.add(IloMinimize(env, lambda));

      /*Constrains*/
      model.add(lambda - M <= 0);

      for (unsigned u = 0; u < n; u++)
        {
          for (unsigned v = 0; v < u; v++)
            {
              if (Par_dist[u][v] <= max_d)
                {
                  model.add(
                      c[v] - c[u] + M * z[u][v] >= Par_d[Par_dist[u][v] - 1]);
                  model.add(
                      c[u] - c[v] + M * (1 - z[u][v])
                          >= Par_d[Par_dist[u][v] - 1]);
                }
            }
        }

      for (unsigned u = 0; u < n; u++)
        {
          IloExpr expr(env);
          int deg = 0;
          expr = c[u];

          for (unsigned v = 0; v < u; v++)
            {
              if (Par_dist[u][v] == 1)
                {
                  deg += 1;
                  expr += c[v];
                }
            }
          model.add(expr >= (deg + 2.) * (deg - 1) * Par_d[1] / 2. + Par_d[0]);

          expr.end();
        }

      for (unsigned int v = 0; v < n; v++)
        {
          IloExpr expr;

          model.add(c[v] <= lambda);
          model.add(c[v] >= 0);
          expr.end();
        }

      /* solve the Model*/
      cplex.extract(model);
      cplex.exportModel("L-Labeling.lp");

      IloTimer timer(env);
      timer.start();
      int solveError = cplex.solve();
      timer.stop();

      if (!solveError)
        {
          env.error() << "Failed to optimize LP \n";
          exit(1);
        }
      /* get the solution*/
      Info.time = timer.getTime();
      env.out() << "Solution status = " << cplex.getStatus() << "\n";
      lambda_G_d = cplex.getObjValue();
      Info.numconst = cplex.getNrows();
      env.out() << "Solution value  = " << lambda_G_d << "\n";
      for (unsigned int i = 0; i < n; i++)
        {
          C.push_back(cplex.getValue(c[i]));
          std::cout << "c(" << i << ")=" << C[i] << " ";
        }
      std::cout << "\n";
    }
  catch (IloException& e)
    {
      std::cerr << "Concert exception caught: " << e << std::endl;
    }
  catch (...)
    {
      std::cerr << "Unknown exception caught" << std::endl;
    }
  env.end();

  return 0;
}
int
CProblem::save_result(std::string fileName)
{
  std::ofstream outFile;
  std::string outFileName = "results/results_";

  unsigned found = fileName.find_last_of("/");
  std::string line = fileName.substr(found + 1, fileName.length() - 1 - found);

  outFileName = outFileName + line;
  outFile.open(outFileName.c_str());
  if (!outFile.is_open())
    {
      std::cerr << "Cannot open file to save a result!\n";
      exit(1);
    }

  outFile << "File Name:\t" << fileName << "\n";
  outFile << "Optimal value\t" << lambda_G_d << "\n";
  outFile << "Used time: " << Info.time << "sec\n";
  outFile << "Number of constraints: " << Info.numconst << "\n";
  outFile << "Number of variables: " << Info.numvar << "\n";

  outFile << "Labeling\n";
  for (unsigned int i = 0; i < C.size(); i++)
    outFile << "c[" << i + 1 << "]=" << C[i] << " ";

  outFile.close();
  return 0;
}

int
main(int argc, char **argv)
{
  if (argc < 2)
    {
      std::cerr << "No file name to open!\n";
      return 1;
    }
  std::string fileName = argv[1];
  std::cout << "Start with the file " << fileName << "\n";

  CProblem newProblem(fileName);
  //newProblem.print();

  newProblem.setModel();
  newProblem.save_result(fileName);
  return 0;
} // END main

