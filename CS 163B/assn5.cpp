// Pranav Rao
// Assignment 5

#include <iostream>
#include <fstream>

#include "digraph.h"

#define FILEPATH "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt"

digraph<int>::Path pickup(digraph<int>& G, digraph<int>::Path& route) {


	if (G.vertex_count() == 1) {
		//route.push_back(G.begin());
		return route;
	}

	if (G.sources().size() == 0) {
		return route;
	}

	int src = G.sources()[0];

	route.push_back(src);
	G.removeVertex(src);

	pickup(G, route);

}

int main() {

	std::ifstream is(FILEPATH);

	int n, m; // Vertices | Edges
	int a, b; // a -> b

	digraph<int> pile;

	is >> n;
	is >> m;

	// Add vertices
	for (int i = 1; i <= n; i++) {
		pile.addVertex(i);
	}

	// Build edges
	for (int i = 0; i < m; i++) {
		is >> a;
		is >> b;
		pile.addEdge(a, b);
	}

	// Reads in 0 0
	is >> a;
	is >> b;

	std::cout << pile;

	digraph<int>::Path order;

	pickup(pile, order);

	std::cout << std::endl;
	for (std::size_t i = 0; i < order.size(); i++) {
		std::cout << order[i] << ", ";
	}
	std::cout << std::endl;

	return 0;
}