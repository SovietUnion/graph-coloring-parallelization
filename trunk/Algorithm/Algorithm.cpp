#include <iostream> 
#include <stdlib.h>
#include "Algorithm.h"

using namespace std;

Algorithm::~Algorithm() {
}

// Print out the results
void Algorithm::printResults() {
    unsigned int *colour = g_->getColours();
    for (int i = 0; i < g_->getSize(); i++) {
        cout << i << " " << (colour[i]) << endl;
    }
}

// Dummy
int Algorithm::colourGraph() {
  return 0;
}
