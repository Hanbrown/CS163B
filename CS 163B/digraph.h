#pragma once

#ifndef DIGRAPH_H
#define DIGRAPH_H

#include "graph.h"
#include "DFS.h"

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

template <class T>
std::ostream& operator <<(std::ostream& os, const digraph<T>& G)
{
    os << G.n() << " " << G.m() << std::endl;
    for (auto v : G.V())
        os << v << " ";
    os << std::endl;

    for (auto v : G.V())
        for (auto w : G.Adj(v))
            os << v << " " << w << std::endl;
    return os;
}


template <class T>
std::istream& operator >>(std::istream& is, digraph<T>& G)
{
    std::size_t n, m;
    is >> n >> m;

    G = digraph<T>();

    T v, w;
    for (std::size_t i = 0; i < n; ++i)
    {
        is >> v;
        G.addVertex(v);
    }

    for (std::size_t i = 0; i < m; ++i)
    {
        is >> v >> w;
        G.addEdge(v, w);
    }

    return is;
}

#endif // DIGRAPH_H
