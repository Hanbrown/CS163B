#pragma once
#ifndef TSCC_H
#define TSCC_H

#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "digraph.h"

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

#endif // TSCC_H
