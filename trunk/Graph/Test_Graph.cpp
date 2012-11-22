#include "MatrixGraph.h"

int main(int argc, char* argv[]) {
   unsigned int gSize = 4;
   vector<vector<bool> > testMatrix;
   testMatrix.resize(gSize, vector<bool>(gSize, 0));
   //initialize adjacency matrix graph
   MatrixGraph graph(testMatrix, gSize);

   // add edges
   graph.addNeighbour(2,1);
   graph.addNeighbour(0,1);
   graph.addNeighbour(3,1);
   cout<<graph.isNeighbours(2,1)<<endl;

   cout<<graph.isNeighbours(2,1)<<endl;

   //print out whole graph in adjacency matrix format
   cout<<graph;

   // print out neighbour of node 1
   vector<unsigned int> neighbours;
   graph.neighbours(1, neighbours);

   vector<unsigned int>::iterator it;
   cout<<"node 1's neighbours: ";
   for (it=neighbours.begin(); it < neighbours.end(); it++)
       cout << " " << *it;
   cout<<endl;
   
   vector<unsigned int> nonneighbours;
   graph.nonNeighbours(1, nonneighbours);
   //vector<unsigned int>::iterator it2;
   cout<<"node 1's nonneighbours: ";
   for (it=nonneighbours.begin(); it < nonneighbours.end(); it++)
       cout << " " << *it;
   cout<<endl;
   cout<<"max degree vertex: "<<graph.getMaxDegreeVertex()<<endl;
   cout<<endl;
   cout<<"degree for vertex 2: "<<graph.getDegree(2)<<endl;
   cout<<endl;
   cout<<"number for common neighbbor for 2 and 3: "<<graph.getCommonNeighboursCount(2,3)<<endl;
   cout<<endl;
   cout<<"number for common neighbbor for 0 and 3: "<<graph.getCommonNeighboursCount(0,3)<<endl;
   cout<<endl;
   graph.addNeighbour(3,2);
   graph.addNeighbour(3,0);
   //print out whole graph in adjacency matrix format
   cout<<graph;
   cout<<"number for common neighbbor for 1 and 3: "<<graph.getCommonNeighboursCount(1,3)<<endl;

   return 0;
}

