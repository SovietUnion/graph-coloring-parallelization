#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <stdlib.h>
#include "../Graph/Graph.h"
#include "../Graph/MatrixGraph.h"
#include "GraphChecker.h"

using namespace std;

// Check if Graph g with colouring scheme c is valid or not
bool GraphChecker::checkGraph(Graph* g, int* c, const int n) {

  vector<unsigned int> neigh;

  // go through all nodes and their neighbours to make sure no 2 neigbhours' colours are the same
  for ( int i = 0; i < n; i++ ) {
    g->neighbours(i,neigh);
    for (int j = 0; j < neigh.size(); j++) {
      if (c[i] == c[neigh[j]]) {
       cout << "Node " << i << " and Node " << neigh[j] << "both are colour " << c[i] << endl;
       return false;
      }
    }
    neigh.clear();
  } 

  return true;

}

// Takes in 2 inputs
// First input (n, an integer) specifies the number of nodes
// Second input (d, a decimal number from 0 - 1) specifies the graph densities
// Output: Display a message if the given graph is incorrect
int main(int argc, char* argv[]) { 

   ifstream graph(argv[1]);
   ifstream colour(argv[2]);

   int* c; 
   Graph* g; 

   int numNodes = 0;
   int numEdges = 0;

   // read in the graph
   if ( graph.is_open() ) {

     graph >> numNodes;
     graph >> numEdges;

     g = new MatrixGraph(numNodes);

     string line;

     while ( graph.good() ) {
       getline ( graph, line );
       stringstream ss(line);
       int a,b;
       
       // Read in the node      
       ss >> a;

       // Read in the neighbours 
       while ( ss.good() ) {
         ss >> b;
         g->addNeighbour(a,b);
       }
     }
 
     graph.close();

   } else {
     printf("Can't open file %s\n", argv[1]);
     return 4;
   }

   // read in the colours
   c = new int[numNodes];
   if ( colour.is_open() ) {

     while ( colour.good() ) {
      int a,b;
      colour >> a >> b;
      c[a] = b;
     }

     colour.close();

   } else {
     printf("Can't open file %s\n", argv[2]);
     return 4;
   }

   if (!GraphChecker::checkGraph(g,c,numNodes)) {
      delete g;
      cout << "FAIL" << endl;
      return 1;
   }

   delete g;
 
   cout << "PASS" << endl;
   return 0; 
}
