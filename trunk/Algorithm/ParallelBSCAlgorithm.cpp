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
    void*** arg = new void**[threadcount];

    // Initialize array
    for (int i = 0; i < threadcount; i++) {
       arg[i] = new void*[2];
       arg[i][0] = (void*) this;
       arg[i][1] = (void*) i;
       T[i] = new MiscData();
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
ParallelBSCAlgorithm::mergeHeap(SkewHeap* h, queue<int>& q) {

  // Return right away if there are nothing to merge
  if (q.size() == 1) {
    int tmp = q.front(); q.pop();
    return tmp;
  }

  int root = maxInt;

  // Merging several independent heaps
  while(q.size() > 1) {
    int a = q.front(); q.pop();
    int b = q.front(); q.pop();
    root = mergeHeap(h,a,b);
    q.push(root);
  }

  // push out the last one
  q.pop();

  return root;
}

void
ParallelBSCAlgorithm::update(int x, queue<int>& updates,
                     vector<pair<int,int> >& undo, SkewHeap* h,
                     unsigned int* colours) {

    vector<unsigned int> n;

    // Get neigbhours of x and update their 
    g_->neighbours(x,n);

    // Update the DSAT value of its neighbours
    // Remove it from the heap and queue it up if it has a parent
    for (int i = 0; i < n.size(); i++) {

       // Only update DSAT values of non-coloured neighbours
       if (colours[n[i]] != 0)
          continue;

       undo.push_back(make_pair(n[i],h[n[i]].DSAT));
       h[n[i]].DSAT = g_->getVertexDSATUR(n[i]) * g_->getSize() +
                      g_->getDegree(n[i]);

       // It is already in the queue if it has maxInt as parent
       if (h[n[i]].parent == notInQueue) {
         // Add it onto the queue if it is not in the queue yet
         h[n[i]].parent = maxInt;
         updates.push(n[i]);
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
         updates.push(n[i]);
       } 
    }
};

void
ParallelBSCAlgorithm::revert(SkewHeap* h, vector<pair<int,int> >& undo, queue<int>& updates) {

    // Undo the updated DSAT value of its neighbours
    // Remove it from the heap and queue it up if it has a parent
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
           updates.push(left);
       }

       // Cut off its right child if right child is greater after the update
       if (right != maxInt && h[right].DSAT > dsat) {
           h[i].right = maxInt;
           h[right].parent = maxInt;
           updates.push(right);
       }
    }
};


void
ParallelBSCAlgorithm::popHeap(SkewHeap* h, int root, queue<int>& updates) {

    int right = h[root].right;
    int left  = h[root].left ;

    // Clear out data
    h[root].parent = notInQueue;
    h[root].right  = maxInt;
    h[root].left   = maxInt;

    // Set the children's parent to maxInt, and push it to updates
    if (right != maxInt) {
        h[right].parent = maxInt;
        updates.push(right);
    }

    if (left  != maxInt) {
        h[left].parent  = maxInt;
        updates.push(left);
    }
}

void
ParallelBSCAlgorithm::findFreeColour(int a, int colourNumber, set<unsigned int>& neighbour_colours) {

  vector<unsigned int> na;
  set<unsigned int>::iterator it;
  unsigned int* colours = g_->getColours();

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

  // can at most go to colourNumber + 1
  neighbour_colours.insert(colourNumber+1);

}

//Test ParallelBSC
int
ParallelBSCAlgorithm::colourGraph(){

    int size = g_->getSize();
    T[0]->back = false;
    T[0]->start = 0;

    T[0]->A = new ParallelBSCData[size];
    T[0]->heap = new SkewHeap[size];
    T[0]->colours = g_->getColours();
    T[0]->Fopt    = new unsigned int[size];

    // Initialize Heap
    for (int i = 0; i < size; i++) { 
       T[0]->heap[i].DSAT   = g_->getDegree(i);
       T[0]->heap[i].parent = maxInt;
       T[0]->heap[i].left   = maxInt;
       T[0]->heap[i].right  = maxInt;
       T[0]->pendingUpdates.push(i);
    }
    T[0]->A[0].x = g_->getMaxDegreeVertex();
    T[0]->heap[T[0]->A[0].x].DSAT = g_->getDegree(T[0]->A[0].x);
    T[0]->root = mergeHeap(T[0]->heap, T[0]->pendingUpdates);
    popHeap(T[0]->heap, T[0]->root, T[0]->pendingUpdates);

    T[0]->A[0].x = T[0]->root;
    T[0]->A[0].U.insert(1);
    T[0]->heap[T[0]->A[0].x].DSAT = g_->getVertexDSATUR(T[0]->A[0].x)*size + g_->getDegree(T[0]->A[0].x);
    T[0]->optColorNumber = g_->getDegree(T[0]->A[0].x) + 1;
    //optColorNumber = size;
    while(T[0]->start >= 0) {

      T[0]->back = false;

      // Keep colouring until you can't
      for (int i = T[0]->start; i < size; i++) {

         int c = 0;

         // Not the first one
         if (i > T[0]->start) {

           if (i > 0)
              c = T[0]->A[i-1].colors;

           if (T[0]->optColorNumber - 2 > 0)
              c = min(c,T[0]->optColorNumber - 2);

           // Find the node with the maximum degree of saturation
           T[0]->root = mergeHeap(T[0]->heap, T[0]->pendingUpdates);
           popHeap(T[0]->heap, T[0]->root, T[0]->pendingUpdates);

           // Find the set of free colour for that node
           T[0]->A[i].U.clear();
           findFreeColour(T[0]->root,c,T[0]->A[i].U);

         }

         // Check if the set is empty or not
         if (T[0]->A[i].U.size() > 0 && *T[0]->A[i].U.begin() < T[0]->optColorNumber) {

           // Colour the node
           c = *T[0]->A[i].U.begin(); T[0]->A[i].U.erase(T[0]->A[i].U.begin());
           T[0]->colours[T[0]->root] = c;

           // Remember which node coloured
           T[0]->A[i].x = T[0]->root;
           if (i > 0)
             T[0]->A[i].colors = max(c,T[0]->A[i-1].colors);
           else
             T[0]->A[i].colors = c;

           // Update the heap
           update(T[0]->A[i].x, T[0]->pendingUpdates,
                  T[0]->A[i].undo, T[0]->heap, T[0]->colours);
         } else {
           // Exit for loop if failed
           T[0]->start = i-1;
           T[0]->back = true;
           break;
         }
      }

      // Check if the above for loop found a good colouring or not
      if (T[0]->back) {

        // Add the removed node back to the heap
        if (T[0]->heap[T[0]->root].parent == notInQueue) {
           T[0]->heap[T[0]->root].parent = maxInt;
           T[0]->pendingUpdates.push(T[0]->root);
        }

        if (T[0]->start >= 0) {

          // Try another colour for the previous node
          T[0]->root = T[0]->A[T[0]->start].x;
          T[0]->colours[T[0]->root] = 0;
          revert(T[0]->heap,T[0]->A[T[0]->start].undo,T[0]->pendingUpdates);
 
        }
      } else {

        // Copy the colouring over
        for (int i = 0; i < size; i++)
           T[0]->Fopt[i] = T[0]->colours[i];

        int prevOpt = T[0]->optColorNumber;
        T[0]->optColorNumber = T[0]->A[size-1].colors;

        // Look for where to restart and remove unused colours of the freeColor set
        for (T[0]->start = 0; T[0]->A[T[0]->start].colors != T[0]->optColorNumber; T[0]->start++) {
           for (int i = prevOpt; i >= T[0]->optColorNumber; i--) 
              T[0]->A[T[0]->start].U.erase(i);
        }
        T[0]->start--;
        if (T[0]->start < 0)
           break;      // optimal is found!

        // revert changes
        for (int i = size-1; i >= T[0]->start; i--) {
          T[0]->colours[T[0]->A[i].x] = 0;
          if (T[0]->heap[T[0]->A[i].x].parent == notInQueue) {
             T[0]->heap[T[0]->A[i].x].parent = maxInt;
             T[0]->pendingUpdates.push(T[0]->A[i].x);
          }
          revert(T[0]->heap,T[0]->A[i].undo,T[0]->pendingUpdates);
        }
        T[0]->colours[T[0]->start] = 0;
        T[0]->root = T[0]->start;
      }

    }

    // copy Fopt over
    g_->setColour(T[0]->Fopt); 
 
    return T[0]->optColorNumber;  
}
