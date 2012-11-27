#ifndef PARALLELCONTRACTALGORITHM_H
#define PARALLELCONTRACTALGORITHM_H
#include "Algorithm.h"

class ParallelContractAlgorithm : public Algorithm {
public:

    // Constructor
    ParallelContractAlgorithm(Graph* g, int threadcount);

    // Print out the results
    virtual void printResults();

    // Colour the graph G and return number of colours used
    virtual int colourGraph();

protected:

    // Helper class
    static void* colour_helper(void* c)
    {
       pair<ParallelContractAlgorithm*,int>* p = (pair<ParallelContractAlgorithm*,int>*) c;
       return  p->first->colourSubGraph((void*) p->second);
    }

    // Colour a slice of the graph
    void* colourSubGraph(void* slice);

};

#endif
