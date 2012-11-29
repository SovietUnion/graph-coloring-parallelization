#include "ParallelContractAlgorithm.h"
#include <pthread.h>
#include <queue>

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
    int size = g_->getSize();
    int from = (slice_ * size)/threadcount_;	
    int to = ((slice_+1) * size)/threadcount_;
    int colournumber = 0;
    
    // Update the size of slice(note: "to: is upper bound but exclusive)
    size = to-from;
    cout<<"slice no: "<<slice_<<" from: " <<from<<" to:" << to<<endl;
    while ( size > 0) {
        // determine a vertex x of maximal degree in G
        int x = g_->getMaxDegreeVertex(from, to);

        // color x
        g_->setColour(x, ++colournumber);
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
            g_->contract(x, y,from,to);
            cout << "contracting " << x << " " << y << endl;
            cout << *g_;
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



int
ParallelContractAlgorithm::colourGraph() {

    pthread_t threads[threadcount_];
    queue<pair<int,int> >* conflicts = new queue<pair<int,int> >[threadcount_];
    void*** p = new void**[threadcount_];
    int colourNumber = 0;
    

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

    for (int i = 0; i < threadcount_; i++)
        delete [] p[i];
    delete [] p;

    return colourNumber;
}
