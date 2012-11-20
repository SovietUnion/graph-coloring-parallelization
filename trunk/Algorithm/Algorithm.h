#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "../Graph/Graph.h"
#include "../Graph/MatrixGraph.h"

using namespace std;

class Algorithm {
protected:
    Graph* g_;

public:

    // Print out the results
    void printResults();

    // Colour the graph G and return number of colours used
    virtual int colourGraph() = 0;
};

#endif
