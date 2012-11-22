#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H
#include "Graph.h"

class MatrixGraph: public Graph {

   public:
    // Constructor that generates a 2d graph
    MatrixGraph(unsigned int mSize);
    // Constructor that take in a pregenerated graph
    MatrixGraph(const vector<vector<bool> > &g, unsigned int mSize);
    // Return true if a and b are neighbours
    virtual bool isNeighbours(int a, int b);

    // Return an iterator that will iteratates all the neighbours of a
    // we could discuss more about this and come to agreement if you find this setup doesn't work
    virtual void neighbours(int a, vector<unsigned int> &neighbours);

    // Return a set of non-neighbours
    virtual void nonNeighbours(int a, vector<unsigned int> &nonNeighbours);

    // Add an edge between a and b
    virtual void addNeighbour(int a, int b);

    // Remove an edge between a and b
    virtual bool removeNeighbour(int a, int b);

    // Number of the common neighbours for a and b
    virtual int getCommonNeighboursCount(int a, int b);

    // Get the vertex degree
    virtual int getDegree(int vertex);

    // Get the vertex with Maximal degree
    virtual int getMaxDegreeVertex();
    
    // Get the vertext with Maximal degree of a particular set
    virtual int getMaxDegreeVertex(vector<unsigned int> & nn);
    
    // Get the vertex count in G
    virtual unsigned int getSize();

    // Contract a into b;
    virtual void contract(int a, int b);

    // Set color for a vertex
    virtual void setColour(int v, int colour);

    // get colors of graph
    virtual unsigned int* getColours();
    
    // reset colors;
    virtual void resetColours();

    // format ostream output to printout the graph matrix
    virtual ostream& print(ostream &os) const;
    private: 
	unsigned int size;
	vector<vector<bool> > graph;

};



#endif
