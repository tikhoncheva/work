#ifndef PROBLEMDATA_H_
#define PROBLEMDATA_H_

#include<string>
#include<vector>

typedef std::vector<double> VectorDouble;
typedef std::vector<VectorDouble> MatrixDouble;

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
MatrixDouble hexagonalLattice2(int n, int n1)
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
MatrixDouble triangularLattice2(int n, int n1)
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
MatrixDouble squareLattice2(int n)
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

class CProblemData
{

private:
	int lattice; // lattice type, if we consider a lattice graph
	std::string fileName; // name of the file with distance matrix
	int readData();
	void coordTOdist();
public:
	unsigned int n; // number of vertices
	unsigned int diameter; // diameter of the graph

	MatrixDouble distances; // distance matrix
	MatrixDouble coordinates; // coordinates of the graph vertices

	void print();
	const std::string getFileName()
	{
		return fileName;
	}

	CProblemData()
	{
		lattice = 0;
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

void CProblemData::coordTOdist()
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

int CProblemData::readData()
{
	std::string line;
	std::fstream oFile;
	std::stringstream buf;

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

		std::getline(oFile, line); //number of vertices and diameter
		std::getline(oFile, line);
		buf << line;
		buf >> n;
		buf >> diameter;
		buf.clear();

		std::getline(oFile, line); // coordinates
		coordinates.resize(n);
		for (unsigned int i = 0; i < n; i++)
		{
			coordinates[i].resize(2);
			std::getline(oFile, line);
			buf << line;
			buf >> bufDouble; //because first number in line is the number of the vertex
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
		coordinates = hexagonalLattice(n, 2); // number of vertices, number of vertices in first line
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
		diameter = 8;
		coordinates = squareLattice(n);
		fileName = "square_lattice.dat";
		break;

	default:
		std::cerr << "Wrong lattice type\n";
		exit(-1);
	} //switch

	coordTOdist();

	return 0;
}

void CProblemData::print()
{
	std::cout << "\n";
	std::cout << "FileName: " << fileName << std::endl;
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
}

#endif /* PROBLEMDATA_H_ */
