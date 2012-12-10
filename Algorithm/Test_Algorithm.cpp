#include "ParallelContractAlgorithm.h"
#include <pthread.h>
#include <stdint.h> 

int N; //number of threads; 
Algorithm *a; 

// Helper function to pass class function into thread
//void *colourGraph(void *s) {
//   // nasty cast required since pthread create only take void* in 4th param
//   int slice = *((int*)(&s));
//   
//   a->colourGraph(slice);
//}

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
	

    // create N thread to run colour algorithm (later need to use command line arg)
    N = 4;

    a = new ParallelContractAlgorithm(g, N);
    //print out the graph matrix
    cout<<*g;

    int k = a->colourGraph();
        cout<<endl;
    cout<<"graph after running contract algorithm\n"<<*g;
    cout << "number of colours used: " << k << endl;

    //run algorithm
    a->printResults();


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
    a = new ParallelContractAlgorithm(g,N);

    //print out the graph matrix
    cout<<*g;
    //run algorithm
    k = a->colourGraph();
    cout << "number of colours used: " << k << endl;
    a->printResults();
    cout<<endl;
    delete a;
    delete g;

    g = new MatrixGraph(testMatrix, 2);
    g->addNeighbour(0, 1);
    //print out the graph matrix
    cout<<*g;
    a = new ParallelContractAlgorithm(g,N);
    //run algorithm
    k = a->colourGraph();
    cout << "number of colours used: " << k << endl;
    a->printResults();
    cout<<endl;


    g->removeNeighbour(0, 1);
    delete a;
    a = new ParallelContractAlgorithm(g,N);

    //print out the graph matrix
    cout<<*g;
    k = a->colourGraph();
    cout << "number of colours used: " << k << endl;
    a->printResults();



}
