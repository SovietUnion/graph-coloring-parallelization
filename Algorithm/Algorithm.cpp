#include <iostream> 
#include <stdlib.h>
#include "Algorithm.h"

using namespace std;

Algorithm::Algorithm (int num_nodes, Graph* g) {

  this->colours = new int[num_nodes];
  this->num_of_nodes = num_nodes;
  this->g = g;

}

Algorithm::~Algorithm {
  delete [] colours;
}

// Print out the results
void Algorithm::printResults() {
  for (int i = 0; i < num_of_nodes; i++)
    cout << i << " " << colours[i] << endl;
}

========================================
I revised this head file like this：


#include <iostream>
#include <stdlib.h>
#include "Algorithm.h"

using namespace std;

Algorithm::Algorithm (int num_nodes, Graph* g) {

  this->colours = new int[num_nodes];
  this->num_of_nodes = num_nodes;
  this->g_= g;
  for(int i=0;i<num_nodes;i++){
	  this->colours[i]=0;
  }
}

Algorithm::~Algorithm() {
  delete [] colours;
}

// Print out the results

void Algorithm::printResults() {
  for (int i = 0; i < num_of_nodes; i++)
    cout << i << " " << colours[i] << endl;
}

--Yao
=====================================
