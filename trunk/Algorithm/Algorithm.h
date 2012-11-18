#ifndef ALGORITHM_H
#define ALGORITHM_H


  using namespace std;

  class Algorithm {

   protected:

    // Store the colours in an array
    int* colours;
    int num_of_nodes;
    Graph* g;
    
   public:

    // Constructor
    Algorithm (int num_nodes, Graph* g);

    // Print out the results
    void printResults();

    // Colour the graph G
    virtual void colourGraph() = 0;
  };

#endif
