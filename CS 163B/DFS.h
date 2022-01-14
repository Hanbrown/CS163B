#pragma once
#ifndef DFS_H
#define DFS_H

#include <iostream>
#include <unordered_map>

template <class GRAPH>
class DFS
{

public:
    typedef typename GRAPH::Vertex Vertex;

    DFS(const GRAPH& G)
    {
        _ncc = 0;
        std::size_t time = 0;

        for (auto& v : G.V())
        {
            if (_C.count(v) == 0)  // v has not been visited
            {
                std::unordered_map<Vertex, std::size_t> pre, post;
                std::unordered_map<Vertex, Vertex> parent;

                dfs_one(G, v, time, pre, post, parent);
                std::cout << "dfs_one result: " << std::endl;
                std::cout << "pre" << std::endl;
                for (auto& p : pre)
                    std::cout << p.first << ", " << p.second << std::endl;

                std::cout << "post" << std::endl;
                for (auto& p : post)
                    std::cout << p.first << ", " << p.second << std::endl;

                std::cout << "parent" << std::endl;
                for (auto& p : parent)
                    std::cout << p.first << ", " << p.second << std::endl;



                ++_ncc;
                for (auto& p : pre)
                    _C[p.first] = _ncc;
            }
        }
    }

    void dfs_one(const GRAPH& G,
        const Vertex& v,
        std::size_t& time,
        std::unordered_map<Vertex, std::size_t>& pre,
        std::unordered_map<Vertex, std::size_t>& post,
        std::unordered_map<Vertex, Vertex>& parent)
    {
        pre[v] = ++time;
        for (auto& w : G.Adj(v))
        {
            if (pre.count(w) == 0)
            {
                parent[w] = v;
                dfs_one(G, w, time, pre, post, parent);

            }
        }
        post[v] = ++time;
    }



private:
    std::size_t _ncc;
    std::unordered_map<Vertex, std::size_t> _C;



};

#endif // !DFS_H
