#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <cassert> // Remember, assert() accepts a condition and
				   // throws an error if that condition isn't met.

/*
* This graph class represents graphs as an adjacency list.
* Each element is a tuple consisting of a vertex and all its edges
*/
template <class T>
class graph {

public:

	typedef T Vertex; // Vertices
	typedef std::unordered_set<T> VertexSet; // Edges

	graph<T>() {

	}

	/**
	* Return the number of vertices in the graph
	* NOTE: const prevents function from modifying any private vars
	*/
	size_t n() const {
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

	VertexSet V() const {
		VertexSet ans;

		for (auto p : _t)
			ans.insert(p.first);

		return ans;
	}

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

	VertexSet Adj(const T& v) {
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

private:

	// T is a vertex, VertexSet is an edge
	std::unordered_map<T, VertexSet> _t;
};

#endif // !GRAPH_H
