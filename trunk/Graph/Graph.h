

#ifndef GRAPH_H
#define GRAPH_H
#include <iostream> 
#include <vector>

using namespace std;

  class Graph {

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

    // Get the vertex degree
    virtual int getDegree(int vertex) = 0;

    // Get the vertex with Maximal degree
    virtual int getMaxDegreeVertex() = 0;

  };

#endif
