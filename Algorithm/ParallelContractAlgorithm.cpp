#include "ParallelContractAlgorithm.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
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
        cout << "vertex " << i << " colour: " << (colour[i]) << endl;
    }
}


//**** NEED TO MODIFY THE GRAPH TO TAKE SLICE & THREAD INTO ACCOUNT
void*
ParallelContractAlgorithm::colourSubGraph(void* slice) {
    cout<<(long)slice<<endl;
    int slice_ = (int)((long)slice);
    int colournumber = 0;
    int size = g_->getSize();
    while (size > 0) {
        // determien a vertex x of maximal degree in G
        int x = g_->getMaxDegreeVertex();
        // color x
        g_->setColour(x, ++colournumber);
        // retrieve set of non-neighbors for x
        vector<unsigned int> nn;
        g_->nonNeighbours(x, nn);

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
            g_->contract(x, y);
            size--;

            // update set of NN of non-neighbors of x
            nn.clear();
            g_->nonNeighbours(x, nn);

        }
        size--;
    }
    return (void*)colournumber;
}



int
ParallelContractAlgorithm::colourGraph() {

    pthread_t threads[threadcount_];
    pair<ParallelContractAlgorithm*,int>* p = new pair<ParallelContractAlgorithm*,int>[threadcount_];;
    int colourNumber = 0;
    

    // Preassigned the slice number before running thread.
    for (int i=1; i<threadcount_; i++) {
       p[i].first = this;
       p[i].second = i;
       //args[i].to= ((i+1) * (SIZE-1))/N +1;
     }
    
    // Partition the graph and colour them
    for (int i = 1; i < threadcount_; i++) {
	pthread_create(&threads[i], NULL, this->colour_helper, &p[i]);
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
    //queue<pair<int,int>>* conflicts = new queue<pair<int,int>>[threadcount_];

    return colourNumber;
}