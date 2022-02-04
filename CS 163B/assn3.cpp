#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "graph.h"
#include "Eulerian.h"

// Filepath to test input
#define path "C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt"

using namespace std;

int main()
{
    std::ifstream is(path);

    graph<int> colors;

    // Number of test cases; number of beads in chain
    size_t rounds, beads;
    is >> rounds;

    size_t i, j, k;
    int a, b; // Placeholders for each bead

    // output
    std::string output = "";

    for (i = 0; i < rounds; i++) {

        is >> beads;

        for (j = 0; j < beads; j++) {
            // Get colors
            is >> a;
            is >> b;

            // Add to graph if they don't already exist
            colors.addVertex(a);
            colors.addVertex(b);

            // Add edge between them
            colors.addEdge(a, b);
        }
        colors.edge_count();
        
        //cout << colors << endl << "---" << endl;

        // Now that graph is built, find Eulerian path
        Eulerian<graph<int>> path_obj(colors);

        Eulerian<graph<int>>::EPath chain;
        chain = path_obj.ec();

        // Output
        output += "Case #" + to_string(i+1) + "\n";

        if (chain.size() > 0) {
            for (k = 0; k < chain.size(); k++) {
                output += to_string(chain[k].a) + " " + to_string(chain[k].b) + "\n";
            }
        }
        else {
            output += "some beads may be lost\n";
        }

        output += "\n";

        // Empty graph for reuse
        colors.clear();

    }

    cout << output;

    return 0;
}