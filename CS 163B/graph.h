#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <cassert> // Remember, assert() accepts a condition and
				   // throws an error if that condition isn't met.
#include "BFS.h"

template <class GRAPH> class BFS;

/*
* This graph class represents graphs as an adjacency list.
* Each element is a tuple consisting of a vertex and all its edges
*/
template <class T>
class graph {

public:

	typedef T Vertex; // Vertices
	typedef std::unordered_set<T> VertexSet; // Edges
	typedef std::vector<T> Path; // Path

	graph<T>() {

	}

	/**
	* Return the number of vertices in the graph
	* NOTE: const prevents function from modifying any private vars
	*/
	size_t n() const {
		return _t.size();
	}
	size_t vertex_count() const {
		return _t.size();
	}
	
	/**
	* Return number of edges in the graph
	* NOTE: With maps, each element is a pair (a, b)
	* .first references "a" and .second references "b"
	*/
	size_t m() const {
		std::size_t ans(0); // Alternate way to assign a variable ans = 0

		// Sum up the size of all edge lists in _t
		for (auto p : _t)
			ans += p.second.size();

		// Each edge is double counted because
		// if there's an edge (a, b), we saw this edge while
		// going through both vertex "a" and vertex "b".
		// So we divide ans by two to get the number of edges.
		return ans / 2;
	}
	size_t edge_count() const {
		std::size_t ans(0);
		for (auto p : _t)
			ans += p.second.size();
		return ans / 2;
	}

	// Return all vertices
	VertexSet V() const {
		VertexSet ans;

		for (auto p : _t)
			ans.insert(p.first);

		return ans;
	}
	VertexSet vertices() const {
		VertexSet ans;

		for (auto p : _t)
			ans.insert(p.first);

		return ans;
	}

	// Is vertex v in this graph?
	bool isVertex(const T& v) const {
		return (_t.count(v) != 0);
	}

	/*
	* NOTE: With maps, [] is different from .at()
	* With [], if the element does not exist, it is created
	* With .at(), if the element does not exist, an error is thrown
	*/
	bool isEdge(const T& v, const T& w) const {
		return (isVertex(v) && isVertex(w) && _t.at(v).count(w) == 1);
	}

	// Return neighbors of v
	VertexSet Adj(const T& v) const {
		assert(isVertex(v));
		return _t.at(v);
	}

	std::size_t deg(const T& v) const {
		assert(isVertex(v));
		return _t.at(v).size();
	}

	void addVertex(const T& v) {
		if (isVertex(v))
			return;

		_t[v] = VertexSet();
	}

	void addEdge(const T& v, const T& w) {
		assert(isVertex(v) && isVertex(w));
		_t[v].insert(w);
		_t[w].insert(v);
	}

	void removeEdge(const T& v, const T& w) {
		if ( !isEdge(v, w) )
			return;

		_t[v].erase(w);
		_t[w].erase(v);
	}

	void removeVertex(const T& v) {
		if (!isVertex(v))
			return;

		// Remove linked edges
		for (auto nbr : Adj(v)) {
			removeEdge(v, nbr);
		}

		// Remove vertex
		_t.erase(v);
	}

	bool isConnected() const {
		BFS<graph> B(*this); // Construct a Breadth-first Search object out of this graph
		return (B.ncc() == 1); // If every island is linked together, the graph is connected.
	}

	std::size_t ncc() const {
		BFS<graph> B(*this); // Kind of a simpler version of the last function
		return B.ncc();
	}

	bool isAcyclic() const {
		// vertices == islands + edges
		return (n() == ncc() + m()); // There's a theorem for this
	}

	bool isTree() const {
		return (isConnected() && isAcyclic());
	}

	// Shortest path assignment (1/14/22)
	// Return a vector that shows the shortest path between two nodes in this graph
	Path shortest_path(const Vertex& a, const Vertex & b) {
		
		// This will contain the route, including the destination node
		Path route;

		// If either vertex doesn't exist, throw error.
		assert(isVertex(a) && isVertex(b));
		
		// Utility variable declarations
		std::unordered_map<T, Path> paths;
		std::unordered_map<T, T> parents;
		BFS<graph<T>> search_obj(*this);

		// Build BFS object
		search_obj.bfs_path(*this, a, paths, parents);

		// If no path exists, return an empty Path
		if (paths.find(b) == paths.end())
			return route;

		// Get the path we want and return
		route = paths.at(b);
		
		return route;

	}



private:

	// T is a vertex, VertexSet is an edge
	std::unordered_map<Vertex, VertexSet> _t;
};

// Overload output operator
// Takes in the output stream and the graph (don't modify the graph)
template <class T>
std::ostream& operator << (std::ostream& os, const graph<T>& G) {
	os << G.n() << " " << G.m() << std::endl; // Ouput num of vertices and num of edges

	// Output all vertices
	for (auto v : G.V())
		os << v << " ";

	os << std::endl;

	// For every vertex
	for (auto v : G.V()) {
		// Get neighbors
		for (auto w : G.Adj(v)) {
			// Prevent double-counting
			if (v < w) {
				os << v << " " << w << std::endl;
			}
		}
	}

	return os;
}

// Allows us to read in a graph from a file
template <class T>
std::istream& operator >> (std::istream& is, graph<T>& G) {

	std::size_t n, m;
	is >> n >> m;

	//G = graph<T>();

	T v, w;
	for (std::size_t i = 0; i < n; ++i) {
		is >> v;
		G.addVertex(v);
	}

	for (std::size_t i = 0; i < m; ++i) {
		is >> v >> w;
		G.addEdge(v, w);
	}

	return is;
}

#endif // !GRAPH_H
