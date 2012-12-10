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
	
	// Return an iterator that will iteratates all the neighbours of a
    virtual void neighbours(int a, vector<int> &neighbours);

    // Add an edge between a and b
    virtual void addNeighbour(int a, int b);

    // Get the vertex degree
    virtual int getDegree(int vertex);

	//Get the vertex with MAX degree
	virtual int getMaxDegreeVertex();
   
    // Get the vertex count in G
    virtual int getSize();

    // Set color for a vertex
    virtual void setColour(int v, int colour);

    // get colors of graph
    virtual int* getColours();

    // format ostream output to printout the graph matrix
    friend std::ostream& operator<<(ostream &os, const MatrixGraph &g);

	//Print out graph
	virtual void printGraph();;

	//Decide which vertex to color next
    virtual void getNextVertex(vector<int> &A,vector<pair <int,int>> &B);

	//Get the degree of saturation of vertex x
	virtual int getVertexDSATUR(int x);

    //Get the set of free colours, which used but not present in neighbor of x
    virtual void getFreeColours(int x,set<int> &U,int l);
   
    //Get the number of used colour
    virtual int getUsedColourNum(int i,vector<int> &U);
	
	//Get the larger one of two inputs
	virtual int getMax(int a, int b);
   
    //Remove colour k
    virtual void removeColour(int k,set<int> &U);
   
    //Uncolour a vertex
    virtual void unColour(int x);
   
    //Get the colour of vertex with index x
    virtual int getColour(int x);

	// Return an vector containing the neighbours of a
    virtual vector<int> getNeighbours(int a);


	// Check if Graph g with colouring scheme c is valid or not
    virtual bool checkGraph(); 

    private:
        int size;
        vector<vector<bool> > graph;

};

#endif
