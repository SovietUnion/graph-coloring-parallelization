//ParallelBSCAlgorithm.h
#ifndef PARALLELBSCALGORITHM_H
#define PARALLELBSCALGORITHM_H

#include "Algorithm.h"
#include <set>
#include <queue>
#include <stack>

using namespace std;

class ParallelBSCAlgorithm : public Algorithm{
public:

    static const int maxInt     = 2147483647;
    static const int notInQueue = 2147483646;

    // Constructor
    ParallelBSCAlgorithm(Graph* g, int threadcount);

    // Colour the graph G and return number of colours used
    virtual int colourGraph();

protected:

    struct SkewHeap {
      int DSAT;
      int parent;
      int left;
      int right; 
    };

    struct ParallelBSCData {

      // Basic Data
      int x;
      int colors;

      set<unsigned int> U;
      vector<pair<int,int> > undo;
      stack<int> spawnedThread;
    };

    // Signals between Parent to Children threads
    enum TtoCSignal {
       CONTINUE, KILL, RETURN
    };

    // Thread Current Status
    enum CtoTIndicator {
       RUNNING, RETURNED
    };

    // Data used by colourGraph
    struct MiscData {

      // Simple Variables
      int size;
      int optColorNumber; 
      int folkPoint;
      bool back;
      int start;
      int root;

      ParallelBSCData* A;
      queue<int> pendingUpdates;
      SkewHeap* heap;
      unsigned int* colours;
      unsigned int* Fopt;
      vector<int> childrenThread; // Keep track of you children  
      vector<int> threadPool;     // Keep track of unused threads
      stack<int> spawnLocation;   // Keep track of where you spawned

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

