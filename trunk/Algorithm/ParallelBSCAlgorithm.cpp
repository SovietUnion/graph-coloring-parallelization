//ParallelBSCAlgorithm.cpp
#include "ParallelBSCAlgorithm.h"
#include "Algorithm.h"
#include <vector>
#include <iostream>
using namespace std;

// Constructor
ParallelBSCAlgorithm::ParallelBSCAlgorithm(Graph* g, int threadcount) {

    g_ = g;
    // reset colours array
    g_->resetColours();

    // Initialize the array pointers
    threadcount_=threadcount;
    threads = new pthread_t*[threadcount];
    T   = new MiscData*[threadcount];
    p2c = new PtoC*[threadcount];
    c2p = new CtoP*[threadcount];

    // Arguments used for thread creation
    pth_arg = new void**[threadcount];
    int size = g_->getSize();

    // Initialize arrays
    for (int i = 0; i < threadcount; i++) {
       pth_arg[i] = new void*[2];
       pth_arg[i][0] = (void*) this;
       pth_arg[i][1] = (void*) i;
       threads[i] = new pthread_t();
       T[i] = new MiscData();
       T[i]->A = new ParallelBSCData[size];
       T[i]->heap = new SkewHeap[size];
       T[i]->colours = new unsigned int[size];
       T[i]->Fopt = new unsigned int[size];
       p2c[i] = new PtoC();
       c2p[i] = new CtoP();
    }
}


// Tree merge
int
ParallelBSCAlgorithm::mergeHeap(SkewHeap* h, int a, int b) {

  int root = a;
  int addee = b;

  // Return if you are trying to merge the same node
  if (a == b) 
    return maxInt; // force an error

  if (h[a].DSAT < h[b].DSAT) {
    root = b;
    addee = a;
  }


  if (h[root].right != maxInt) {
    addee = mergeHeap(h,h[root].right,addee);
  }

  h[addee].parent = root;

  // Swap the children
  h[root].right = h[root].left;
  h[root].left  = addee;

  return root;
}


int
ParallelBSCAlgorithm::mergeHeap(SkewHeap* h, list<int>& q) {

  // Return right away if there are nothing to merge
  if (q.size() == 1) {
    int tmp = q.front(); q.pop_front();
    return tmp;
  }

  int root = maxInt;

  // Merging several independent heaps
  while(q.size() > 1) {
    int a = q.front(); q.pop_front();
    int b = q.front(); q.pop_front();
    root = mergeHeap(h,a,b);
    q.push_back(root);
  }

  // push out the last one
  q.pop_front();

  return root;
}

void
ParallelBSCAlgorithm::update(int x, list<int>& updates,
                     vector<pair<int,int> >& undo, SkewHeap* h,
                     unsigned int* colours) {

    vector<unsigned int> n;
    undo.clear();

    // Get neigbhours of x and update their 
    g_->neighbours(x,n);

    // Update the DSAT value of its neighbours
    // Remove it from the heap and queue it up if it has a parent
    for (int i = 0; i < n.size(); i++) {

       // Only update DSAT values of non-coloured neighbours
       if (colours[n[i]] != 0)
          continue;

       undo.push_back(make_pair(n[i],h[n[i]].DSAT));
       h[n[i]].DSAT = g_->getVertexDSATUR(n[i],colours) * g_->getSize() +
                      g_->getDegree(n[i]);

       // It is already in the queue if it has maxInt as parent
       if (h[n[i]].parent == notInQueue) {
         // Add it onto the queue if it is not in the queue yet
         h[n[i]].parent = maxInt;
         updates.push_back(n[i]);
       } else if (h[n[i]].parent != maxInt) {
         int parent = h[n[i]].parent;
         int right  = h[n[i]].right;

         // Give its right child to its parent
         if (h[parent].left == n[i])
             h[parent].left = right;
         else 
             h[parent].right = right;

         // If the right child is a node, set its parent properly
         if (right != maxInt)
             h[right].parent = parent;

         h[n[i]].right = maxInt;

         // Remove from heap
         h[n[i]].parent = maxInt;
         updates.push_back(n[i]);
       } 
    }
};

void
ParallelBSCAlgorithm::revert(SkewHeap* h, vector<pair<int,int> >& undo, list<int>& updates) {

    // Undo the updated DSAT value of its neighbours
    // Remove its children from the heap and queue it up if it has a parent
    while (!undo.empty()) {

       int i = undo.back().first;
       int dsat = undo.back().second;
       undo.pop_back();

       h[i].DSAT = dsat;

       int left   = h[i].left ;
       int right  = h[i].right;

       // Cut off its left child if left child is greater after the update
       if (left != maxInt && h[left].DSAT > dsat) {
           h[i].left = maxInt;
           h[left].parent = maxInt;
           updates.push_back(left);
       }

       // Cut off its right child if right child is greater after the update
       if (right != maxInt && h[right].DSAT > dsat) {
           h[i].right = maxInt;
           h[right].parent = maxInt;
           updates.push_back(right);
       }
    }
};


void
ParallelBSCAlgorithm::popHeap(SkewHeap* h, int root, list<int>& updates) {

    int right = h[root].right;
    int left  = h[root].left ;

    // Clear out data
    h[root].parent = notInQueue;
    h[root].right  = maxInt;
    h[root].left   = maxInt;

    // Set the children's parent to maxInt, and push it to updates
    if (right != maxInt) {
        h[right].parent = maxInt;
        updates.push_back(right);
    }

    if (left  != maxInt) {
        h[left].parent  = maxInt;
        updates.push_back(left);
    }
}

void
ParallelBSCAlgorithm::findFreeColour(int a, int colourNumber, set<unsigned int>& neighbour_colours, unsigned int* colours) {

  vector<unsigned int> na;
  set<unsigned int>::iterator it;

  g_->neighbours(a,na);

  // insert the set of free colour
  neighbour_colours.insert(colourNumber);
  it = neighbour_colours.begin();
  for(unsigned int j = colourNumber - 1; j > 0; j--)
     neighbour_colours.insert(it,j);

  // Go through its neighbours and remove neighbouring colours from free colour
  for (int j = 0; j < na.size() && !neighbour_colours.empty(); j++) {
     neighbour_colours.erase(colours[na[j]]);
  }

}

//Test ParallelBSC
void 
ParallelBSCAlgorithm::prepareDataForThread (int parent, int child, int fork) {

    int size = g_->getSize();
    int pfork = T[parent]->forkPoint;
    int cfork = T[parent]->forkPoint + fork;
    int numFreeColor = T[parent]->A[fork].U.size()  / 2;
    int numThread    = T[parent]->threadPool.size() / 2;

    T[child]->optColorNumber = T[parent]->optColorNumber; 
    T[child]->forkPoint      = cfork;
    T[child]->parent         = parent;
    T[child]->start          = 0;
    T[child]->back           = T[parent]->back;
    T[child]->root           = T[parent]->root;

    if (fork > 0)
      T[child]->A[0].colors    = T[parent]->A[fork-1].colors+1;  
    else
      T[child]->A[0].colors    = T[parent]->A[fork].colors+1;  

    // Initialize Colour
    for (int i = 0; i < size; i++)
       T[child]->colours[i] = T[parent]->colours[i];

    // Initialize Heap
    for (int i = 0; i < size; i++) {
         T[child]->heap[i].DSAT   = T[parent]->heap[i].DSAT;
         T[child]->heap[i].parent = T[parent]->heap[i].parent;
         T[child]->heap[i].left   = T[parent]->heap[i].left  ;
         T[child]->heap[i].right  = T[parent]->heap[i].right ;
    }
    T[child]->pendingUpdates.clear();
    T[child]->pendingUpdates = T[parent]->pendingUpdates;
  
    // Give half the set and threads to the child
    set<unsigned int>::reverse_iterator it = T[parent]->A[fork].U.rbegin();
    for (int i = 0; i < numFreeColor; i++,it++) {
        T[child]->A[0].U.insert(*it);
        T[parent]->A[fork].U.erase(*it);
    }
    for (int i = 0; i < numThread; i++)
       T[child]->threadPool.splice(T[child]->threadPool.begin(),
                                   T[parent]->threadPool,
                                   T[parent]->threadPool.begin());

    // Setup signals
    p2c[child]->s = PC_CONTINUE;
    p2c[child]->OptColorFound = T[parent]->optColorNumber;
    c2p[child]->s = CP_RUNNING;
    c2p[child]->OptColorFound = maxInt;

    // rememeber the spawns
    T[parent]->spawn.push_front(make_pair(child,fork));
}

int
ParallelBSCAlgorithm::colourGraph(){

    int size = g_->getSize();

    T[0]->A       = new ParallelBSCData[size];
    T[0]->heap    = new SkewHeap[size];
    T[0]->colours = g_->getColours();
    T[0]->Fopt    = new unsigned int[size];

    // Initialize Heap
    for (int i = 0; i < size; i++) { 
       T[0]->heap[i].DSAT   = g_->getDegree(i);
       T[0]->heap[i].parent = maxInt;
       T[0]->heap[i].left   = maxInt;
       T[0]->heap[i].right  = maxInt;
       T[0]->pendingUpdates.push_back(i);
    }
    T[0]->A[0].x = g_->getMaxDegreeVertex();
    T[0]->heap[T[0]->A[0].x].DSAT = g_->getDegree(T[0]->A[0].x);
    T[0]->root = mergeHeap(T[0]->heap, T[0]->pendingUpdates);
    popHeap(T[0]->heap, T[0]->root, T[0]->pendingUpdates);

    T[0]->A[0].x = T[0]->root;
    T[0]->A[0].U.insert(1);
    T[0]->optColorNumber = g_->getDegree(T[0]->A[0].x) + 1;
    
    T[0]->forkPoint = 0;

    // Insert thread resources
    for (int i = 1; i < threadcount_; i++)
      T[0]->threadPool.push_back(i);

    long tmp = (long) colourGraph((void*) 0);

    // copy Fopt over
    g_->setColour(T[0]->Fopt); 

    return (int) tmp;
}

void*
ParallelBSCAlgorithm::colourGraph(void* c){

    // data used by function
    int size = g_->getSize();
    int t = (int) c;
    int beforeRollBack = 0;
    void* ret;

    // create alias for readability
    BackAction&  back     = T[t]->back;
    int&   optColorNumber = T[t]->optColorNumber; 
    int&   forkPoint      = T[t]->forkPoint;
    int&   start          = T[t]->start;
    int&   root           = T[t]->root;

    ParallelBSCData* A;
    SkewHeap* heap;
    unsigned int* colours;
    unsigned int* Fopt;

    A = T[t]->A;
    heap = T[t]->heap;
    Fopt = T[t]->Fopt;
    colours = T[t]->colours;
 
    // main loop
    // Try tocolour vertice at forkPoint and beyond
    size -= forkPoint; // only worry about colouring at the fork point and beyond
    while(start >= 0) {

      back = BACK_GOOD;

      // Keep colouring until you can't
      for (int i = start; i < size; i++) {

         int c = 0;
         beforeRollBack = i;

         // Not the first one
         if (i > start) {

           if (i > 0)
              c = A[i-1].colors + 1;
           else
              c = A[0].colors;

           if (optColorNumber - 1 > 0)
              c = min(c,optColorNumber - 1);

           // Find the node with the maximum degree of saturation
           root = mergeHeap(heap, T[t]->pendingUpdates);
           popHeap(heap, root, T[t]->pendingUpdates);

           // Find the set of free colour for that node
           A[i].U.clear();
           findFreeColour(root,c,A[i].U,colours);

         }

    // if (t > 2)  
     { cout << "T" << t << ": I" << i << ": V" << root << ": C" << optColorNumber << ": ";
      for (set<unsigned int>::iterator it = A[i].U.begin(); it != A[i].U.end(); it++)
          cout << " " << (*it);
      cout << endl; }

         // Listen to parent threads for signals
         if (t > 0) {  // only if you are not the master thread
           if (p2c[t]->s == PC_KILL) {
               back = BACK_KILL; break;
           } else if (p2c[t]->s == PC_PAUSE ) {
               back = BACK_KILL; break;
           } else if (p2c[t]->OptColorFound <= optColorNumber) {
               optColorNumber = p2c[t]->OptColorFound;

               // Rollback if the parent found a better colour already
               if (c >= optColorNumber) {
                 back = BACK_ROLLBACK;
                 cout << "Thread " << t << " Rolled back due to sig" << endl;
                 break;
               }
           }
         }

         // Watch for children threads
         //while (!T[t]->spawn.empty()) {

         //    pair<int, int> p = T[t]->spawn.front();
         //    int child = p.first;
         //}

         // Check if the set is empty or not
         if (A[i].U.size() > 0){// && *A[i].U.begin() < optColorNumber) {

           // Spawn threads if it has the resources
           // there is a branch
           if (A[i].U.size() > 1 && T[t]->threadPool.size() > 0) {

              // Prepare the data for the thread
              int child = T[t]->threadPool.front(); 
              T[t]->threadPool.pop_front(); 
              prepareDataForThread(t, child, i);
              cout << "Thread " << child << " launched at " << i << endl;
              // Launch thread :)
              pthread_create(threads[child], NULL, this->colour_helper, (void*) pth_arg[child]);
           }

           // Colour the node
           c = *A[i].U.begin(); A[i].U.erase(A[i].U.begin());
           colours[root] = c;

           // Remember which node coloured
           A[i].x = root;
           if (i > 0)
             A[i].colors = max(c,A[i-1].colors);
           else
             A[i].colors = c;

           // Update the heap
           update(A[i].x, T[t]->pendingUpdates, A[i].undo, heap, colours);
         } else {
           // Exit for loop if failed
           start = i-1;
           back = BACK_ONCE;
           break;
         }
      }

      // Check if the above for loop found a good colouring or not
      switch (back) {

       case BACK_ONCE:
        // Add the removed node back to the heap
        if (heap[root].parent == notInQueue) {
           heap[root].parent = maxInt;
           T[t]->pendingUpdates.push_back(root);
        }

        if (start >= 0) {

          // Try another colour for the previous node
          root = A[start].x;
          colours[root] = 0;
          revert(heap,A[start].undo,T[t]->pendingUpdates);
 
        }
        break;

       case BACK_GOOD:
        // Copy the colouring over
        for (int i = 0; i < size; i++)
           Fopt[i] = colours[i];

        optColorNumber = A[size-1].colors;
        c2p[t]->OptColorFound = optColorNumber;

       // Good carries on to roll back!!
        cout << "T" << t << " found good colour " << optColorNumber << endl;
       case BACK_ROLLBACK:
        cout << "T" << t << " is rolling back" << endl;
        // Notify your children about the better colour
        for (list<pair<int, int> >::iterator it = T[t]->spawn.begin();
             it != T[t]->spawn.end(); it++) {
             p2c[(*it).first]->OptColorFound = optColorNumber;
        }

        // Look for where to restart and remove unused colours of the freeColor set
        {
        for (start = 0; A[start].colors < optColorNumber && start <= beforeRollBack; start++) {
               cout << "T" << t << " erasing U " << start << endl;
           for (set<unsigned int>::reverse_iterator it = A[start].U.rbegin();
                it != A[start].U.rend() && *it >= optColorNumber; it = A[start].U.rbegin()) {
                    A[start].U.erase(*it);
            }
        } }
        start--;
        if (start < 0) {
           goto LOOP_END;
           break;      // optimal is found!
        }

        // revert changes
        for (int i = size-1; i >= start; i--) {
          colours[A[i].x] = 0;
          if (heap[A[i].x].parent == notInQueue) {
             heap[A[i].x].parent = maxInt;
             T[t]->pendingUpdates.push_back(A[i].x);
          }
          revert(heap,A[i].undo,T[t]->pendingUpdates);
        }
        colours[start] = 0;
        root = start;
        break;

       case BACK_KILL:
        goto LOOP_END;
        break;
 
       default:
        cout << "I shouldn't be here " << endl;
        break;

      }
      

    }

    LOOP_END:
    cout << "Thread " << t << " completed" << endl;
    // Wait for spawned threads
    while (!T[t]->spawn.empty()) {

       void* tmp;
       pair<int, int> p = T[t]->spawn.front();
       int childThread = p.first;
       T[t]->spawn.pop_front();

       // Release children threads resources
       cout << "Thread " << t << " is Waiting for Thread " << childThread << " to join" << endl;
       pthread_join(*threads[childThread], &tmp);
       T[t]->threadPool.push_back(childThread);
       T[t]->threadPool.splice(T[t]->threadPool.end(),T[childThread]->threadPool);
       cout << "Thread " << childThread << " joined" << endl;
       cout << "Return " << tmp << endl;

       // Keep the best results if the child is returned, not killed
       if (   c2p[childThread]->s == CP_RETURNED
          && (unsigned long)tmp < c2p[t]->OptColorFound) {
          c2p[t]->OptColorFound = (int)tmp;
          unsigned int* f = T[t]->Fopt;
          T[t]->Fopt = T[childThread]->Fopt;
          T[childThread]->Fopt = f;
       }

    }

    // Indicate how the thread is returned
    if (p2c[t]->s == PC_KILL)
        c2p[t]->s = CP_KILLED;
    else if (p2c[t]->s == PC_PAUSE )
        c2p[t]->s = CP_PAUSE;
    else 
        c2p[t]->s = CP_RETURNED;

    ret = (void*) c2p[t]->OptColorFound; 
    return ret;  
}
