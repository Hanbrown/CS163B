// Pranav Rao
// Assignment 7

#include <iostream>
#include <fstream>

#include <string>

#include "wgraph.h"

#define FILEPATH "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt"

// The central line that can go to any of the buildings has this identifier
#define CENTRAL "@CENTRAL"

/**
* Find minimum spanning tree of _g using Kruskal's Algorithm
* Return the minimum spanning tree as a graph object
* Print the sum of weights in the MST
*/
wgraph<std::string> mst( wgraph<std::string>& _g ) {

	wgraph<std::string> ans;
	double cost = 0.0f;

	// Populate ans with vertices from _g
	for (auto v : _g.vertices())
		ans.addVertex(v);

	// Add edges, ensuring there are no cycles
	// Note that edges are sorted by length
	for (auto e : _g.E()) {

		// Using BFS.h to prevent cycles
		// If there is no path between the two, add an edge and update cost
		if (ans.shortest_path(e.a, e.b).size() == 0) {
			ans.addEdge(e.a, e.b, e.w);
			cost += e.w;
		}
	}

	// Print cost (sum of all the edge weights in this MST)
	std::cout << cost << std::endl;

	return ans;
}

/**
* Print the MST as per requirements
* Note that cost should already have been printed by this point in mst()
*/
void print_results(wgraph<std::string> _g) {

	bool direct_connect;
	graph<std::string>::VertexSet nbrs;

	for (auto v : _g.vertices()) {

		// Skip CENTRAL vertex
		if (v != CENTRAL) {

			nbrs = _g.Adj(v); // Find neighboring vertices
			direct_connect = nbrs.find(CENTRAL) != nbrs.end(); // Is there a link to CENTRAL vertex?

			std::cout << v << "  direct_connect " << (direct_connect ? "true" : "false") << " ";

			// Print all neighbors except for CENTRAL (if it is a neighbor)
			for (auto w : nbrs) {
				if (w != CENTRAL)
					std::cout << w << " ";
			}

			std::cout << std::endl;
		}
	}

}

int main() {

	std::ifstream is(FILEPATH);

	wgraph<std::string> uni;

	for (int j = 0; j < 4; j++) {

		uni.addVertex(CENTRAL);

		// Add verts/edges from central point to nodes
		int i, n, m, weight;
		std::string label;

		is >> n;
		for (i = 0; i < n; i++) {
			is >> label;
			is >> weight;

			uni.addVertex(label);

			if (weight >= 0)
				uni.addEdge(CENTRAL, label, weight);
		}

		// Add edges between nodes (not central point)
		std::string src, dst;

		is >> m;
		for (i = 0; i < m; i++) {
			is >> src;
			is >> dst;
			is >> weight;

			if (weight >= 0)
				uni.addEdge(src, dst, weight);
		}

		print_results(mst(uni));

		std::cout << std::endl;

		uni.clear();
	}

	return 0;
}