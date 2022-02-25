#pragma once
#ifndef WGRAPH_H
#define WGRAPH_H
#include "graph.h"
#include <set>
#include <map>

template <class T>
struct WEdge : public Edge<T>
{
    double w;

    WEdge();

    WEdge(const T& a, const T& b, double w) : Edge<T>(a, b), w(w)
    {}
};

template <class T>
bool operator < (const WEdge<T>& e1, const WEdge<T>& e2)
{
    return ((e1.w < e2.w) ||
        ((e1.w == e2.w) && (Edge<T>(e1) < Edge<T>(e2))));
}


template <class T>
std::ostream& operator << (std::ostream& os, const WEdge<T>& e)
{
    os << "(" << e.a << ", " << e.b << ", " << e.w << ")";
    return os;
}

template <class T>
class wgraph : public graph<T>
{
public:

    typedef T Vertex;

    wgraph()
    {

    }

    void addEdge(const T& a, const T& b, double w)
    {
        graph<T>::addEdge(a, b);
        _w[Edge<T>(a, b)] = _w[Edge<T>(b, a)] = w;
    }

    void addEdge(const Edge<T>& e, double w)
    {
        addEdge(e.a, e.b, w);
    }

    void removeEdge(const T& a, const T& b)
    {
        graph<T>::removeEdge(a, b);
        _w.erase(Edge<T>(a, b));
        _w.erase(Edge<T>(b, a));
    }

    double cost(const T& a, const T& b) const
    {
        return _w.at(Edge<T>(a, b));

    }

    double cost(const Edge<T>& e) const
    {
        return cost(e.a, e.b);
    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;

        for (auto& v : graph<T>::V())
            for (auto& w : graph<T>::Adj(v))
                ans.insert(WEdge<T>(v, w, cost(v, w)));

        return ans;

    }

private:

    std::map<Edge<T>, double> _w;   // maps an edge to its weight



};

template <class T>
std::ostream& operator << (std::ostream& os, const wgraph<T>& G)
{

    os << G.n() << " " << G.m() << std::endl;
    for (auto& v : G.V())
        os << v << " ";

    os << std::endl;

    /*
    for (auto & v: G.V())
        for (auto &w: G.Adj(v))
            if (v < w)
                os << v << " " << w << " " << G.cost(v, w) <<  std::endl;
                */

    for (auto& e : G.E())
        if (e.a < e.b)
            os << e << std::endl;

    return os;

}

template <class T>
std::istream& operator >> (std::istream& is, wgraph<T>& G)
{
    G = wgraph<T>();


    std::size_t n, m;
    is >> n >> m;
    T v, w;
    double c;

    for (std::size_t i = 0; i < n; ++i)
    {
        is >> v;
        G.addVertex(v);

    }

    for (std::size_t i = 0; i < m; ++i)
    {
        is >> v >> w >> c;
        G.addEdge(v, w, c);
    }

    return is;
}

#endif // WGRAPH_H