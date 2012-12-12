//ParallelBSCAlgorithm.h
#ifndef PARALLELBSCALGORITHM_H
#define PARALLELBSCALGORITHM_H

#include "Algorithm.h"
#include <pthread.h>
#include <set>
#include <list>
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
    };

    // Signals between Parent to Children threads
    enum PtoCSig {
       CONTINUE, KILL, RETURN
    };

    // Thread Current Status
    enum CtoPSig {
       RUNNING, RETURNED, KILLED
    };

    // Structures for Communications
    struct PtoC {
        PtoCSig s;
        int OptColorFound;
    };
    struct CtoP {
        CtoPSig s;
        int OptColorFound;
    };

    // Data used by colourGraph
    struct MiscData {

      // Simple Variables
      int optColorNumber; 
      int forkPoint;
      bool back;
      int start;
      int root;

      ParallelBSCData* A;
      list<int> pendingUpdates;
      SkewHeap* heap;
      unsigned int* colours;
      unsigned int* Fopt;

      // Parallel Data
      int parent;
      list<int>  threadPool;        // Keep track of unused threads

      // Keep track of where you spawned,
      // first is thread ID, second is spawn location
      list<pair<int,int> > spawn;
    };

    // Instance Data
    pthread_t** threads;
    MiscData**  T;
    PtoC**      p2c;
    CtoP**      c2p;
    void*** pth_arg;

    // Heap functions
    static int  mergeHeap(SkewHeap* h, int a, int b);
    static int  mergeHeap(SkewHeap* h, list<int>& q);
    static void popHeap(SkewHeap* h, int root, list<int>& updates);
    static void revert(SkewHeap* h, vector<pair<int,int> >& undo, list<int>& updates);

    // Helper Function
    static void* colour_helper(void* c)
    {
       void** p = (void**) c;
       cout << "been here" << endl;
       return  ((ParallelBSCAlgorithm*)p[0])->colourGraph((void*) p[1]);
    }
    void* colourGraph(void* c);
 
    // updates 
    void update(int x, list<int>& updates, vector<pair<int,int> >& undo,
                SkewHeap* h, unsigned int* colours);

    void findFreeColour(int a, int colourNumber, set<unsigned int>& neighbour_colours);
    void prepareDataForThread(int parent, int child, int fork);
};

#endif

