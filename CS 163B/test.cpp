#include <iostream>
#include <string>

#include "graph.h"
#include "BFS.h"
#include "network.h"
#include "dary.h"

using namespace std;

int main()
{
    
    network<int> G;
    // Consider point cloud edge case for d-ary heaps and Dijkstra
    dary_heap<int> Q(G.edge_count() / G.vertex_count());


    cout << G << endl;

}