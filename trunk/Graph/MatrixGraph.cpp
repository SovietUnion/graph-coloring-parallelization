#include "MatrixGraph.h"

using namespace std;

MatrixGraph::MatrixGraph(int mSize) {
    if (mSize <= 0) {
	throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize; 
    graph.resize(size, vector<int>(size, 0));
}

MatrixGraph::MatrixGraph(vector<vector<int> > g, int mSize) {
    if (mSize <= 0) {
	throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize; 
    graph = g;
}

bool
MatrixGraph::isNeighbours(int a, int b) {
    if (a >= size || b >= size || a < 0 || b < 0){
        throw "invalid index\n";
    }
    return (graph[a][b] == 1 && graph[b][a] == 1);
}

vector<int>
MatrixGraph::neighbours(int a) {
    vector<int> neighbours;
    for (int i = 0; i < size; i++) {
	if (graph[a][i] == 1) {
           neighbours.push_back(i);
        }
     }	    
    return neighbours;
}

void
MatrixGraph::addNeighbour(int a, int b) {
    graph[a][b] = graph[b][a] = 1;
}

bool 
MatrixGraph::removeNeighbour(int a, int b) {
    if (graph[a][b] == 0 || graph[b][a] == 0 ) {
       return false;
    }
    graph[a][b] = graph[b][a] = 0;
    return true;
}

ostream& operator<<(ostream &os, const MatrixGraph &g){
	for (int i = 0; i < g.size; i++) {
		for (int j = 0; j < g.size; j++) {
                    os<<g.graph[i][j]<<" ";  
		}
		os<<endl;
	}
	return os;
}

/* code to test if matrix graph works */
int main(int argc, char* argv[]) {
   int gSize = 4;
   vector<vector<int> > testMatrix;
   testMatrix.resize(gSize, vector<int>(gSize, 0));
   //initialize adjacency matrix graph
   MatrixGraph graph(testMatrix, gSize);

   // add edges
   graph.addNeighbour(2,1);
   graph.addNeighbour(0,1);
   graph.addNeighbour(3,1);
   cout<<graph.isNeighbours(2,1)<<endl;

   graph.removeNeighbour(2,1);
   cout<<graph.isNeighbours(2,1)<<endl;

   //print out whole graph in adjacency matrix format
   cout<<graph;

   // print out neighbour of node 1
   vector<int> neighbours = graph.neighbours(1);
   vector<int>::iterator it;
   cout<<"node 1's neighbours: ";
   for (it=neighbours.begin(); it < neighbours.end(); it++)
       cout << " " << *it;
   cout<<endl;

   return 0;
}
