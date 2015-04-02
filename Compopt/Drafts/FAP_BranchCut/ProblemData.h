#ifndef PROBLEMDATA_H_
#define PROBLEMDATA_H_

#include<string>
#include<vector>
#include"var.h"

/*
 typedef std::vector<double> VectorDouble;
 typedef std::vector<VectorDouble> MatrixDouble;
 */

MatrixDouble hexagonalLattice(int n, int n1)
{
	MatrixDouble coord;
	return coord;
}

MatrixDouble triangularLattice(int n, int n1)
{
	MatrixDouble coord;
	return coord;
}

MatrixDouble squareLattice(int n, int n1)
{
	MatrixDouble coord;
	return coord;
}

MatrixDouble squareCoordToDist(MatrixDouble coord)
{
	// simply city block distance
	MatrixDouble distance;
	distance.resize(coord.size());

	for (unsigned int u; u < coord.size(); u++)
	{
		distance[u].resize(coord.size());
		for (unsigned int v; v < u; v++)
		{
			distance[u][v] = std::abs(coord[u][0] - coord[v][0])
					+ std::abs(coord[u][1] - coord[v][1]);
			//distance[v][u] = distance[u][v];
		}

		distance[u][u] = 0;
	}

	std::cout << "Distances: " << "\n";
	for (unsigned int u = 0; u < distance.size(); u++)
	{
		for (unsigned int v = 0; v < distance[u].size(); v++)
		{
			std::cout << std::setw(3) << distance[u][v] << " ";
		}
		std::cout << "\n";
	}

	return distance;
}

class CProblemData
{

private:
	int lattice; // lattice type, if we consider a lattice graph
	std::string fileName; // name of the file with distance matrix
	int readData();
	//void coordTOdist();
	int dist_linear(int x);
	void set_d_function();

public:
	unsigned int n; // number of vertices
	//unsigned int diameter; // diameter of the graph
	unsigned int maxGrad; // maximal grade of a graph
	MatrixDouble dist; // distance matrix

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

		dist.resize(n);
		for (unsigned int i = 0; i < n; i++)
		{
			dist[i].resize(n);
			dist[i][i] = 0;

			std::getline(oFile, line);

			std::stringstream buf(line);
			int int_buf;

			for (unsigned int j = 0; j < i; j++)
			{
				buf >> int_buf;
				//distanceVektor.push_back(int_buf);
				dist[i][j] = int_buf;
				dist[j][i] = dist[i][j];
			}
			buf.clear(); //clear buf

		}

		oFile.close();
		break;

	case 1:
		n = 24;
		dist = hexagonalLattice(n, 2); // number of vertices, number of vertices in first line
		//diameter = 7;
		maxGrad = 3;
		fileName = "hexagonal_lattice.dat";
		break;

	case 2:
		n = 23;
		dist = triangularLattice(n, 5); // number of vertices, number of vertices in first line
		// diameter = 6;
		maxGrad = 6;
		fileName = "triangular_lattice.dat";
		break;

	case 3:
		n = 25;
		//diameter = 8;
		maxGrad = 4;
		dist = squareCoordToDist(squareLattice(n, sqrt(n)));

		fileName = "square_lattice.dat";
		break;

	default:
		std::cerr << "Wrong lattice type\n";
		exit(-1);
	} //switch

	set_d_function();

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
			std::cout << std::setw(3) << dist[u][v] << " ";
		}
		std::cout << "\n";
	}
}

int CProblemData::dist_linear(int x)
{
	int value = 1 + max_dist - x;
	return (value < 0) ? 0 : value; /* linear function of dist*/
	//return value;
}

void CProblemData::set_d_function()
{
	d.resize(n);
	for (unsigned int u = 0; u < n; u++)
	{
		d[u].resize(n);
		for (unsigned int v = 0; v < u; v++)
		{
			d[u][v] = dist_linear(dist[u][v]);
			d[v][u] = d[u][v];

			if (dist[u][v] <= max_dist && u != v)
			{ // save vertices for which interferent with each other
				info.InterferenceVertices.push_back(u);
				info.InterferenceVertices.push_back(v);
			}

		}
	}

}

#endif /* PROBLEMDATA_H_ */
