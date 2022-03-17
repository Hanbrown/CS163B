
#ifndef NETWORK_H
#define NETWORK_H

#include "wgraph.h"
#include "digraph.h"
#include <map>
#include <set>
#include <cassert>
#include <limits>


template <class T>
class network : public digraph<T>
{

public:

    typedef T Vertex;
    network()
    {

    }

    void addEdge(const T& a, const T& b, double w)
    {
        if (this->isEdge(a, b)) {

            if (this->cost(a, b) < w) {
                this->removeEdge(a, b);
                digraph<T>::addEdge(a, b);
                _w[{a, b}] = w;
            }
        }

        else {
            digraph<T>::addEdge(a, b);
            _w[{a, b}] = w;
        }
    }

    void addEdge(const WEdge<T>& e)
    {
        digraph<T>::addEdge(e.a, e.b);
        _w[{e.a, e.b}] = e.w;
    }

    double cost(const T& a, const T& b) const
    {
        assert(digraph<T>::isEdge(a, b));
        return _w.at({ a, b });
    }

    void setCost(const T& a, const T& b, double w)
    {
        assert(digraph<T>::isEdge(a, b));
        _w[{a, b}] = w;

    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;

        for (auto& a : digraph<T>::V())
            for (auto& b : digraph<T>::Adj(a))
                ans.insert({ a, b, cost(a, b) });
    }


    network<Vertex> Bellman_Ford(const Vertex& s) const
    {

        network<Vertex> ans;

        std::unordered_map<Vertex, double> d, D;  // old and new distances from s to the other vertices

        std::unordered_map<Vertex, Vertex> parent;

        bool nwc(false);   // negative-weight cycle

        for (auto& v : digraph<Vertex>::V())
        {
            d[v] = std::numeric_limits<double>::infinity();
        }
        d[s] = 0.0;

        for (std::size_t i = 1; i <= digraph<Vertex>::n(); ++i)
        {

            D = d;
            for (auto& e : E())
            {
                double temp = d[e.a] + e.w;

                if (temp < D[e.b])
                {
                    D[e.b] = temp;
                    parent[e.b] = e.a;

                    if (i == digraph<Vertex>::n())
                        nwc = true;
                }
            }
            d = D;
        }

        for (auto& v : digraph<Vertex>::V())
            ans.digraph<Vertex>::addVertex(v);

        if (!nwc)
            for (auto& p : parent)
                ans.addEdge(p.second, p.first, cost(p.second, p.first));

        return ans;


    }

private:
    std::map<Edge<T>, double> _w;

};

template <class T>
std::ostream& operator << (std::ostream& os, const network<T>& G)
{
    os << G.n() << " " << G.m() << std::endl;
    for (auto& v : G.V())
        os << v << " ";
    os << std::endl;
    for (auto& v : G.V())
        for (auto& w : G.Adj(v))
            os << v << " " << w << " " << G.cost(v, w) << std::endl;

    return os;
}

template <class T>
std::istream& operator >> (std::istream& is, network<T>& G)
{
    std::size_t n, m;
    T v, w;
    double c;

    G = network<T>();

    is >> n >> m;
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



#endif // NETWORK_H
