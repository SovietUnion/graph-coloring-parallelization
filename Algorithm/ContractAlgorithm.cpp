#include "ContractAlgorithm.h"

using namespace std;

ContractAlgorithm::ContractAlgorithm(Graph* g) {
    g_ = g;
    // reset colours array
    g_->resetColours();
}

// Print out the results
void
ContractAlgorithm::printResults() {
    unsigned int *colour = g_->getColours();
    for (int i = 0; i < g_->getSize(); i++) {
        cout << "vertex " << i << " colour: " << (colour[i]) << endl;
    }
}

int
ContractAlgorithm::colourGraph() {
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
    
    return colournumber;
}

