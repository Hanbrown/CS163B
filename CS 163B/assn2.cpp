#include <iostream>
#include <fstream>

#include "graph.h"

using namespace std;

int main()
{
    graph<int> wheels;
    int i;

    // Construct graph with all nodes
    for (i = 0; i < 10000; i++) {
        // E.g. current vertex is 9999
        wheels.addVertex(i);

        //******** Ones Place

        // Connect 9999 and 9998
        if (i % 10 != 0) {
            wheels.addEdge(i, i - 1);
        }
        
        // Connect 9999 and 9990
        if (i % 10 == 9) {
            wheels.addEdge(i, i - 9);
        }

        //******** Tens Place

        // Connect 9999 and 9989
        if ((i % 100 / 10) != 0) {
            wheels.addEdge(i, i - 10);
        }

        // Connect 9999 and 9909
        if ((i % 100 / 10) == 9) {
            wheels.addEdge(i, i - 90);
        }

        //******** Hundreds Place

        // Connect 9999 and 9899
        if ((i % 1000 / 100) != 0) {
            wheels.addEdge(i, i - 100);
        }

        // Connect 9999 and 9099
        if ((i % 1000 / 100) == 9) {
            wheels.addEdge(i, i - 900);
        }

        //******** Thousands Place

        // Connect 9999 and 8999
        if ((i / 1000) != 0) {
            wheels.addEdge(i, i - 1000);
        }

        // Connect 9999 and 0999
        if ((i / 1000) == 9) {
            wheels.addEdge(i, i - 9000);
        }
    }

    // Maintain original graph with all vertices
    //graph<int> wheels_og (wheels);

    // Now read input from file
    std::ifstream is("C:\\Users\\Pranav\\source\\repos\\CS163B\\CS 163B\\input\\test.txt");

    // How many times to do this
    int rounds;
    is >> rounds;

    bool solvable; // If origin/dest is a reject vertex, this graph is not allowed.

    for (int j = 0; j < rounds; j++) {

        solvable = true;

        // Read origin vertex from file
        int k, digit, origin = 0;
        for (k = 1000; k >= 1; k /= 10) {
            is >> digit;
            origin += digit * k;
        }

        // Read destination vertex from file
        int dest = 0;
        for (k = 1000; k >= 1; k /= 10) {
            is >> digit;
            dest += digit * k;
        }

        // Number of reject vertices
        int r, reject;
        is >> r;

        // Now remove vertices as specified
        for (int j = 0; j < r; j++) {
            reject = 0;

            // Construct the current reject number
            for (k = 1000; k >= 1; k /= 10) {
                is >> digit;
                reject += digit * k;
            }

            // Check if reject vertex is either origin or destination
            if (reject == origin || reject == dest) {
                solvable = false;
                break;
            }

            // Remove it
            wheels.removeVertex(reject);
        }

        graph<int>::Path route;
        // Get the shortest path, if this input is deemed solvable
        if (solvable) {
            route = wheels.shortest_path(origin, dest);
        }

        // If path exists, subtract one from size (because path includes start vertex)
        // If path does not exist, print -1
        int size = route.size() > 0 ? ( route.size() - 1 ) : -1;

        cout << size << endl;

        // Construct graph with all nodes
        for (i = 0; i < 10000; i++) {
            // E.g. current vertex is 9999
            wheels.addVertex(i);

            //******** Ones Place

            // Connect 9999 and 9998
            if (i % 10 != 0) {
                wheels.addEdge(i, i - 1);
            }

            // Connect 9999 and 9990
            if (i % 10 == 9) {
                wheels.addEdge(i, i - 9);
            }

            //******** Tens Place

            // Connect 9999 and 9989
            if ((i % 100 / 10) != 0) {
                wheels.addEdge(i, i - 10);
            }

            // Connect 9999 and 9909
            if ((i % 100 / 10) == 9) {
                wheels.addEdge(i, i - 90);
            }

            //******** Hundreds Place

            // Connect 9999 and 9899
            if ((i % 1000 / 100) != 0) {
                wheels.addEdge(i, i - 100);
            }

            // Connect 9999 and 9099
            if ((i % 1000 / 100) == 9) {
                wheels.addEdge(i, i - 900);
            }

            //******** Thousands Place

            // Connect 9999 and 8999
            if ((i / 1000) != 0) {
                wheels.addEdge(i, i - 1000);
            }

            // Connect 9999 and 0999
            if ((i / 1000) == 9) {
                wheels.addEdge(i, i - 9000);
            }
        }


    }
}