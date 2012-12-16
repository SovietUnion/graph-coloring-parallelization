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

    static const int maxInt     = 2147483646;
    static const int notInQueue = 2147483645;

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
       PC_CONTINUE, PC_KILL, PC_PAUSE 
    };

    // Thread Current Status
    enum CtoPSig {
       CP_RUNNING, CP_RETURNED, CP_PAUSE, CP_KILLED
    };

    enum BackAction {
       BACK_ONCE, BACK_ROLLBACK, BACK_KILL, BACK_GOOD
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
      unsigned int holdingFopt;
      int optColorNumber; 
      int prevColor;
      int forkPoint;
      BackAction back;
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
      //list<pair<int,int> > spawn;
      list<pair<int,int> > spawn;
    };

    // Instance Data
    pthread_t** threads;
    int GlobalOptColor;
    pthread_mutex_t GlobalOptColorLock;
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
       return  ((ParallelBSCAlgorithm*)p[0])->colourGraph((void*) p[1]);
    }
    void* colourGraph(void* c);
 
    // updates 
    void update(int x, list<int>& updates, vector<pair<int,int> >& undo,
                SkewHeap* h, unsigned int* colours);

    void findFreeColour(int a, int colourNumber, set<unsigned int>& neighbour_colours, unsigned int* colours);
    void prepareDataForThread(int parent, int child, int fork);
};

#endif

