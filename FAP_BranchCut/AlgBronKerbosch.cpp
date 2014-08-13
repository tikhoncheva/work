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
		// we found maximal clique C
		listC.push_back(C);
	}
	else
	{
		// If there is an element in Not, that is connected to all points
		// in Cand, the extension of current clique will not lead to maximal
		// subgraph => return

		for (std::set<int>::iterator itNot = Not.begin(); itNot != Not.end();
				itNot++)
		{

			int ne = *itNot; //
			int accept = 0;
			for (std::set<int>::iterator itCand = Cand.begin();
					itCand != Cand.end(); itCand++)
			{
				int ce = *itCand;

				if (G[ce][ne] < max_dist)
				{
					accept = 1;
					break;
				}
			}

			if (!accept)
			{
				return;
			}
		}

		//	else
		// try out all candidates in Cand
		for (std::set<int>::iterator itCand = Cand.begin();
				itCand != Cand.end(); itCand++)
		{
			int ce = *itCand; // new candidate

			std::set<int> newCand; // new set of vertices to extend the clique
			std::set<int> newNot; // new set of vertices that cannot be in clique

			// extend existed clique C
			C.insert(ce);

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

			enumirateCliques1(G, C, newCand, newNot, listC);

			// delete selected candidate ce from Cand and clique C
			Cand.erase(ce);
			C.erase(ce);
			// and add it to set Not
			Not.insert(ce);

		}
	}
}

// select new candidate to be add in existed clique C as a vertex from Cand
// with maximal number of cinnections to other vertices in Cand
int findNewCandidate(const std::vector<std::vector<int> > G, std::set<int> Cand)
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

		if (N_ce1 >= N_v)
		{
			v = ce1;
			N_v = N_ce1;
		}
	}
	return v;
}

void enumirateCliques2(const std::vector<std::vector<int> > G, std::set<int> C,
		std::set<int> Cand, std::set<int> Not,
		std::vector<std::set<int> >& listC)
{
	if (Cand.empty() && Not.empty())
	// If there is no extension candidates AND
	// clique is not a part of other clique
	{
		// we found maximal clique
		listC.push_back(C);
	}

	else
	{
		int ce = findNewCandidate(G, Cand); // select first candidate as
		// a vertex with maximum conntection to vetrices in Cand

		while (ce >= 0)
		{
			std::set<int> newCand; // new set of vertices to extend the clique
			std::set<int> newNot; // new set of vertices that cannot be in clique

			// extend existed clique C
			C.insert(ce);

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

			enumirateCliques1(G, C, newCand, newNot, listC);

			// delete selected candidate ce from Cand and clique C
			Cand.erase(ce);
			C.erase(ce);
			// and add it to set Not
			Not.insert(ce);

			ce = findNewCandidate(G, Cand); // new candidate
		}
	}
}

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
	//enumirateCliques1(G, C, Cand, Not, cliqueList);

	enumirateCliques2(G, C, Cand, Not, cliqueList);

	return cliqueList;
}
