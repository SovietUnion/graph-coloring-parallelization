

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
    virtual vector<unsigned int> neighbours(int a) = 0;

    // Add an edge between a and b
    virtual void addNeighbour(int a, int b) = 0;

    // Remove an edge between a and b
    virtual bool removeNeighbour(int a, int b) = 0;
  };

#endif
