#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <cassert> // Remember, assert() accepts a condition and
				   // throws an error if that condition isn't met.
#include <cmath>
#include "BFS.h"

template <class GRAPH> class BFS;

template <class T>
struct Edge
{
	T a, b;

	Edge() {}

	Edge(const T& a, const T& b) : a(a), b(b)
	{}

};

/*
* This graph class represents graphs as an adjacency list.
* Each element is a tuple consisting of a vertex and all its edges
*/
template <class T>
class graph {

public:

	typedef T Vertex; // Vertices
	typedef std::unordered_multiset<T> VertexSet; // Edges unordered_multiset
	typedef std::vector<T> Path; // Path

	graph() {

	}

	/**
	* Return the number of vertices in the graph
	* NOTE: const prevents function from modifying any private vars
	*/
	size_t n() const {
		return _t.size();
	}
	size_t vertex_count() const {
		return n();
	}
	
	/**
	* Return number of edges in the graph
	* NOTE: With maps, each element is a pair (a, b)
	* .first references "a" and .second references "b"
	*/
	size_t m() const {
		std::size_t ans(0); // Alternate way to assign a variable ans = 0
		std::size_t self_loops(0), sl_current(0);

		// Sum up the size of all edge lists in _t
		for (auto p : _t) {

			// Check for self-loops
			sl_current = std::count(p.second.begin(), p.second.end(), p.first);
			self_loops += sl_current;
			
			ans += p.second.size() - sl_current;

		}

		// Each edge is double counted because
		// if there's an edge (a, b), we saw this edge while
		// going through both vertex "a" and vertex "b".
		// So we divide ans by two to get the number of edges.
		return ( (ans / 2) + self_loops );
	}
	size_t edge_count() const {
		return m();
	}

	// Return all vertices
	VertexSet V() const {
		VertexSet ans;

		for (auto p : _t)
			ans.insert(p.first);

		return ans;
	}
	VertexSet vertices() const {
		return V();
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
		return (isVertex(v) && isVertex(w) && _t.at(v).count(w) >= 1); // >= for self-loops
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

		// Since we now allow self-loops, prevent double counts:
		if (v == w)
			_t[v].insert(w);
		else {
			_t[v].insert(w);
			_t[w].insert(v);
		}
	}

	void removeEdge(const T& v, const T& w) {
		if ( !isEdge(v, w) )
			return;

		if (v == w) {
			const auto pos = _t[v].find(w);
			_t[v].erase(pos);
		}
		else {
			auto pos = _t[v].find(w);
			_t[v].erase(pos);

			pos = _t[w].find(v);
			_t[w].erase(pos);
		}
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

	// Clear graph of all nodes
	void clear() {
		_t.clear();
	}

	/*Vertex begin() {
		return *(_t.begin());
	}*/

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



protected:

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
			// Prevent double-counting. = is there so we count self-loops
			if (v <= w) {
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

template <class T>
bool operator <(const Edge<T>& e1,
	const Edge<T>& e2)
{

	return ((e1.a < e2.a) || ((e1.a == e2.a) && (e1.b < e2.b)));
}

template <class T>
std::ostream& operator << (std::ostream& os, const Edge<T>& e)
{
	os << "{" << e.a << ", " << e.b << "}";
	return os;
}

#endif // !GRAPH_H
