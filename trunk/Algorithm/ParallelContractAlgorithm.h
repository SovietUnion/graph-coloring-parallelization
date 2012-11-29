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

    // Helper Function
    static void* colour_helper(void* c)
    {
       void** p = (void**) c;
       return  ((ParallelContractAlgorithm*)p[0])->colourSubGraph((void*) p[1]);
    }
    static void* conflict_helper(void* c)
    {
       void** p = (void**) c;
       return  ((ParallelContractAlgorithm*)p[0])->detectConflict(c);
    }

    // Find a free colour for vertex a
    unsigned int findFreeColour(int a, int colourNumber);

    // Colour a slice of the graph
    void* colourSubGraph(void* slice);
    void* detectConflict(void* arg);

};

#endif
