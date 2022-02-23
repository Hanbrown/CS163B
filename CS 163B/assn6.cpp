// Pranav Rao
// Assignment 6

#include <iostream>
#include <fstream>

#include "digraph.h"
#include "TSCC.h"

#define FILEPATH "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt"

int main() {

	std::ifstream is(FILEPATH);

	std::size_t n;
	std::size_t x, y;
	std::size_t a, b; // a -> b

	digraph<int> seq;

	// Test Cases
	is >> n;

	std::size_t i, j;

	// Each test case
	for (i = 0; i < n; i++) {

		seq.clear();

		// Vertices | Edges
		is >> x;
		is >> y;

		// Add vertices
		for (j = 1; j <= x; j++) {
			seq.addVertex(j);
		}

		// Add edges
		for (j = 0; j < y; j++) {
			is >> a;
			is >> b;

			seq.addEdge(a, b);
		}

		
		TSCC<digraph<int>> obj(seq);
		std::cout << obj.ncc();
		
	}

	return 0;
}