#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
protected:
   int * colours_;

public:
	// Return true if a and b are neighbours
    virtual bool isNeighbours(int a, int b) = 0;
   
    // Add an edge between a and b
    virtual void addNeighbour(int a, int b) = 0;

    // Get the vertex degree
    virtual int getDegree(int vertex) = 0;

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

    //Get all the degrees
    virtual vector<int> getAllDegree(vector<int> &AllDegree)= 0;
   
    //Sort the vertices according to non-increasing degrees
    virtual vector<int> SortbyDegree(vector<int> &AllDegree)= 0;

	//Maintain a sequence of vertex by non-increasing DSATUR
    virtual void SortbyDSATUR(vector<int> &B)=0;

	//Get the degree of saturation of vertex x
	virtual int getVertexDSATUR(int x)=0;

	//Get the vertex with maximum degree of saturation
	virtual int getMaxDSATURvertex(vector<int> &A)= 0;

	//Swap the element in a vector.
    virtual void swap(int a, int b, vector<int> &A)=0;
   
    //Get the set of free colours, which used but not present in neighbor of x
    virtual void getFreeColours(int x,set<int> &U)=0;
   
    //Get the number of used colour
    virtual int getUsedColourNum(int i,vector<int> &U)= 0;

	//Get the larger one of two inputs
	virtual int getMax(int a, int b)= 0;
   
    //Remove colour k
    virtual void removeColour(int k,set<int> &U)= 0;
   
    //Sort the set of colour(referred as U) according to non-decreasing order
    virtual void sortU(vector<int> &U)= 0;
   
    //Uncolour a vertex
    virtual void unColour(int x)= 0;
   
    //Get the colour of vertex with index x
    virtual int getColour(int x)= 0;
    
};

#endif
