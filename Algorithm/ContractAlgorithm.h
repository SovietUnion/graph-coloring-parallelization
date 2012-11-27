#ifndef CONTRACTALGORITHM_H
#define CONTRACTALGORITHM_H
#include "Algorithm.h"

class ContractAlgorithm : public Algorithm {
public:

    // Constructor
    ContractAlgorithm(Graph* g, int threadcount);

    // Print out the results
    virtual void printResults();

    // Colour the graph G and return number of colours used
    virtual int colourGraph();

};

#endif
