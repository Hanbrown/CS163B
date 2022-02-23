#ifndef DFS_H
#define DFS_H
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>


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


#endif // DFS_H
