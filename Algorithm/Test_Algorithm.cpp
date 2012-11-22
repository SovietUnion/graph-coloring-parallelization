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
    //print out the graph matrix
    cout<<*g;
    //run algorithm
    cout << "number of color: " << a->colourGraph() << endl;
    a->printResults();
    cout<<endl;
     
    delete a;
    delete g;

    g = new MatrixGraph(testMatrix, gSize);
    // Make it complete graph K7,7
    g->addNeighbour(0, 3);
    g->addNeighbour(0, 6);
    g->addNeighbour(1, 2);
    g->addNeighbour(1, 4);
    g->addNeighbour(2, 6);
    g->addNeighbour(3, 4);
    g->addNeighbour(4, 5);
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
    g->addNeighbour(5, 6);
    a = new ContractAlgorithm(g);

    //print out the graph matrix
    cout<<*g;
    //run algorithm
    cout << "number of color: " << a->colourGraph() << endl;
    a->printResults();
    cout<<endl;
    delete a;
    delete g;

    g = new MatrixGraph(testMatrix, 2);
    g->addNeighbour(0, 1);
    //print out the graph matrix
    cout<<*g;
    a = new ContractAlgorithm(g);
    //run algorithm
    cout << "number of color: " << a->colourGraph() << endl;
    a->printResults();
    cout<<endl;

    /*
      0 1
    0 0 0 
    1 0 0
     */
    g->removeNeighbour(0, 1);
    delete a;
    a = new ContractAlgorithm(g);

    //print out the graph matrix
    cout<<*g;
    //run algorithm
    cout << "number of color: " << a->colourGraph() << endl;
    a->printResults();
   

}
