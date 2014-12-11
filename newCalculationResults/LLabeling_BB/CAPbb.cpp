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
  double preSolvetime;

  int numAddVar;
  VectorInt ConstrIndex;
  int numConstr;
  int numVar;
};

struct cplexData
{
  double LB, UB;

  IloModel model;
  IloCplex cplex;
  IloNumVar lambda; // Objective
  IloNumVarArray c; // Labels
  IloIntVarArray z; // addition variables
  IloArray<IloNumArray> par_d; // parameters
  IloObjective OBJ;
};

MatrixDouble
hexagonalLattice(int n, int n1)
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
MatrixDouble
hexagonalLattice2(int n, int n1)
{
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

MatrixDouble
triangularLattice(int n, int n1)
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

MatrixDouble
triangularLattice2(int n, int n1)
{
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

MatrixDouble
squareLattice(int n)
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

MatrixDouble
squareLattice2(int n)
{
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

  double max_distance;

  SSolutionInfo Info;
  cplexData modelData;
  void
  print();
  void
  set_d_function();
  void
  set_d();
  void
  setVariables(IloEnv env);
  void
  setConstrains(IloModel model);
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
    Info.time = 0;
    Info.preSolvetime = 0;

    max_distance = _max_d ;

    setProblem();

  }
  ;

  double
  dist_linear(double x)
  {
    double value = 1 + max_distance - x;
    return (value < 0) ? 0 : value; /* linear function of distances*/
    //return value;
  }
};
void
CProblem::coordTOdist()
{
  distances.resize(n);
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
        }

      distances[0][0] = 0;
    }
}
void
CProblem::set_d_function()
{
  //max_distance = dist_linear(0);
  d.resize(n);
  for (unsigned int u = 0; u < n; u++)
    {
      d[u].resize(n);
      for (unsigned int v = 0; v < u; v++)
        {
          d[u][v] = dist_linear(distances[u][v]);
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

int
CProblem::setProblem()
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
    Info.numVar = 1 + n; // lambda + all c;
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

void
CProblem::print()
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

void
CProblem::setVariables(IloEnv env)
{
  //
  modelData.lambda = IloNumVar(env, "lambda");
  // Labels
  modelData.c = IloNumVarArray(env, n);
  for (unsigned int u = 0; u < n; u++)
    {
      std::stringstream ss;
      ss << u;
      std::string str = "c" + ss.str();
      modelData.c[u] = IloNumVar(env, str.c_str());
    }
  // additional variables z
  modelData.z = IloIntVarArray(env, Info.numAddVar);
  for (int i = 0; i < Info.numAddVar; i++)
    {
      std::stringstream ss;
      ss << i;
      std::string str = "z_" + ss.str();
      modelData.z[i] = IloIntVar(env, 0, 1, str.c_str());
    }
  // Parameter d=function of the distance
  modelData.par_d = IloArray<IloNumArray>(env, n);
  for (unsigned int u = 0; u < n; u++)
    {
      modelData.par_d[u] = IloNumArray(env, n);
      for (unsigned int v = 0; v < n; v++)
        {
          modelData.par_d[u][v] = d[u][v];
        }
    }
  //Objective
  modelData.OBJ = IloMinimize(env, 0);
}
void
CProblem::setConstrains(IloModel model)
{
  switch (lattice)
    {
  case 1: // hexagonal_lattice
    if (max_distance == 2)
      {
        modelData.LB = 5.;
        modelData.UB = 10.;
      } //d(x) = 3 -x
    else
      {
        modelData.LB = 9.;
        modelData.UB = 27.;
      }
    break;
  case 2: // triangular_lattice
    if (max_distance == 2)
      {
        modelData.LB = 8.;
        modelData.UB = 16.;
      } //d(x) = 3 -x
    else
      {
        modelData.LB = 18.;
        modelData.UB = 54.;
      }
    break;
  case 3: // square_lattice
    if (max_distance == 2)
      {
        modelData.LB = 6.;
        modelData.UB = 12.;
      } //d(x) = 3 -x
    else
      {
        modelData.LB = 11.;
        modelData.UB = 33.;
      }
    break;
  default:
    modelData.LB = 0;
    modelData.UB = n * max_distance;
    break;
    }

  int M = max_distance + modelData.UB;
  for (int i = 0; i < Info.numAddVar; i++)
    {
      int u = Info.ConstrIndex[i * 2];
      int v = Info.ConstrIndex[i * 2 + 1];
      model.add(
          modelData.c[u] - modelData.c[v] + M * (modelData.z[i])
              >= modelData.par_d[u][v]);
      model.add(
          modelData.c[v] - modelData.c[u] + M * (1 - modelData.z[i])
              >= modelData.par_d[u][v]);
    }

  for (unsigned int v = 0; v < n; v++)
    {
      model.add(modelData.c[v] <= modelData.lambda);
      model.add(modelData.c[v] >= 0);
    }

  // add objective
  model.add(modelData.OBJ);
}

int
CProblem::setModel()
{
  IloEnv env;
  try
    {
      IloModel model(env);
      IloCplex cplex(env);

      setVariables(env);
      setConstrains(model);

      // tighten bounds for lambda
      IloTimer timer(env);
      int step = 0;
      double UB = modelData.UB, LB = modelData.LB, UBold = UB;
      IloExtractable AddConst1, AddConst2;

      std::cout << "Tighted Bounds for Lambda: " << "  old LB=" << LB
          << "  old UB = " << UB << "\n";
      //cplex.setOut(env.getNullStream());
      //cplex.setParam(IloCplex::PreInd, 0); // switch off presolve

      timer.start();
      while (UB - LB > .5)
        {
          UB = (LB + UB) / 2.; // move UB links
          step += 1;
          std::cout << "    Versuch " << step << "  :";

          // and add new with neu values
          AddConst1 = model.add(modelData.lambda - UB <= 0);
          AddConst2 = model.add(modelData.lambda - LB >= 0);

          cplex.extract(model);
          cplex.solve(); // try to solve

          if (cplex.getStatus() == IloAlgorithm::Optimal)
            {
              UBold = UB;
            }
          else
            {
              LB = UB;
              UB = UBold;
            }
          cplex.clear();
          std::cout << "  new LB=" << LB << "  new UB = " << UB << "\n";
          // delete old constrains
          model.remove(AddConst1);
          model.remove(AddConst2);
        }
      timer.stop();
      Info.preSolvetime = timer.getTime();

      modelData.UB = UB;
      modelData.LB = LB;
      // and add new with neu values
      AddConst1 = model.add(modelData.lambda - modelData.UB <= 0);
      AddConst2 = model.add(modelData.lambda - modelData.LB >= 0);

      // Solve Problem with new constrains
      modelData.OBJ.setExpr(modelData.lambda); // new Objective
      cplex.extract(model);
      cplex.exportModel("L-Labeling.lp");
      cplex.setOut(std::cout);

      timer.start();
      int solveError = cplex.solve();
      timer.stop();

      if (!solveError)
        {
          std::cout << "STATUS : " << cplex.getStatus() << "\n";
          env.error() << "Failed to optimize LP \n";
          exit(1);
        }

      Info.time = timer.getTime();
      env.out() << "Solution status = " << cplex.getStatus() << "\n";

      Info.numConstr = cplex.getNrows();
      env.out() << " Number of constraints = " << Info.numConstr << "\n";

      /* get the solution*/
      lambda_G_d = cplex.getObjValue();
      env.out() << "Solution value  = " << lambda_G_d << "\n";
      for (unsigned int u = 0; u < n; u++)
        {
          C.push_back(cplex.getValue(modelData.c[u]));
          std::cout << "c(" << u << ")=" << C[u] << " ";
        }
      std::cout << "\n\n";
    } // end try
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
CProblem::save_result(std::string _fileName)
{
  std::ofstream outFile;

  std::string outFileNameSolve = "results/resultsSolve_";
  std::string outFileName;

  std::stringstream ss;
  ss << max_distance;
  std::string str = "d" + ss.str();
  outFileNameSolve += str;

  outFileNameSolve += "_function_";

  outFileName = outFileNameSolve;

  unsigned found = fileName.find_last_of("/");
  std::string line = fileName.substr(found + 1, fileName.length() - 1 - found);

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
  outFile << "Used time: find Bounds for Lambda " << Info.preSolvetime
      << "sec\n";
  outFile << "           solve Problem " << Info.time << "sec\n";
  outFile << "           Summe " << Info.time + Info.preSolvetime << "sec\n";
  outFile << "Labeling:\n";
  for (unsigned int u = 0; u < C.size(); u++)
    outFile << "c[" << u + 1 << "]=" << C[u] << " ";

  outFile.close();
  return 0;
}

int
main(int argc, char **argv)
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

