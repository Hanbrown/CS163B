// Pranav Rao
// Assignment 9

#include <iostream>
#include <fstream>

#include <string>
#include <math.h>

#include "network.h"

#define FILEPATH "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\"

// Return a vector of a path from node i to node j (return empty path if we exceed length len)
std::vector<int> make_path(std::size_t** next, std::size_t i, std::size_t j, std::size_t len) {

	std::vector<int> cycle;

	std::size_t ind = 0, k = i;

	//std::cout << "[" << i;
	cycle.push_back(i);

	while (j != next[k][j]) {
		if (ind > len)
			return std::vector<int>();

		//std::cout << " " << next[k][j];
		cycle.push_back(next[k][j]);
		k = next[k][j];
		ind++;
	}

	return cycle;
	//std::cout << "]";
}

// Print an mxn matrix
template <class T>
void print_matrix(T** mat, std::size_t m, std::size_t n) {

	for (std::size_t i = 0; i < m; i++) {
		for (std::size_t j = 0; j < n; j++) {
			std::cout << mat[i][j] << "\t\t";
		}
		std::cout << std::endl;
	}

}

// Use Floyd-Warshall (Dynamic Programming) to find shortest paths between all vertices
std::vector<int> fwdp(network<int> G) {

	//  Store number of vertices in a variable for readibility
	std::size_t len = G.n();

	// Dist is a matrix that contains the weights. Next helps with pathfinding.
	// Scroll to the pathfinding function to see how it is used.
	double** dist = new double* [len];
	std::size_t** next = new std::size_t * [len];


	// Populate the dist and next matrices with default values
	// in accordance with Floyd-Warshall Algorithm
	for (std::size_t i = 0; i < len; i++) {
		dist[i] = new double[len];
		next[i] = new std::size_t[len];

		dist[i][i] = 0;
		next[i][i] = i;

		for (std::size_t j = 0; j < len; j++) {
			if (j != i) {

				// If an edge exists between vertices i and j
				if (G.isEdge(i, j)) {
					dist[i][j] = G.cost(i, j);
					next[i][j] = j;
				}
				// If i and j are not directly reachable (requires a stopover)
				else {
					dist[i][j] = INFINITY;
					next[i][j] = NULL;
				}
			}
		}
	}

	/*print_matrix(dist, len, len);
	std::cout << std::endl << std::endl;
	print_matrix(next, len, len);
	std::cout << std::endl << std::endl;*/

	// Now correctly populate the matrices
	for (std::size_t k = 0; k < len; k++) {
		for (std::size_t i = 0; i < len; i++) {

			if (k != i) {
				for (std::size_t j = 0; j < len; j++) {

					// See if current route is more expensive than a route via vertex k
					if (dist[i][j] > dist[i][k] + dist[k][j]) {

						dist[i][j] = dist[i][k] + dist[k][j];
						next[i][j] = next[i][k]; // Says where to go AFTER getting to k

						/*std::cout << k << std::endl;
						print_matrix(dist, len, len);
						std::cout << std::endl << std::endl;
						print_matrix(next, len, len);
						std::cout << std::endl << std::endl;*/
					}
				}
			}
		}
	}

	//print_matrix(dist, len, len);

	// Return a money-making cycle
	for (std::size_t i = 0; i < len; i++) {
		std::vector<int> cycle = make_path(next, i, i, len);
		if (cycle.size() > 0) {
			return cycle;
		}
	}

	return std::vector<int>();

}

std::vector<int> arbitrage(std::ifstream& is) {

	// Declare network
	network<int> xe;

	std::size_t n;
	double rate;

	is >> n;

	// Add vertices
	for (std::size_t i = 0; i < n; i++)
		xe.addVertex(i);

	// Add edges
	for (std::size_t i = 0; i < n; i++) {
		for (std::size_t j = 0; j < n; j++) {

			is >> rate;

			// Use logarithms because log(a*b*c) = log(a)+log(b)+log(c)
			// Helps us with multiplication in the end
			// Multiply by -1 so larger weights are prioritised over smaller weights
			if (rate != 1.0)
				xe.addEdge(i, j, (-1 * std::log(rate)));

		}
	}

	// Use Floyd-Warshall to find negative cycles
	return fwdp(xe);

}

int main() {

	std::string names[]{ "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	for (auto n : names)
	{
		std::ifstream is(FILEPATH + n); // Altered
		if (is.fail())
		{
			std::cout << "Can't read!" << std::endl;
			exit(1);
		}
		// NOTE: currency names are 0, 1, 2, ..., n-1
		std::vector<int> cycle = arbitrage(is);
		for (auto e : cycle)
			std::cout << e << " ";
		std::cout << std::endl;
	}

	
	
	return 0;
}