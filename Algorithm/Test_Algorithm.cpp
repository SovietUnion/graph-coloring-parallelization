#include "ContractAlgorithm.h"
 
int main(int argc, char* argv[]) {
    unsigned int gSize = 7;
    vector < vector<bool> > testMatrix;
    testMatrix.resize(gSize, vector<bool>(gSize, 0));

    //initialize adjacency matrix graph
    Graph* g;
    g = new MatrixGraph(testMatrix, gSize);
    /*
      0 1 2 3 4 5 6
    0 0 1 1 0 1 1 0
    1 1 0 0 1 0 1 1
    2 1 0 0 1 1 1 0
    3 0 1 1 0 0 1 1
    4 1 0 1 0 0 0 1
    5 1 1 1 1 0 0 0
    6 0 1 0 1 1 0 0 
     */
    g->addNeighbour(0, 1);
    g->addNeighbour(0, 2);
    g->addNeighbour(0, 4);
    g->addNeighbour(0, 5);
    g->addNeighbour(1, 3);
    g->addNeighbour(1, 5);
    g->addNeighbour(1, 6);
    g->addNeighbour(2, 3);
    g->addNeighbour(2, 4);
    g->addNeighbour(2, 5);
    g->addNeighbour(3, 5);
    g->addNeighbour(3, 6);
    g->addNeighbour(4, 6);

	
    Algorithm *a = new ContractAlgorithm(g);
    //run algorithm
    cout << "number of color: " << a->colourGraph() << endl;
    a->printResults();

}
