#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
protected:
   int * colours_;
   int * degrees_;

public:
	// Return true if a and b are neighbours
    virtual bool isNeighbours(int a, int b) = 0;

    // Return an iterator that will iteratates all the neighbours of a
    virtual void neighbours(int a, vector<int> &neighbours) = 0;
   
    // Add an edge between a and b
    virtual void addNeighbour(int a, int b) = 0;

    // Get the vertex degree
    virtual int getDegree(int vertex) = 0;

    //Get the vertex with MAX degree
	virtual int getMaxDegreeVertex()=0;

    // Get the vertex count in G
    virtual int getSize() = 0;

    // Set color for a vertex
    virtual void setColour(int v, int colour) = 0;

    // get colors of graph
    virtual int* getColours() = 0;

	//Print out graph
	virtual void printGraph()= 0;

	// Return an vector containing the neighbours of a
    virtual vector<int> getNeighbours(int a)= 0;

	//Decide which vertex to color next
    virtual void getNextVertex(vector<int> &A,vector<pair <int,int>> &B)=0;

	//Get the degree of saturation of vertex x
	virtual int getVertexDSATUR(int x)=0;
   
    //Get the set of free colours, which used but not present in neighbor of x
    virtual void getFreeColours(int x,set<int> &U,int l)=0;
   
    //Get the number of used colour
    virtual int getUsedColourNum(int i,vector<int> &U)= 0;

	//Get the larger one of two inputs
	virtual int getMax(int a, int b)= 0;
   
    //Remove colour k
    virtual void removeColour(int k,set<int> &U)= 0;
   
    //Uncolour a vertex
    virtual void unColour(int x)= 0;
   
    //Get the colour of vertex with index x
    virtual int getColour(int x)= 0;

	// Check if Graph g with colouring scheme c is valid or not
    virtual bool checkGraph()=0; 
    
};

#endif
