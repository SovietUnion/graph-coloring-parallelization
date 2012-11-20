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

    // format ostream output to printout the graph matrix
    friend std::ostream& operator<<(ostream &os, const MatrixGraph &g);
    
    ============================================================
    following are some extra functions used for BSC(may be still open to revise)
    //Get all the degrees
    virtual void getAllDegree(vector<unsigned int> &A,vector<unsigned int> &AllDegree);
    
    //Sort the vertices according to non-increasing degrees
    virtual void SortbyDegree(vector<unsigned int> &A,vector<unsigned int> &AllDegree);
    
    //Get the set of free colors, which used but not present in neighbor of x
    virtual void getFreeColors(int x,vector<unsigned int> &U);
    
    //Get the number of used color
    virtual int getUsedColorNum(int i);
    
    //Remove color k
    virtual void removeColor(unsigned int k,vector<unsigned int> &U );
    
    //Sort the set of color(U) according to non-decreasing order
    virtual void sortU(vector<unsigned int> &U);
    
    //Uncolor a vertex
    virtual void unColour(int v);
    
    //Get the color of vertex with index x
    virtual unsigned int getColor(unsigned int x);
                                                ---edited by Yao
    =============================================================
    private: 
	unsigned int size;
	vector<vector<bool> > graph;

};



#endif
