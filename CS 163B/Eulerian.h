#pragma once
#ifndef EULERIAN_H
#define EULERIAN_H

#include <stack>
#include <algorithm>

template <class GRAPH>
class Eulerian
{
public:

    typedef typename GRAPH::Vertex Vertex;
    typedef typename GRAPH::Path Path;

    struct node {
        Vertex a;
        Vertex b;
    };

    typedef typename std::vector<node> EPath;

    Eulerian(const GRAPH& G)
    {
        _has_cycle = true;

        for (auto& v : G.fn())
        {
            if (
                G.deg(v.first) & 1 // Odd degree
                &&
                std::find(v.second.begin(), v.second.end(), v.first) == v.second.end() // No self-loops
                )
            {
                _has_cycle = false;
                break;
            }
        }
        _G = G;

    }

    EPath ec()
    {
        if (!_has_cycle)
            return EPath(); // Empty path

        std::vector<node> ans;

        std::stack<Vertex> S;

        Vertex v = *(_G.V().begin());   // first vertex in V()
        S.push(v);

        while (!S.empty())
        {
            Vertex s = S.top();
            if (_G.deg(s) != 0)
            {
                Vertex w = *(_G.Adj(s).begin());  // first neighbor
                _G.removeEdge(s, w);
                S.push(w);
            }
            else // back to s
            {
                S.pop();

                Vertex w;
                if (!S.empty()) {
                    w = S.top();

                    struct node tmp = { s, w };
                    ans.push_back(tmp);
                }
            }
        }

        return ans;


    }
private:

    bool _has_cycle;  // true if the input graph has an Eulerian cycle
    GRAPH _G;
};

#endif // EULERIAN_H
