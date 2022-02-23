#pragma once

#ifndef KSCC_H
#define KSCC_H

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <iostream>
#include <algorithm>


template <class DIGRAPH>
class KSCC
{

public:
    typedef typename DIGRAPH::Vertex Vertex;
    typedef typename DIGRAPH::VertexSet VertexSet;


    KSCC(const DIGRAPH& G)
    {

        DIGRAPH R = G.reverse();
        std::list<Vertex> L;

        for (auto& v : G.V())
            L.push_front(v);

        dfs(R, L);


        dfs(G, _rpost);


    }


    std::unordered_map<Vertex, std::size_t> component() const
    {
        return _C;
    }

    std::size_t ncc() const
    {
        return std::max_element(_C.begin(), _C.end())->second;

    }


private:

    void dfs(const DIGRAPH& G, const std::list<Vertex>& L)
    {

        std::size_t _ncc(0);
        VertexSet _visited;
        for (auto& v : L)
            if (_visited.count(v) == 0)
            {
                ++_ncc;
                dfs_one(G, v, _visited, _ncc);
            }
    }

    void dfs_one(const DIGRAPH& G, const Vertex& v, VertexSet& _visited, std::size_t& _ncc)
    {
        _visited.insert(v);

        _C[v] = _ncc;
        for (auto& w : G.Adj(v))
            if (_visited.count(w) == 0)
                dfs_one(G, w, _visited, _ncc);

        _rpost.push_front(v);

    }



    std::list<Vertex> _rpost;
    std::unordered_map<Vertex, std::size_t> _C;


};


#endif // KSCC_H
