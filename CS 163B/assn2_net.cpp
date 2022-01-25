#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cassert>

/////////////////////////////////////  Graph  ///////////////////////////////////////////

template <class GRAPH>

class BFS {
public:

	typedef typename GRAPH::Vertex Vertex;

	BFS(const GRAPH& G) {
		_ncc = 0; // We will add to this now

		// For every vertex in graph G
		for (auto v : G.V()) {
			if (_C.count(v) == 0)   // unvisited
			{
				std::unordered_map<Vertex, std::size_t> D;
				std::unordered_map<Vertex, Vertex> P;

				bfs_one(G, v, D, P);
				++_ncc;
				for (auto p : D)
					_C[p.first] = _ncc;

			}
		}
	}

	std::size_t ncc() const
	{
		return _ncc;
	}

	std::unordered_map<Vertex, std::size_t> component() const
	{
		return _C;
	}

	bool has_path_between(const Vertex& a, const Vertex& b) const
	{
		return (_C.at(a) == _C.at(b));
	}

	void bfs_one(GRAPH G,
		const Vertex v,                                // start vertex
		std::unordered_map<Vertex, std::size_t>& D,   // distance from start vertex
		std::unordered_map<Vertex, Vertex>& P)        // parent in path back to start vertex
	{
		std::queue<Vertex> Q;

		P[v] = v;
		D[v] = 0;
		Q.push(v);

		while (!Q.empty())
		{
			Vertex f = Q.front();
			Q.pop();

			for (auto w : G.Adj(f))
				if (D.count(w) == 0)  // not visited
				{
					D[w] = 1 + D[f];
					P[w] = f;
					Q.push(w);
				}
		}
	}

	// Shortest path assignment (1/14/22)
	// A modified version of bfs_one to find shortest paths
	void bfs_path(GRAPH G,
		const Vertex v,                                // start vertex
		std::unordered_map<Vertex, std::vector<Vertex>>& paths,   // Path from start vertex to dest vertex
		std::unordered_map<Vertex, Vertex>& P)        // parent in path back to start vertex
	{
		std::queue<Vertex> Q;

		P[v] = v;
		paths[v] = std::vector<Vertex>(); // Instead of distances, log vectors (Paths)
		paths[v].push_back(v); // Initialize with current vertex
		Q.push(v);

		while (!Q.empty())
		{
			Vertex f = Q.front();
			Q.pop();

			for (auto w : G.Adj(f))
				if (paths.count(w) == 0)  // not visited
				{
					paths[w] = paths[f]; // Instead of adding one, push the current vertex to back
					paths[w].push_back(w);
					P[w] = f;
					Q.push(w);
				}
		}
	}

private:

	std::size_t _ncc; // Number of connected components/islands
	std::unordered_map<typename GRAPH::Vertex, std::size_t> _C;
	/**
	* About the above variable:
	* Each element is a tuple with a sort of index and a Vertex.
	* If a vertex has an index i, it belongs to the ith island
	* This helps us find out what vertices belong to what islands
	*/
};

////////////////////////////////////  BFS Class /////////////////////////////////////////

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
		if (!isEdge(v, w))
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

///////////////////////////////////  Main  /////////////////////////////////////////////

using namespace std;

int main()
{
	graph<int> wheels;
	int i;

	// Construct graph with all nodes
	for (i = 0; i < 10000; i++) {
		// E.g. current vertex is 9999
		wheels.addVertex(i);

		//******** Ones Place

		// Connect 9999 and 9998
		if (i % 10 != 0) {
			wheels.addEdge(i, i - 1);
		}

		// Connect 9999 and 9990
		if (i % 10 == 9) {
			wheels.addEdge(i, i - 9);
		}

		//******** Tens Place

		// Connect 9999 and 9989
		if ((i % 100 / 10) != 0) {
			wheels.addEdge(i, i - 10);
		}

		// Connect 9999 and 9909
		if ((i % 100 / 10) == 9) {
			wheels.addEdge(i, i - 90);
		}

		//******** Hundreds Place

		// Connect 9999 and 9899
		if ((i % 1000 / 100) != 0) {
			wheels.addEdge(i, i - 100);
		}

		// Connect 9999 and 9099
		if ((i % 1000 / 100) == 9) {
			wheels.addEdge(i, i - 900);
		}

		//******** Thousands Place

		// Connect 9999 and 8999
		if ((i / 1000) != 0) {
			wheels.addEdge(i, i - 1000);
		}

		// Connect 9999 and 0999
		if ((i / 1000) == 9) {
			wheels.addEdge(i, i - 9000);
		}
	}

	// How many instances of the problem to run (In the example, it's 2)
	int rounds;
	cin >> rounds;

	bool solvable; // If origin or dest are a reject vertex, this graph is not allowed.

	// Vector for storing output
	vector<int> output;

	for (int j = 0; j < rounds; j++) {

		solvable = true;

		// Read origin vertex from file
		int k, digit, origin = 0;
		for (k = 1000; k >= 1; k /= 10) {
			cin >> digit;
			origin += digit * k;
		}

		// Read destination vertex from file
		int dest = 0;
		for (k = 1000; k >= 1; k /= 10) {
			cin >> digit;
			dest += digit * k;
		}

		// Number of reject vertices, current reject vertex
		int r, reject;
		cin >> r;

		// Now remove vertices as specified
		for (int j = 0; j < r; j++) {
			reject = 0;

			// Construct the current reject number
			for (k = 1000; k >= 1; k /= 10) {
				cin >> digit;
				reject += digit * k;
			}

			// Check if reject vertex is either origin or destination
			if (reject == origin || reject == dest) {
				solvable = false;
				break;
			}

			// Remove it
			wheels.removeVertex(reject);
		}

		graph<int>::Path route;
		// Get the shortest path, if this input is deemed solvable
		if (solvable) {
			route = wheels.shortest_path(origin, dest);
		}

		// If path exists, subtract one from size (because path includes start vertex)
		// If path does not exist, print -1
		int size = route.size() > 0 ? (route.size() - 1) : -1;

		output.push_back(size);

		// Construct graph with all nodes
		for (i = 0; i < 10000; i++) {
			// E.g. current vertex is 9999
			wheels.addVertex(i);

			//******** Ones Place

			// Connect 9999 and 9998
			if (i % 10 != 0) {
				wheels.addEdge(i, i - 1);
			}

			// Connect 9999 and 9990
			if (i % 10 == 9) {
				wheels.addEdge(i, i - 9);
			}

			//******** Tens Place

			// Connect 9999 and 9989
			if ((i % 100 / 10) != 0) {
				wheels.addEdge(i, i - 10);
			}

			// Connect 9999 and 9909
			if ((i % 100 / 10) == 9) {
				wheels.addEdge(i, i - 90);
			}

			//******** Hundreds Place

			// Connect 9999 and 9899
			if ((i % 1000 / 100) != 0) {
				wheels.addEdge(i, i - 100);
			}

			// Connect 9999 and 9099
			if ((i % 1000 / 100) == 9) {
				wheels.addEdge(i, i - 900);
			}

			//******** Thousands Place

			// Connect 9999 and 8999
			if ((i / 1000) != 0) {
				wheels.addEdge(i, i - 1000);
			}

			// Connect 9999 and 0999
			if ((i / 1000) == 9) {
				wheels.addEdge(i, i - 9000);
			}
		}


	}

	for (i = 0; i < output.size(); i++) {
		cout << output[i] << endl;
	}
}