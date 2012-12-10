//BSCAlgorithm.h
#ifndef BSCALGORITHM_H
#define BSCALGORITHM_H

#include "Algorithm.h"
#include <set>
#include<queue>

using namespace std;

class BSCAlgorithm : public Algorithm{
public:

    static const int maxInt     = 2147483647;
    static const int notInQueue = 2147483646;

    // Constructor
    BSCAlgorithm(Graph* g);

    // Colour the graph G and return number of colours used
    virtual int colourGraph();

protected:

    struct SkewHeap {
      int DSAT;
      int parent;
      int left;
      int right; 
    };

    struct BSCData {

      // Basic Data
      int x;
      int root;
      int colors;

      set<unsigned int> U;
      vector<pair<int,int> > undo;
    };

    // Heap functions
    static int  mergeHeap(SkewHeap* h, int a, int b);
    static int  mergeHeap(SkewHeap* h, queue<int>& q);
    static void popHeap(SkewHeap* h, int root, queue<int>& updates);
    static void revert(SkewHeap* h, vector<pair<int,int> >& undo, queue<int>& updates);

    // updates 
    void update(int x, queue<int>& updates, vector<pair<int,int> >& undo,
                SkewHeap* h, unsigned int* colours);

    void findFreeColour(int a, int colourNumber, set<unsigned int>& neighbour_colours);
};

#endif

