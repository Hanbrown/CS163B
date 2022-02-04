#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//////////////////////////////////////////   Graph.h   ///////////////////////////////////////////////////

#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <cassert> // Remember, assert() accepts a condition and
				   // throws an error if that condition isn't met.
#include <cmath>

template <class GRAPH> class BFS;

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
		return _t.size();
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
		return ((ans / 2) + self_loops);
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
		VertexSet ans;

		for (auto p : _t)
			ans.insert(p.first);

		return ans;
	}

	std::unordered_map<Vertex, VertexSet> fn() const {
		return _t;
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
		if (!isEdge(v, w))
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

	Vertex begin() {
		return *(_t.begin());
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
	Path shortest_path(const Vertex& a, const Vertex& b) {

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

//////////////////////////////////////////   Eulerian.h   ///////////////////////////////////////////////////

#include <stack>

template <class GRAPH>
class Eulerian
{
public:

	typedef typename GRAPH::Vertex Vertex;
	typedef typename GRAPH::Path Path;

	struct node {
		Vertex a;
		Vertex b;
	};

	typedef typename std::vector<node> EPath;

	Eulerian(const GRAPH& G)
	{
		_has_cycle = true;

		for (auto& v : G.fn())
		{
			if (
				G.deg(v.first) & 1
				&&
				std::find(v.second.begin(), v.second.end(), v.first) == v.second.end()
				)
			{
				_has_cycle = false;
				break;
			}
		}
		_G = G;

	}

	EPath ec()
	{
		if (!_has_cycle)
			return EPath();

		std::vector<node> ans;

		std::stack<Vertex> S;

		Vertex v = *(_G.V().begin());   // first vertex in V()
		S.push(v);

		while (!S.empty())
		{
			Vertex s = S.top();
			if (_G.deg(s) != 0)
			{
				Vertex w = *(_G.Adj(s).begin());  // first neighbor
				_G.removeEdge(s, w);
				S.push(w);
			}
			else // back to s
			{
				S.pop();

				Vertex w;
				if (!S.empty()) {
					w = S.top();

					struct node tmp = { s, w };
					ans.push_back(tmp);
				}
			}
		}

		return ans;


	}
private:

	bool _has_cycle;  // true if the input graph has an Eulerian cycle
	GRAPH _G;
};

//////////////////////////////////////////   Main   ///////////////////////////////////////////////////

// Filepath to test input
#define path "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt"

using namespace std;

int main()
{
    //std::ifstream is(path);

    graph<int> colors;

    // Number of test cases; number of beads in chain
    size_t rounds, beads;
	cin >> rounds;

    size_t i, j, k;
    int a, b; // Placeholders for each bead

    // output
    std::string output = "";

    for (i = 0; i < rounds; i++) {

        cin >> beads;

        for (j = 0; j < beads; j++) {
            // Get colors
			cin >> a;
			cin >> b;

            // Add to graph if they don't already exist
            colors.addVertex(a);
            colors.addVertex(b);

            // Add edge between them
            colors.addEdge(a, b);
        }
        colors.edge_count();
        
        //cout << colors << endl << "---" << endl;

        // Now that graph is built, find Eulerian path
        Eulerian<graph<int>> path_obj(colors);

        Eulerian<graph<int>>::EPath chain;
        chain = path_obj.ec();

        // Output
        output += "Case #" + to_string(i+1) + "\n";

        if (chain.size() > 0) {
            for (k = 0; k < chain.size(); k++) {
                output += to_string(chain[k].a) + " " + to_string(chain[k].b) + "\n";
            }
        }
        else {
            output += "some beads may be lost\n";
        }

        output += "\n";

        // Empty graph for reuse
        colors.clear();

    }

    cout << output;

    return 0;
}