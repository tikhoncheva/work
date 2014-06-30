#ifndef PROBLEMDATA_H_
#define PROBLEMDATA_H_

#include<string>
#include<vector>

typedef std::vector<double> VectorDouble;
typedef std::vector<VectorDouble> MatrixDouble;

MatrixDouble hexagonalLattice(int n, int n1)
{
	MatrixDouble coord;
	/*
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
	 */
	return coord;
}

MatrixDouble triangularLattice(int n, int n1)
{
	MatrixDouble coord;
	/*
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
	 */
	return coord;
}

MatrixDouble squareLattice(int n)
{
	MatrixDouble coord;

	coord.resize(n);
	/*
	 for (int i = 0; i < sqrt(n); i++)
	 {
	 for (int j = 0; j < sqrt(n); j++)
	 {
	 coord[i * sqrt(n) + j].resize(2);

	 coord[i * sqrt(n) + j][0] = j;
	 coord[i * sqrt(n) + j][1] = i;
	 }
	 }
	 */
	return coord;
}

class CProblemData
{

private:
	int lattice; // lattice type, if we consider a lattice graph
	std::string fileName; // name of the file with distance matrix
	int readData();
	//void coordTOdist();
public:
	unsigned int n; // number of vertices
	//unsigned int diameter; // diameter of the graph
	unsigned int maxGrad; // maximal grade of a graph
	MatrixDouble distances; // distance matrix

	void print();
	const std::string getFileName()
	{
		unsigned found = fileName.find_last_of("/");

		if (found != 0)
			fileName = fileName.substr(found + 1,
					fileName.length() - 1 - found);

		return fileName;
	}

	CProblemData()
	{
		lattice = 0;
		maxGrad = 0;
		fileName = "dumm.txt";
	}

	CProblemData(std::string fileName_) :
			fileName(fileName_)
	{
		lattice = 0;
		readData();
	}

	CProblemData(int lattice_) :
			lattice(lattice_)
	{
		fileName = "WeWorkWithLatticeGraph.txt";
		readData();
	}

	~CProblemData()
	{
	}
	;
};

int CProblemData::readData()
{
	std::string line;
	std::fstream oFile;
	std::stringstream buf;

	switch (lattice)
	{
	case 0:
		oFile.open(fileName.c_str());
		if (!oFile.is_open())
		{
			std::cerr << "Cannot open file!\n";
			exit(1);
		}

		std::getline(oFile, line); //number of vertices and diameter
		buf << line;
		buf >> n;
		//buf >> diameter;
		buf >> maxGrad;
		buf.clear();

		std::getline(oFile, line);

		distances.resize(n);
		for (unsigned int i = 0; i < n; i++)
		{
			distances[i].resize(n);
			distances[i][i] = 0;

			std::getline(oFile, line);

			std::stringstream buf(line);
			int int_buf;

			for (unsigned int j = 0; j < i; j++)
			{
				buf >> int_buf;
				//distanceVektor.push_back(int_buf);
				distances[i][j] = int_buf;
				distances[j][i] = distances[i][j];
			}
			buf.clear(); //clear buf

		}

		oFile.close();
		break;

	case 1:
		n = 24;
		distances = hexagonalLattice(n, 2); // number of vertices, number of vertices in first line
		//diameter = 7;
		maxGrad = 3;
		fileName = "hexagonal_lattice.dat";
		break;

	case 2:
		n = 23;
		distances = triangularLattice(n, 5); // number of vertices, number of vertices in first line
		// diameter = 6;
		maxGrad = 6;
		fileName = "triangular_lattice.dat";
		break;

	case 3:
		n = 25;
		//diameter = 8;
		maxGrad = 4;
		distances = squareLattice(n);
		fileName = "square_lattice.dat";
		break;

	default:
		std::cerr << "Wrong lattice type\n";
		exit(-1);
	} //switch

	return 0;
}

void CProblemData::print()
{
	std::cout << "\n";
	std::cout << "FileName: " << fileName << std::endl;
	std::cout << "Graph (V,E) with |V|= " << n << " and maxGrade = " << maxGrad
			<< "\n";
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
}

#endif /* PROBLEMDATA_H_ */
