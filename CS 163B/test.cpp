#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <limits>

#include "graph.h"
#include "BFS.h"
#include "network.h"
#include "dary.h"

#define FILEPATH "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt"

using namespace std;

typedef pair<int, double> DVERT;
double dijkstra(network<int> G, int src, int T) {
    // Consider point cloud edge case for d-ary heaps and Dijkstra
    size_t nc = G.m() / G.n();
    std::cout << nc << std::endl;

    dary_heap<DVERT> Q(nc);

    map<int, double> dist;
    //map<int, int> prev;

    dist[src] = 0;

    for (auto& v : G.vertices()) {

        DVERT c(v, 0);

        if (v != src) {
            c.second = numeric_limits<double>::infinity();
            dist[v] = numeric_limits<double>::infinity();
            //prev[v] = INT_MAX;
        }

        Q.push(c);
    }

    while (!Q.empty()) {
        DVERT min_vert = Q.min();
        int u = min_vert.first;
        Q.pop_min();

        if (u == T)
            return min_vert.second;

        for (auto& v : G.Adj(u)) {
            double alt = dist.at(u) + G.cost(u, v);

            if (alt < dist.at(v)) { // XXX
                double cost = dist.at(v);
                dist.at(v) = alt;
                //prev.at(v) = u;

                Q.decrease_key(DVERT(v, cost), DVERT(v, alt)); // XXX
            }
        }
    }

    return numeric_limits<double>::infinity();

}

int main()
{

    std::ifstream is(FILEPATH);

    int N;
    int n, m, S, T;

    int s, t;
    double w;

    is >> N;
    
    for (int i = 0; i < N; i++) {
        cout << "\nCase #" << (i+1) << ": \n";

        network<int> G;

        is >> n;
        is >> m;
        is >> S;
        is >> T;

        if (m == 0) {
            cout << "unreachable" << endl;
            continue;
        }

        for (int j = 0; j < n; j++) {
            G.addVertex(j);
        }
        for (int j = 0; j < m; j++) {
            is >> s;
            is >> t;
            is >> w;

            G.addEdge(s, t, w);
            G.addEdge(t, s, w);
        }

        int cost = dijkstra(G, S, T);
        cout << ( (cost == numeric_limits<double>::infinity()) ? "unreachable" : to_string(cost) ) << endl;
    }

}