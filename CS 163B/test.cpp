#include <iostream>
#include <string>

#include "graph.h"
#include "BFS.h"

using namespace std;

int main()
{
    int N(10);

    // L is a path of length N
    graph<string> L;

    // Build Graph
    for (int i = 1; i <= N; ++i)
        L.addVertex(to_string(i));

    for (int i = 1; i < N; ++i)
        L.addEdge(to_string(i), to_string(i + 1));

    // Test
    cout << "BFS example: " << endl;

    unordered_map<string, size_t> D;
    unordered_map<string, string> P;
    BFS<graph<string>> B(L);
    B.bfs_one(L, "1", D, P);

    for (auto& p : D)
        cout << p.first << ": " << p.second << endl;

    for (auto& p : P)
        cout << p.first << ": " << p.second << endl;

    int dst = L.shortest_path("3", "9");
    cout << dst;

}