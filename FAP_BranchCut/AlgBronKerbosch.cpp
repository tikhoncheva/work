/*
 * Bron Kerbosch Algorithm
 *
 * Algorithm (classic formulation) for finding a maximal complete subgraph in
 * the given graph
 *
 */

void enumirateCliques1(const std::vector<std::vector<int> > G,
		std::set<int> & C, std::set<int> Cand, std::set<int> Not,
		std::vector<std::set<int> >& listC)
{

	if (Cand.empty() && Not.empty())
	// If there is no extension candidates AND
	// clique is not a part of other clique
	{
		// we found maximal clique
		std::cout << "we found maximal clique : ";

		for (std::set<int>::iterator itC = C.begin(); itC != C.end(); itC++)
		{
			std::cout << *itC << "<->";
		}
		std::cout << "|" << std::endl;
		listC.push_back(C);
	}
	else
	{
		for (std::set<int>::iterator itCand = Cand.begin();
				itCand != Cand.end(); itCand++)
		{
			int ce = *itCand;

			std::cout << "\n";
			std::cout << "ce = " << ce << "\n";

			int accept = 1;
			for (std::set<int>::iterator itNot = Not.begin();
					itNot != Not.end(); itNot++)
			{
				int ne = *itNot; // neighbor of ce in Cand
				if (G[ce][ne] < max_dist)
				{
					accept = 0;
					break;
				}
			}

			if (!accept)
			{
				return;
			}
			else
			{
				std::set<int> newCand; // new set of vertices to extend the clique
				std::set<int> newNot; // new set of vertices that cannot be in clique

				// extend existed clique C
				Cand.erase(ce);
				C.insert(ce);

				// new set of candidates (neighbors of selected vertex ce)
				for (std::set<int>::iterator itCand2 = Cand.begin();
						itCand2 != Cand.end(); itCand2++)
				{
					int N_ce = *itCand2; // neighbor of ce in Cand
					if (G[ce][N_ce] == max_dist)
					{
						newCand.insert(N_ce);
					}
				}
				// new set Not
				for (std::set<int>::iterator itNot = Not.begin();
						itNot != Not.end(); itNot++)
				{
					int ne = *itNot; // neighbor of ce in Not
					if (G[ce][ne] == max_dist)
					{
						newNot.insert(ne);
					}
				}

				enumirateCliques1(G, C, newCand, newNot, listC);

				// delete selected candidate from Cand
				//Cand.erase(ce);
				Not.insert(ce);
				C.erase(ce);
			}
		}

	}
}

/*
 int find_v(const std::vector<std::vector<int> > G, std::set<int> Cand)
 {
 int v = -1;
 int N_v = 0;
 for (std::set<int>::iterator itCand1 = Cand.begin(); itCand1 != Cand.end();
 itCand1++)
 {
 int ce1 = *itCand1;
 int N_ce1 = 0;
 for (std::set<int>::iterator itCand2 = Cand.begin();
 itCand2 != Cand.end(); itCand2++)
 {
 int ce2 = *itCand2;
 if (G[ce1][ce2] == max_dist)
 {
 N_ce1++;
 }
 }

 if (N_ce1 > N_v)
 {
 v = ce1;
 N_v = N_ce1;
 }
 }
 return v;
 }

 void enumerateCliques(const std::vector<std::vector<int> > G, std::set<int> & C,
 std::set<int> Cand, std::set<int> Not,
 std::vector<std::set<int> >& listC)
 {
 std::cout << "begin enumerate\n";
 if (Cand.empty() && Not.empty())
 {
 // we found maximal clique
 std::cout << "we found maximal clique : ";

 for (std::set<int>::iterator itC = C.begin(); itC != C.end(); itC++)
 {
 std::cout << *itC << "<->";
 }
 std::cout << "|" << std::endl;
 listC.push_back(C);
 }

 else
 {
 int v = find_v(G, Cand);
 //std::cout << "v = " << v << std::endl;
 while (v >= 0)
 {
 std::cout << "v = " << v << std::endl;
 std::cout << "Cand: ";
 for (std::set<int>::iterator itCand = Cand.begin();
 itCand != Cand.end(); itCand++)
 {
 std::cout << *itCand << " ";
 }

 std::cout << "  Not:";

 for (std::set<int>::iterator itNot = Not.begin();
 itNot != Not.end(); itNot++)
 {
 std::cout << *itNot << " ";
 }

 std::set<int> newCand; // new set of vertices to extend the clique
 std::set<int> newNot; // new set of vertices that cannot be in clique

 // extend existed clique C
 C.insert(v);
 Cand.erase(v);

 // new set of candidates (neighbors of selected vertex ce)
 for (std::set<int>::iterator itCand = Cand.begin();
 itCand != Cand.end(); itCand++)
 {
 int N_v = *itCand; // neighbor of ce in Cand
 if (G[v][N_v] == max_dist)
 {
 newCand.insert(N_v);
 }
 }
 // new set Not
 for (std::set<int>::iterator itNot = Not.begin();
 itNot != Not.end(); itNot++)
 {
 int ne = *itNot; // neighbor of ce in Not
 if (G[v][ne] == max_dist)
 {
 newNot.insert(ne);
 }
 }

 enumerateCliques(G, C, newCand, newNot, listC);

 // delete selected candidate from Cand
 //Cand.erase(v);
 Not.insert(v);
 //C.erase(ce);


 for (std::set<int>::iterator itCand = Cand.begin();
 itCand != Cand.end(); itCand++)
 {
 int w = *itCand; // neighbor of ce in Cand
 if (G[v][w] < max_dist && v != w)
 {
 v = w;
 break;
 }
 else
 {
 v = -1;
 }
 }
 //v = find_v(G, C);
 std::cout << "new v = " << v << std::endl;

 }
 }

 }
 */

std::vector<std::set<int> > findMaxClique(std::vector<std::vector<int> > G)
{
	int n = G.size(); // number of vertices in input graph

	std::set<int> Cand; // a set of vertices that are connected to all vertices
// in C and can be added to C to make a larger clique

	std::set<int> Not; // a set of vertices that are connected to all vertices
// in C can not be added to C (empty)

	std::vector<std::set<int> > cliqueList; // maximal clique

// add all vertices ob G in N
	for (int u = 0; u < n; u++)
	{
		Cand.insert(u);
	}
	std::set<int> C;

// Recursion
	enumirateCliques1(G, C, Cand, Not, cliqueList);

//enumerateCliques(G, C, Cand, Not, cliqueList);

	return cliqueList;
}
