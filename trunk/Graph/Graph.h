
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream> 
#include <vector>
#include <queue>
#include <set>

using namespace std;

class Graph {
protected:
   unsigned int * colours_;
   unsigned int * degrees_;

public:

    // Return the degree of saturation for node x
    virtual int getVertexDSATUR(int x) = 0;
    virtual void setColour(unsigned int* c) = 0;

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
    
    // helper function to override ostream to work in subclass
    virtual ostream& print(ostream &os) const = 0;

    /**Parallel specific functions **/
    // Get the vertext with Maximal degree from a slice
    virtual int getMaxDegreeVertex(int from, int to) = 0;
    
    // Return a set of nonNeighbours from a slice
    virtual void nonNeighbours(int a, vector<unsigned int> &nonNeighbours, int from, int to) = 0;
    
    /**Below parallel functions might be removed, but need more testing*/
    // Number of the common neighbours for a and b from a slice
    virtual int getCommonNeighboursCount(int a, int b, int avoidColour) = 0;    

    // Backup and restore a vertex
    virtual void backupVertex (int a, vector<bool> &b) = 0;
    virtual void restoreVertex(int a, vector<bool> b) = 0;
    // Get neighbour of higher index
    virtual void neighbours(int a, queue<unsigned int> &neighbours, int from) = 0; 
    // Contract a into b within a slice;
    virtual void contract(int a, int b, int from, int to) = 0;
    
    // Get the vertex degree
    virtual int getDegree(int vertex, int from, int to) = 0;
    

    // override default ostream so cout<<object can be achieved
    friend ostream& operator<<(ostream &os, const Graph &g) {
       g.print(os);
       return os;
    }
};

#endif
