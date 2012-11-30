//BSCAlgorithm.h
#ifndef BSCALGORITHM_H
#define BSCALGORITHM_H

#include "Algorithm2.h"
using namespace std;

class BSCAlgorithm : public Algorithm{
public:
    // Constructor
    BSCAlgorithm(Graph* g);

    // Print out the results
    virtual void printResults();

    // Colour the graph G and return number of colours used
    virtual int colourGraph();
};

#endif
