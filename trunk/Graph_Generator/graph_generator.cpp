#include <iostream> 
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

// Takes in 2 inputs
// First input (n, an integer) specifies the number of nodes
// Second input (d, a decimal number from 0 - 1) specifies the graph densities
// Output: A graph with ceil(d * n * (n-1) / 2) edges
int main(int argc, char* argv[]) { 

   int n = atoi(argv[1]);
   double d = strtod(argv[2],NULL);
   int num_edges = n * (n-1) / 2;

   pair<int,int>* edges = new pair<int,int>[(n*(n-1)/2)];
   pair<int,int> p;
   int ctr = 0;

   // initialize the vector
   for (int i = 0; i < n; i++) {
     for (int j = i+1; j < n; j++) {
       p.first = i;
       p.second = j;
       edges[ctr] = p;
       ctr++;  
     }
   }

   // initialize random seed
   srand (time(NULL));

   // Start swapping edges randomly
   for (int i = 0; i < num_edges; i++) {
     int j = rand() % num_edges;
     p = edges[j];
     edges[j] = edges[i];
     edges[i] = p;
   } 

   // Print out the results
   int e = ceil ( d * num_edges );
   for (int i = 0; i < n; i++) {
     cout << i;
     for (int j = 0; j < e; j++) {
       if (edges[j].first == i) {
         cout << " " << edges[j].second;
       } else if (edges[j].second == i) {
         cout << " " << edges[j].first;
       }
     }
     cout << endl;
   }

   delete [] edges;

   return 0; 
}
