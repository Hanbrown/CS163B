#ifndef BFS_H_
#define BFS_H_

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

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

#endif