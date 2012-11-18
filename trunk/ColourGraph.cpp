#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <stdlib.h>
#include <ctime>
#include "../Graph/Graph.h"
#include "../Graph/Algorithm.h"
#include "../Graph/MatrixGraph.h"

using namespace std;

// Code found on the internet released to public domain :)
// http://typethinker.blogspot.ca/2010/05/timing-section-of-code-in-c.html
class TimedSection {
    char const *d_name;
    timespec d_start;
    public:
        TimedSection(char const *name) :
            d_name(name)
        {
            clock_gettime(CLOCK_REALTIME, &d_start);
        }
        ~TimedSection() {
            timespec end;
            clock_gettime(CLOCK_REALTIME, &end);
            double duration = 1e3 * (end.tv_sec - d_start.tv_sec) +
                              1e-6 * (end.tv_nsec - d_start.tv_nsec);
            std::cerr << d_name << '\t' << std::fixed << duration << " ms\n"; 
        }
};

void timeColour (Algorithm* a) {

   TimedSection t("timeColour");

   a->colourGraph();

}

// Takes in 2 inputs
// First Input: the name of the file defining the graph 
// Second Input: the name of the graph implementation
// Third Input: the name of the algorithm implementation  
// Output: Output the graph colouring to console
//         Std err will give the time
int main(int argc, char* argv[]) { 

   ifstream graph(argv[1]);

   int* c; 
   Graph* g; 
   Algorithm* a;

   int numNodes = 0;
   int numEdges = 0;

   // read in the graph
   if ( graph.is_open() ) {

     graph >> numNodes;
     graph >> numEdges;

     if      (string("matrix").compare(argv[2]) == 0)     
       g = new MatrixGraph(numNodes);
     else if (string("list").compare(argv[2]) == 0)     
       g = new MatrixGraph(numNodes);
     else
       return 16;

     string line;

     while ( graph.good() ) {
       getline ( graph, line );
       stringstream ss(line,stringstream::out);
       int a,b;
       
       // Read in the node      
       ss >> a;

       // Read in the neighbours 
       while ( ss.good() ) {
         ss >> b;
         g->addNeighbour(a,b);
       }
     }
 
     graph.close();

   } else {
     printf("Can't open file %s\n", argv[1]);
     return 4;
   }

   // Pick the algorithm
   /*if      (string("vertex").compare(argv[3]) == 0)     
     a = new MatrixGraph(numNodes);
   else if (string("backtrack").compare(argv[3]) == 0)     
     a = new MatrixGraph(numNodes);
   else
     return 16;
   */

   timeColour(a);

   a->printResults();

   delete g;
   delete a;
 
   return 0; 
}
