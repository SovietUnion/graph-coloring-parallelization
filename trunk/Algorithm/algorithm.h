#ifndef ALGORITHM_H
#define ALGORITHM_H

  #include <fstream>
  #include "../Graph/graph.h"

  using namespace std;

  class Algorithm {

   private:

    // Store the colours in an array
    int* colours;

   public:

    // Print out the results
    void printResults();

    // Colour the graph G
    virtual void addNeighbour(Graph g) = 0;
  }

#ENDIF
