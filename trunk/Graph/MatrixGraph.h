
#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H
#include "graph.h"

class MatrixGraph: public Graph {


   public:

    // Constructor that generates a 2d graph
    MatrixGraph(int mSize);
    // Constructor that take in a pregenerated graph
    MatrixGraph(vector<vector<int> > g, int mSize);
    // Return true if a and b are neighbours
    virtual bool isNeighbours(int a, int b);

    // Return an iterator that will iteratates all the neighbours of a
    // we could discuss more about this and come to agreement if you find this setup doesn't work
    virtual vector<int> neighbours(int a);

    // Add an edge between a and b
    virtual void addNeighbour(int a, int b);

    // Remove an edge between a and b
    virtual bool removeNeighbour(int a, int b);

    // format ostream output to printout the graph matrix
    friend std::ostream& operator<<(ostream &os, const MatrixGraph &g);

    private:
	vector<vector<int> > graph;
	int size;

};



#endif
