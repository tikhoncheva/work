/*
 * Bron Kerbosch Algorithm
 *
 * Algorithm (classic formulation) for finding a maximal complete subgraph in
 * the given graph
 *
 */

void extendClique(std::vector<std::vector<int> > G, std::set<int> & C,
		std::set<int> Cand, std::set<int> Not)
{
	/*
	 std::cout << "extend clique Cand: ";
	 for (std::set<int>::iterator itCand = Cand.begin(); itCand != Cand.end();
	 itCand++)
	 {
	 std::cout << *itCand << " ";
	 }

	 std::cout << "  Not:";

	 for (std::set<int>::iterator itNot = Not.begin(); itNot != Not.end();
	 itNot++)
	 {
	 std::cout << *itNot << " ";
	 }

	 std::cout << "\n";
	 */
	/*
	 int adjToAllN = 0;

	 for (std::set<int>::iterator itNot = Not.begin(); itNot != Not.end();
	 itNot++)
	 {
	 adjToAllN = 1;
	 int ne = *itNot; // Not Element

	 for (std::set<int>::iterator itCand = Cand.begin();
	 itCand != Cand.end(); itCand++)
	 {
	 int ce = *itCand; // Candidat Element
	 if (G[ne][ce] != max_dist)
	 {
	 adjToAllN = 0;
	 break;
	 }
	 }
	 }

	 if (adjToAllN)
	 {
	 // there is no new maximal clique
	 std::cout << "there is no new maximal clique \n";
	 return;
	 }
	 else
	 */
	{
		// for all candidates

		std::set<int>::iterator itCand;

		for (itCand = Cand.begin(); itCand != Cand.end(); itCand++)
		{
			std::set<int> newCand; // new set of vertices to extend the clique
			std::set<int> newNot; // new set of vertices that cannot be in clique

			std::cout << "\n";

			int ce = *itCand;
			std::cout << "ce = " << ce << "\n";

			// delete selected candidate from Cand
			Cand.erase(ce);
			// extend existed clique C
			C.insert(ce);

			// new set of candidates (neighbors of selected vertex ce)
			std::set<int>::iterator itCand2;
			for (itCand2 = Cand.begin(); itCand2 != Cand.end(); itCand2++)
			{
				int N_ce = *itCand2; // neighbor of ce in Cand
				if (G[ce][N_ce] == max_dist && ce != N_ce)
				{
					newCand.insert(N_ce);
				}
			}
			// new set Not
			std::set<int>::iterator itNot;
			for (itNot = Not.begin(); itNot != Not.end(); itNot++)
			{
				int ne = *itNot; // neighbor of ce in Not
				if (G[ce][ne] == max_dist)
				{
					newNot.insert(ne);
				}
			}

			//
			if (newCand.empty() && newNot.empty())
			// there is no extension candidates AND
			// clique is not a part of other clique
			{
				// we found maximal clique
				std::cout << "we found maximal clique : ";

				for (std::set<int>::iterator itC = C.begin(); itC != C.end();
						itC++)
				{
					std::cout << *itC << "<->";
				}
				std::cout << "|" << std::endl;
				//return;

			}
			else
			{
				// extend clique to maximal
				extendClique(G, C, newCand, newNot);
			}

			// backtracking
			std::cout << "Backtracking" << std::endl;
			std::cout << "ce = " << ce << "\n";

			C.erase(ce);
			Not.insert(ce);

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
	extendClique(G, C, Cand, Not);
	cliqueList.push_back(C);

	return cliqueList;
}
