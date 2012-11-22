
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream> 
#include <vector>

using namespace std;

class Graph {
protected:
   unsigned int * colours_;

public:
    // Return true if a and b are neighbours
    virtual bool isNeighbours(int a, int b) = 0;

    // Return an iterator that will iteratates all the neighbours of a
    // we could discuss more about this and come to agreement if you find this setup doesn't work
    virtual void neighbours(int a, vector<unsigned int> &neighbours) = 0;

    // Return a set of non-neighbours
    virtual void nonNeighbours(int a, vector<unsigned int> &nonNeighbours) = 0;

    // Add an edge between a and b
    virtual void addNeighbour(int a, int b) = 0;

    // Remove an edge between a and b
    virtual bool removeNeighbour(int a, int b) = 0;

    // Number of the common neighbours for a and b
    virtual int getCommonNeighboursCount(int a, int b) = 0;

    // Get the vertext with Maximal degree of a particular set
    virtual int getMaxDegreeVertex(vector<unsigned int> & nn) = 0;

    // Get the vertex degree
    virtual int getDegree(int vertex) = 0;

    // Get the vertex with Maximal degree
    virtual int getMaxDegreeVertex() = 0;

    // Get the vertex count in G
    virtual unsigned int getSize() = 0;

    // Contract a into b;
    virtual void contract(int a, int b) = 0;

    // Set color for a vertex
    virtual void setColour(int v, int colour) = 0;

    // reset colors;
    virtual void resetColours() = 0;
    // get colors of graph
    virtual unsigned int* getColours() = 0;


};

#endif
