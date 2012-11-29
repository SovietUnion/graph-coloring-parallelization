#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "../Graph/Graph.h"
#include "../Graph/MatrixGraph.h"

using namespace std;

class Algorithm {
protected:
    Graph* g_;
    int threadcount_;

public:

    // Print out the results
    virtual void printResults() = 0;

    // Colour the graph G and return number of colours used
    virtual int colourGraph() = 0;

};

#endif

/**************************************/
//For BSC by Yao
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "Graph.h"
#include "MatrixGraph.h"

using namespace std;

class Algorithm {
protected:
    Graph* g_;
	int *colours;
	int num_of_nodes;

public:
	//Constructor
	Algorithm(int num_nodes, Graph* g);
	Algorithm(){};
    // Print out the results
    virtual void printResults() = 0;

    // Colour the graph G and return number of colours used
    virtual int colourGraph() = 0;
	//Destructor
	Algorithm::~Algorithm();
};

#endif
