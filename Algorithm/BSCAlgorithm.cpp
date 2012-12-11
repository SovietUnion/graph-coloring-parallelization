//BSCAlgorithm.cpp
#include "BSCAlgorithm.h"
#include "Algorithm.h"
#include <vector>
#include <iostream>
using namespace std;

// Constructor
BSCAlgorithm::BSCAlgorithm(Graph* g) {
    g_ = g;
    // reset colours array
    g_->resetColours();
}


// Tree merge
int
BSCAlgorithm::mergeHeap(SkewHeap* h, int a, int b) {

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
BSCAlgorithm::mergeHeap(SkewHeap* h, queue<int>& q) {

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
BSCAlgorithm::update(int x, queue<int>& updates,
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
BSCAlgorithm::revert(SkewHeap* h, vector<pair<int,int> >& undo, queue<int>& updates) {

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
BSCAlgorithm::popHeap(SkewHeap* h, int root, queue<int>& updates) {

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
BSCAlgorithm::findFreeColour(int a, int colourNumber, set<unsigned int>& neighbour_colours) {

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

//Test BSC
int
BSCAlgorithm::colourGraph(){

    int size = g_->getSize();
    int optColorNumber; 
    bool back = false;
    int start = 0;
    int root;

    BSCData* A = new BSCData[size];
    queue<int> pendingUpdates;
    SkewHeap* heap = new SkewHeap[size];
    unsigned int* colours = g_->getColours();
    unsigned int* Fopt    = new unsigned int[size];

    // Initialize Heap
    for (int i = 0; i < size; i++) { 
       heap[i].DSAT   = g_->getDegree(i);
       heap[i].parent = maxInt;
       heap[i].left   = maxInt;
       heap[i].right  = maxInt;
       pendingUpdates.push(i);
    }
    A[0].x = g_->getMaxDegreeVertex();
    heap[A[0].x].DSAT = g_->getDegree(A[0].x);
    root = mergeHeap(heap, pendingUpdates);
    popHeap(heap, root, pendingUpdates);

    A[0].x = root;
    A[0].U.insert(1);
    heap[A[0].x].DSAT = g_->getVertexDSATUR(A[0].x)*size + g_->getDegree(A[0].x);
    optColorNumber = g_->getDegree(A[0].x) + 1;
    //optColorNumber = size;
    
    while(start >= 0) {

      back = false;

      //cout << "V" << start << ": C" << optColorNumber << ": ";
      //for (set<unsigned int>::iterator it = A[start].U.begin(); it != A[start].U.end(); it++)
      //    cout << " " << (*it);
      //cout << endl;
      // Keep colouring until you can't
      for (int i = start; i < size; i++) {

         int c = 0;

         // Not the first one
         if (i > start) {

           if (i > 0)
              c = A[i-1].colors;

           if (optColorNumber - 2 > 0)
              c = min(c,optColorNumber - 2);

           // Find the node with the maximum degree of saturation
           root = mergeHeap(heap, pendingUpdates);
           popHeap(heap, root, pendingUpdates);

           // Find the set of free colour for that node
           A[i].U.clear();
           findFreeColour(root,c,A[i].U);

         }

         // Check if the set is empty or not
         if (A[i].U.size() > 0 && *A[i].U.begin() < optColorNumber) {

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
           update(A[i].x, pendingUpdates, A[i].undo, heap, colours);
         } else {
           // Exit for loop if failed
           start = i-1;
           back = true;
           break;
         }
      }

      // Check if the above for loop found a good colouring or not
      if (back) {

        // Add the removed node back to the heap
        if (heap[root].parent == notInQueue) {
           heap[root].parent = maxInt;
           pendingUpdates.push(root);
        }

        if (start >= 0) {

          // Try another colour for the previous node
          root = A[start].x;
          colours[root] = 0;
          revert(heap,A[start].undo,pendingUpdates);
 
        }
      } else {

        // Copy the colouring over
        for (int i = 0; i < size; i++)
           Fopt[i] = colours[i];

        int prevOpt = optColorNumber;
        optColorNumber = A[size-1].colors;

        // Look for where to restart and remove unused colours of the freeColor set
        for (start = 0; A[start].colors != optColorNumber; start++) {
           for (int i = prevOpt; i >= optColorNumber; i--) 
              A[start].U.erase(i);
        }
        start--;
        if (start < 0)
           break;      // optimal is found!

        // revert changes
        for (int i = size-1; i >= start; i--) {
          colours[A[i].x] = 0;
          if (heap[A[i].x].parent == notInQueue) {
             heap[A[i].x].parent = maxInt;
             pendingUpdates.push(A[i].x);
          }
          revert(heap,A[i].undo,pendingUpdates);
        }
        colours[start] = 0;
        root = start;
      }

    }

    // copy Fopt over
    g_->setColour(Fopt); 
 
    return optColorNumber;  
}
