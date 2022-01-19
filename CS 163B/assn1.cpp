#include <iostream>
#include <string>

#include "graph.h"
#include "BFS.h"
#include "Doublet.h"

using namespace std;

int main()
{
    // All graphs will have 10 vertices
    int N(10);

    // G is a 10-vertex clique
    graph<string> G;

        // Build Graph
    for (int i = 1; i <= N; ++i)
        G.addVertex(to_string(i));
    
    for (int i = 1; i < N; ++i)
        for (int j = i + 1; j <= N; ++j)
            G.addEdge(to_string(i), to_string(j));
    
        // Test
    for (int i = 1; i < N; ++i)
        for (int j = i + 1; j <= N; ++j)
        {
            graph<string>::Path p = G.shortest_path(to_string(i), to_string(j));
            assert(p.size() == 2);
        }
    
    // H is a 10-vertex point cloud (no edges)
    graph<string> H;
    
        // Build Graph
    for (int i = 1; i <= N; ++i)
        H.addVertex(to_string(i));
    
        // Test
    for (int i = 1; i < N; ++i)
        for (int j = i + 1; j <= N; ++j)
        {
            graph<string>::Path p = H.shortest_path(to_string(i), to_string(j));
            assert(p.size() == 0);
        }
    
    // K is a 9-branch tree with height = 1
    graph<string> K;
    
        // Build Graph
    for (int i = 1; i <= N; ++i)
        K.addVertex(to_string(i));
    
    for (int i = 2; i <= N; ++i)
        K.addEdge(to_string(1), to_string(i));
    
        // Test
    for (int i = 1; i < N; ++i)
        for (int j = i + 1; j <= N; ++j)
        {
            graph<string>::Path p = K.shortest_path(to_string(i), to_string(j));
            if (i == 1)
                assert(p.size() == 2);
            else
                assert(p.size() == 3);
        }
    
    // L is a path of length N
    graph<string> L;
    
        // Build Graph
    for (int i = 1; i <= N; ++i)
        L.addVertex(to_string(i));
    
    for (int i = 1; i < N; ++i)
        L.addEdge(to_string(i), to_string(i + 1));
    
        // Test
    for (int i = 1; i < N; ++i)
        for (int j = i + 1; j <= N; ++j)
        {
            graph<string>::Path p = L.shortest_path(to_string(i), to_string(j));
            assert(p.size() == j - i + 1);
        }
    
    
    cout << "Passed!" << endl;

    ////////////////////////////////////  Part Two   ////////////////////////////////

    Doublet puzzle;

    graph<string>::Path connection = puzzle.find_path("amigo", "signs");
    
    // Print Results
    cout << "[";
    for (auto word : connection) {
        cout << word << ", ";
    }
    cout << "]" << endl;

    return 0;
}