// Pranav Rao
// Assignment 6

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <cassert> // Remember, assert() accepts a condition and
				   // throws an error if that condition isn't met.
#include <cmath>
#include <string>
#include <vector>

#include <stack>

/*************************************************************************************************************/
/****************                                 Graph                                     ******************/
/*************************************************************************************************************/

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

protected:

	// T is a vertex, VertexSet is an edge
	std::unordered_map<Vertex, VertexSet> _t;
};

/*************************************************************************************************************/
/****************                                   DFS                                     ******************/
/*************************************************************************************************************/

template <class GRAPH>
class DFS
{

public:
	typedef typename GRAPH::Vertex Vertex;

	DFS()
	{}

	DFS(const GRAPH& G)
	{
		_ncc = 0;
		std::size_t time = 0;
		_is_dag = true;

		for (auto& v : G.V())
		{
			if (_C.count(v) == 0)  // v has not been visited
			{
				std::unordered_map<Vertex, std::size_t> pre, post, low;
				std::unordered_map<Vertex, Vertex> parent;

				dfs_one(G, v, time, pre, post, low, parent);


				++_ncc;
				for (auto& p : pre)
					_C[p.first] = _ncc;

				// find bridges

				for (auto& p : parent)
				{
					if (low.at(p.first) == pre.at(p.first))
						_bridges[p.first] = p.second;
				}

				// find cut vertices

				for (auto& v : G.V())
				{
					// root of dfs tree
					if (parent.count(v) == 0)
					{
						int nchildren = 0;
						for (auto& p : parent)
							if (p.second == v && ++nchildren >= 2)
							{
								_cv.insert(v);
								break;
							}
					}
					else {
						for (auto& p : parent)
						{
							if (p.second == v && low.at(p.first) >= pre.at(v))
								_cv.insert(v);
						}
					}
				}

			}


		}
	}

	void dfs_one(const GRAPH& G,
		const Vertex& v,
		std::size_t& time,
		std::unordered_map<Vertex, std::size_t>& pre,
		std::unordered_map<Vertex, std::size_t>& post,
		std::unordered_map<Vertex, std::size_t>& low,
		std::unordered_map<Vertex, Vertex>& parent)
	{
		low[v] = pre[v] = ++time;
		for (auto& w : G.Adj(v))
		{
			if (pre.count(w) == 0)
			{
				parent[w] = v;
				dfs_one(G, w, time, pre, post, low, parent);
				low[v] = std::min(low.at(v), low.at(w));
			}
			else if (parent.count(v) == 0 || parent.at(v) != w) // proper back edge
			{
				low[v] = std::min(low.at(v), pre.at(w));
				_is_dag = false;
			}
		}
		post[v] = ++time;
		_ts.push_front(v);
	}

	std::unordered_map<Vertex, Vertex> bridges() const
	{
		return _bridges;
	}

	std::unordered_set<Vertex> cut_vertices() const
	{
		return _cv;
	}

	bool isDag() const
	{
		return _is_dag;
	}

	std::list<Vertex> ts() const
	{
		return _ts;
	}


private:
	std::size_t _ncc;
	std::unordered_map<Vertex, std::size_t> _C;
	std::unordered_map<Vertex, Vertex> _bridges;
	std::unordered_set<Vertex> _cv;
	bool _is_dag;
	std::list<Vertex> _ts;



};

/******************************************************************************************************/
/**************                                 Digraph                                     ***********/
/******************************************************************************************************/

template <class T> class DFS;

template <class T>
class digraph : public graph<T>
{

public:
	typedef T Vertex;
	typedef std::vector<T> Path; // Path

	digraph reverse() const
	{
		digraph ans;

		for (auto& v : graph<Vertex>::V())
			ans.addVertex(v);

		for (auto& v : graph<Vertex>::V())
		{
			for (auto& w : graph<Vertex>::Adj(v))
				ans.addEdge(w, v);
		}

		return ans;
	}

	std::size_t m() const
	{
		std::size_t ans(0);

		for (auto& v : graph<T>::V())
			ans += graph<T>::Adj(v).size();
		return ans;
	}

	std::size_t indeg(const Vertex& v) const
	{
		std::size_t ans(0);

		for (auto& w : graph<T>::V())
		{
			ans += graph<T>::Adj(w).count(v);
		}
		return ans;
	}

	std::size_t outdeg(const Vertex& v) const
	{
		return graph<T>::Adj(v).size();
	}

	void addEdge(const Vertex& v, const Vertex& w)
	{
		assert(graph<T>::isVertex(v) && graph<T>::isVertex(w));
		graph<T>::_t[v].insert(w);
	}

	void removeEdge(const Vertex& v, const Vertex& w)
	{
		if (!graph<T>::isEdge(v, w))
			return;

		graph<T>::_t[v].erase(w);
	}

	// Is acyclic
	bool isDag() const
	{
		DFS<digraph<Vertex>> D(*this);
		return D.isDag();
	}

	// Rudimentary "return source vertices" function. Doesn't work with self-loops and duplicate edges
	Path sources() {
		Path srcs;

		for (auto& v : graph<T>::V()) {
			if (indeg(v) == 0)
				srcs.push_back(v);
		}

		return srcs;
	}

	Path sinks() {
		Path srcs;

		for (auto& v : graph<T>::V()) {
			if (outdeg(v) == 0)
				srcs.push_back(v);
		}

		return srcs;
	}



private:

};

/******************************************************************************************************/
/**********                              TSCC                                           ***************/
/******************************************************************************************************/

template <class DIGRAPH>
class TSCC
{
public:

	typedef typename DIGRAPH::Vertex Vertex;
	typedef typename DIGRAPH::VertexSet VertexSet;

	TSCC(const DIGRAPH& G)
	{
		_time = _cid = 0;
		for (auto& v : G.V())
		{
			if (_pre.count(v) == 0)
				tdfs_one(G, v);
		}

		actual_ncc = _ncc(G);
		//for (auto& v : G.V())
		//    std::cout << v << ": " << _low.at(v) << ", " << _pre.at(v) << ", " << _post.at(v) << std::endl;

	}

	std::size_t nscc() const
	{
		return _cid;
	}

	std::size_t ncc() {
		return actual_ncc;
	}

	std::unordered_map<Vertex, std::size_t> component() const
	{
		return _C;
	}

private:

	void tdfs_one(const DIGRAPH& G, const Vertex& v)
	{
		_pre[v] = _low[v] = _time++;
		_S.push(v);

		for (auto& w : G.Adj(v))
		{
			if (_pre.count(w) == 0) // tree edge
			{
				tdfs_one(G, w);
				_low[v] = std::min(_low.at(v), _low.at(w));
			}

			// check to see if it is a back edge
			else if (_pre.at(w) < _pre.at(v) && _post.count(w) == 0)// back edge
				_low[v] = std::min(_low.at(v), _pre.at(w));
		}
		_post[v] = _time++;
		if (_pre.at(v) == _low.at(v))  // root of a component
		{
			Vertex top;

			do
			{
				top = _S.top();
				_S.pop();
				_C[top] = _cid;

			} while (top != v);
			++_cid;

		}
	}

	std::size_t _ncc(const DIGRAPH& G) {

		for (auto& v : G.vertices()) {
			condensed.addVertex(_C.at(v));

			for (auto& w : G.Adj(v)) {
				if (_C.at(w) != _C.at(v)) {
					condensed.addVertex(_C.at(w));
					condensed.addEdge(_C.at(w), _C.at(v));
				}
			}
		}

		return condensed.sinks().size();
	}


	std::unordered_map<Vertex, std::size_t> _pre, _post, _low, _C;
	std::size_t _time, _cid, actual_ncc;
	std::stack<Vertex> _S;

	digraph<std::size_t> condensed;

};

/**************************************************************************************************************/
/*******************                            Main                                              *************/
/**************************************************************************************************************/
int main() {

	//std::ifstream is(FILEPATH);

	std::size_t n;
	std::size_t x, y;
	std::size_t a, b; // a -> b

	digraph<int> seq;

	// Test Cases
	std::cin >> n;

	std::size_t i, j;

	std::string output = "";

	// Add vertices
	for (i = 0; i < n; i++) {

		seq.clear();

		// Vertices | Edges
		std::cin >> x;
		std::cin >> y;

		// Add vertices
		for (j = 1; j <= x; j++) {
			seq.addVertex(j);
		}

		// Add edges
		for (j = 0; j < y; j++) {
			std::cin >> a;
			std::cin >> b;

			seq.addEdge(a, b);
		}
		
		// Use Tarjan's algorithm to find SCCs, then build condensation graph
		TSCC<digraph<int>> obj(seq);
		// Flip condensation graphs, find number of sinks
		output += std::to_string(obj.ncc()) + "\n";
	}

	std::cout << output;

	return 0;
}