#include <iostream>
#include "graph.h"
#include <string>
#include <fstream>
#include "BFS.h"
#include "DFS.h"

using namespace std;

int main()
{
    graph<int> G;

    for (int i = 0; i < 4; ++i)
        G.addVertex(i);

    G.addEdge(0, 1);
    G.addEdge(1, 2);
    G.addEdge(2, 3);
    G.addEdge(3, 0);
    G.addEdge(0, 2);

    cout << G.n() << endl;
    cout << G.m() << endl;
    cout << G.deg(0) << ", " << G.deg(1) << endl;

    for (auto v : G.V())
        cout << v << " ";
    cout << endl;

    G.removeEdge(0, 2);

    for (auto v : G.Adj(0))
        cout << v << " ";
    cout << endl;

    cout << G.m() << endl;


    cout << "Graph g: " << endl;
    cout << G << endl;




    ifstream is("C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\cyclic_connected.txt");
    if (is.fail())
        exit(1);


    graph<string> H;

    cout << "Enter H: ";
    is >> H;

    cout << "Read: " << endl;
    cout << H << endl;


    cout << "is connected ? : " << H.isConnected() << endl;
    cout << "is acyclic ? : " << H.isAcyclic() << endl;

    cout << "ncc = " << H.ncc() << endl;



    cout << "BFS example: " << endl;

    ifstream is2("C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\bfs-ex.txt");
    if (is2.fail())
        exit(1);


    graph<string> cities;
    is2 >> cities;

    unordered_map<string, size_t> D;
    unordered_map<string, string> P;
    BFS<graph<string>> B(cities);
    B.bfs_one(cities, "Frankfurt", D, P);

    for (auto& p : D)
        cout << p.first << ": " << p.second << endl;

    for (auto& p : P)
        cout << p.first << ": " << p.second << endl;

    graph<string>::Path dst = cities.shortest_path("Nurnberg", "Erfurt");
    //cout << dst << endl;

    cout << "[";
    for (size_t k = 0; k < dst.size(); k++) {
        cout << dst[k] << ", ";
    }
    cout << "]";

    cout << endl << endl;


    cout << "DFS example: " << endl;

    ifstream is3("C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\dfs-ex.txt");
    if (is3.fail())
        exit(1);

    graph<string> X;
    is3 >> X;
    cout << X << endl;
    DFS<graph<string>> Y(X);




    return 0;
}
