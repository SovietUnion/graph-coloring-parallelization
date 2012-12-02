#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "Graph2.h"
using namespace std;
class MatrixGraph: public Graph {

   public:
    // Constructor that take in a pregenerated graph
    MatrixGraph(const vector<vector<bool> > &g, int mSize);

	// Return true if a and b are neighbours
    virtual bool isNeighbours(int a, int b);
	
    // Add an edge between a and b
    virtual void addNeighbour(int a, int b);

    // Get the vertex degree
    virtual int getDegree(int vertex);
   
    // Get the vertex count in G
    virtual int getSize();

    // Set color for a vertex
    virtual void setColour(int v, int colour);

    // get colors of graph
    virtual int* getColours();

    // format ostream output to printout the graph matrix
    friend std::ostream& operator<<(ostream &os, const MatrixGraph &g);

	//Print out graph
	virtual void printGraph();
   
    //Get all the degrees
    virtual vector<int> getAllDegree(vector<int> &AllDegree);
   
    //Sort the vertices according to non-increasing degrees
    virtual vector<int> SortbyDegree(vector<int> &AllDegree);

	//Get the degree of saturation of vertex x
	virtual int getVertexDSATUR(int x);

	//Get the vertex with maximum degree of saturation
	virtual int getMaxDSATURvertex(vector<int> &A);
   
   //Get the set of free colours, which used but not present in neighbor of x
    virtual void getFreeColours(int x, set<int> &U);
   
    //Get the number of used colour
    virtual int getUsedColourNum(int i,vector<int> &U);
	
	//Get the larger one of two inputs
	virtual int getMax(int a, int b);
   
    //Remove colour k
    virtual void removeColour(int k,vector<int> &U);
   
    //Sort the set of colour(referred as U) according to non-decreasing order
    virtual void sortU(vector<int> &U);
   
    //Uncolour a vertex
    virtual void unColour(int x);
   
    //Get the colour of vertex with index x
    virtual int getColour(int x);

	// Return an vector containing the neighbours of a
    virtual vector<int> getNeighbours(int a);

    private:
        int size;
        vector<vector<bool> > graph;

};

#endif
