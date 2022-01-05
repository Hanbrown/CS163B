#include <iostream>
#include "graph.h"

int main() {
	graph<int> G;

	for (int i = 0; i < 4; i++) {
		G.addVertex(i);
	}

	G.addEdge(0, 1);
	G.addEdge(1, 2);
	G.addEdge(2, 3);
	G.addEdge(3, 0);
	G.addEdge(0, 2);

	std::cout << G.n() << std::endl;
	std::cout << G.m() << std::endl;
	std::cout << G.deg(0) << ", " << G.deg(1) << std::endl;

	for (auto v : G.V())
		std::cout << v << " ";
	std::cout << std::endl;

	G.removeEdge(0, 2);

	for (auto v : G.Adj(0))
		std::cout << v << " ";
	std::cout << std::endl;

	std::cout << G.m() << std::endl;

	std::cout << G.n() << std::endl;
	std::cout << G.m() << std::endl;
	std::cout << G.deg(0) <<", "<< G.deg(1) << std::endl;

	return 0;
}