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

    /**Parallel specific functions **/
    // Get the vertex with Maximal degree from a slice
    virtual int getMaxDegreeVertex(int from, int to);

    // Return a set of nonNeighbours from a slice
    virtual void nonNeighbours(int a, vector<unsigned int> &nonNeighbours, int from, int to);
    
    /**Below parallel functions might be removed, but need more testing*/
    // Number of the common neighbours for a and b from a slice
    virtual int getCommonNeighboursCount(int a, int b, int from, int to);
    
    // Contract a into b within a slice;
    virtual void contract(int a, int b, int from, int to);
    
    // Backup and restore a vertex
    virtual void backupVertex (int a, vector<bool> &b);
    virtual void restoreVertex(int a, vector<bool> b);
    
    // Get the vertex degree
    virtual int getDegree(int vertex, int from, int to);
    
    // format ostream output to printout the graph matrix
    virtual ostream& print(ostream &os) const;
    private: 
	unsigned int size;	
	vector<vector<bool> > graph;

};

/************************************************************/
//For BSC by Yao
#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "Graph.h"
using namespace std;
class MatrixGraph: public Graph {

   public:
    // Constructor that take in a pregenerated graph
    MatrixGraph(const vector<vector<bool> > &g, int mSize);
	
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
   
/***************************************************************/
	
	//For BSC

    //Get all the degrees
    virtual vector<int> getAllDegree(vector<int> &AllDegree);
   
    //Sort the vertices according to non-increasing degrees
    virtual vector<int> SortbyDegree(vector<int> &AllDegree);
   
    //Get the set of free colours, which used but not present in neighbor of x
    virtual void getFreeColours(int x,vector<int> &U);
   
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
   
    private:
        int size;
        vector<vector<bool> > graph;

};


#endif
