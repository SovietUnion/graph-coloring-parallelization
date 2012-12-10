#include "ParallelContractAlgorithm.h"
#include <pthread.h>
#include <queue>
#include <set>

using namespace std;

ParallelContractAlgorithm::ParallelContractAlgorithm(Graph* g, int threadcount) {
    g_ = g;
    // reset colours array
    g_->resetColours();
    threadcount_=threadcount;
}

// Print out the results
void
ParallelContractAlgorithm::printResults() {
    unsigned int *colour = g_->getColours();
    for (int i = 0; i < g_->getSize(); i++) {
        cout << i << " " << (colour[i]) << endl;
    }
}


//**** NEED TO MODIFY THE GRAPH TO TAKE SLICE & THREAD INTO ACCOUNT
void*
ParallelContractAlgorithm::colourSubGraph(void* slice) {
    int slice_ = (int)((long)slice);
    int size = g_->getSize();
    int from = (slice_ * size)/threadcount_;	
    int to = ((slice_+1) * size)/threadcount_;
    int colournumber = 0;
    bool firstRun = true;
    bool secondRun = true;
    
    // Update the size of slice(note: "to: is upper bound but exclusive)
    size = to-from;
    //cout<<"slice no: "<<slice_<<" from: " <<from<<" to:" << to<<endl;
    while ( size > 0) {
        // determine a vertex x of maximal degree in G
        int x = g_->getMaxDegreeVertex(from, to);

        // color x
        colournumber++;

        g_->setColour(x, colournumber);
        // retrieve set of non-neighbors for x
        vector<unsigned int> nn;
        g_->nonNeighbours(x, nn, from, to);
        vector<bool> b;
        g_->backupVertex(x,b);

        int y = -1;

        //find y belong to NN to be contracted into x
	while ((int) nn.size() > 0) {
            int maxcn = -1;
            int ydegree = -1;
            vector<unsigned int>::iterator it;
            for (it = nn.begin(); it != nn.end(); it++) {
                int cn = g_->getCommonNeighboursCount(*it, x);
                if (cn > maxcn || (cn == maxcn && g_->getDegree(*it) < ydegree)) {
                    y = *it;
                    ydegree = g_->getDegree(y);
                    maxcn = cn;
                }
            }
            if (maxcn == 0) {
                y = g_->getMaxDegreeVertex(nn);
            }
	    // set y colour
            g_->setColour(y, colournumber);
            // contract y to x
            //g_->contract(x, y,from,to);
            g_->contract(x, y);
            size--;

            // update set of NN of non-neighbors of x
            nn.clear();
            g_->nonNeighbours(x, nn,from,to);

        }
        g_->restoreVertex(x,b);
        size--;
    }


    return (void*)colournumber;
}
/*
void*
ParallelContractAlgorithm::detectConflict(void* arg) {

    void** p = (void**) arg;
    int slice_ = (int)((long) p[1]);
    queue<pair<int,int> >* conflicts = (queue<pair<int,int> >*) p[2];
    int size = g_->getSize();
    unsigned int* colours = g_->getColours();
    
    for (unsigned slice1 = 0; slice1 < threadcount_; slice1++) {

      // block 1 from to end
      int from = (slice1 * size)/threadcount_;	
      int to = ((slice1+1) * size)/threadcount_;

      // Update the size of slice(note: "to: is upper bound but exclusive)
      int ss = to-from;

      // slice the block again and compare against all nodes not within the block
      int from2 = from + (slice_ * ss)/threadcount_;	
      int to2 = from + ((slice_+1) * ss)/threadcount_;

      // only check the slice of the block that this thread is responsible
      for (int j = from2; j < to2; j++) {
        // only check outside the block
        for (int k = to; k < size; k++) {
            // there is a conflict if the 2 colours are the same
            // and node j and k are neighbours
            if (colours[j] == colours[k] && g_->isNeighbours(j,k)) {
              pair<int,int> tmp;

              if (g_->getDegree(j) < g_->getDegree(k)){
                 tmp.first = j; tmp.second = k;
              } else {
                 tmp.first = k; tmp.second = j;
              }

              conflicts->push(tmp);
            }
        }
      }
    }

}
*/
void*
ParallelContractAlgorithm::detectConflict(void* arg) {

    void** p = (void**) arg;
    int slice_ = (int)((long) p[1]);
    queue<pair<int,int> >* conflicts = (queue<pair<int,int> >*) p[2];
    int size = g_->getSize();
    unsigned int* colours = g_->getColours();

    int from = (slice_ * size)/threadcount_;	
    int to = ((slice_+1) * size)/threadcount_;    
    queue<unsigned int> n;
    for (unsigned int i = from; i < to; i++) {

        g_->neighbours(i, n, i);
        //vector<unsigned int>::iterator it;
        while(!n.empty()){
            int j = n.front();
            n.pop();
            if (colours[i] == colours[j] ) {
                pair<int,int> tmp;

              if (g_->getDegree(i) < g_->getDegree(j)){
                 tmp.first = i; tmp.second = j;
              } else {
                 tmp.first = j; tmp.second = i;
              }
                
              conflicts->push(tmp);
            }
            
        }
       
    }
    
}

unsigned int
ParallelContractAlgorithm::findFreeColour(int a, int colourNumber) {

  vector<unsigned int> na;
  set<unsigned int> neighbour_colours;
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

  // start a blend new colour becasue there are no free colours
  if (neighbour_colours.empty())
     return colourNumber + 1;
  
  // return a free colour 
  return *neighbour_colours.begin();
}

int
ParallelContractAlgorithm::colourGraph() {

    pthread_t threads[threadcount_];
    queue<pair<int,int> >* conflicts = new queue<pair<int,int> >[threadcount_];
    void*** p = new void**[threadcount_];
    long colourNumber = 0;
    

    // Preassigned the slice number before running thread.
    for (int i=0; i<threadcount_; i++) {
       p[i] = new void*[3];
       p[i][0] = (void*) this;
       p[i][1] = (void*) ((long)i);
       p[i][2] = (void*) &conflicts[i];
     }
    
    // Partition the graph and colour them
    for (int i = 1; i < threadcount_; i++) {
        pthread_create(&threads[i], NULL, this->colour_helper, (void*) p[i]);
    }

    // main thread is a thread too :)
    colourNumber = (long) colourSubGraph((void*) 0);

    //Wait for all the threads to complete 
    for (unsigned int i=1; i<threadcount_; i++) {
        void* tmp;
	pthread_join(threads[i], &tmp);
        if ((long)tmp > colourNumber)
          colourNumber = (long)tmp;
    }

    // Check for conflicts
    // Look for conflicts in parallel
    for (int i = 1; i < threadcount_; i++) {
        pthread_create(&threads[i], NULL, this->conflict_helper, (void*) p[i]);
    }

    // main thread is a thread too :)
    detectConflict((void*) p[0]);

    //Wait for all the threads to complete 
    for (unsigned int i=1; i<threadcount_; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print out conflits
    unsigned int* colours = g_->getColours();
    for (int i = 0; i < threadcount_; i++) {
        // Resolve conflicts one by one
       while (!conflicts[i].empty()) {
         int a = conflicts[i].front().first;
         int b = conflicts[i].front().second;
         int newColourA, newColourB;
         conflicts[i].pop();

         // Make sure it is still a conflict
         if (colours[a] != colours[b])
           continue;

         // Try to find a new colour for node a
         newColourA = findFreeColour(a,colourNumber);

         // if the new colour increases the colour number
         // try to find a new colour for node b
         if (newColourA > colourNumber) {
           newColourB = findFreeColour(b,colourNumber);

           // give node B the new colour
           colours[b] = newColourB;

           // if node B still can't find a free colour
           // increase the colour count
           if (newColourB > colourNumber) {
              colourNumber++;
           }
         } else {
           colours[a] = newColourA;
         }

       }
    }

    for (int i = 0; i < threadcount_; i++)
        delete [] p[i];
    delete [] p;

    return colourNumber;
}
