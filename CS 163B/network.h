#pragma once
// A weighted directed graph
#ifndef NETWORK_H
#define NETWORK_H

#include "wgraph.h"
#include "digraph.h"

template <class T>
class network : public digraph<T>
{
public:
	network() {
		// Empty
	}

	void addEdge(const T a, const T b, double w) {
		digraph<T>::addEdge(a, b);
		_w[{a, b}] = w; // Remember, [] also initializes
	}

	double cost(const T& a, const T& b) const {
		assert(digraph<T>::isEdge(a, b));
		return _w.at({ a, b });
	}

	void setCost(const T& a, const T& b, double w) {
		assert(digraph<T>::isEdge(a, b));
		_w[{a, b}] = w;
	}

	// Get set of edges
	std::set<WEdge<T>> E() const {
		std::set<WEdge<T>> ans;

		for (auto& a : digraph<T>::V())
			for (auto& b : digraph<T>::Adj(a))
				ans.insert({ a, b, cost(a, b) });
	}

private:
	// 
	std::map<WEdge<T>, double> _w;
};

template <class T>
std::ostream& operator << (std::ostream& os, const network<T>& G) {
	os << G.n() << " " << G.m() << std::endl;

	// Print vertices
	for (auto& v : G.V())
		os << v << " ";

	os << std::endl;

	// Print Edges
	for (auto& v : G.V())
		for (auto& w : G.Adj(v))
			os << v << " " << w << " " << G.cost(v, w) << std::endl;

	return os;
}

#endif // NETWORK_H