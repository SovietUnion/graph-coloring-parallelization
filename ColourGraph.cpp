#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <stdlib.h>
#include <ctime>
#include <string.h>
#include "./Graph/Graph.h"
#include "./Algorithm/Algorithm.h"
#include "./Algorithm/ContractAlgorithm.h"
#include "./Algorithm/ParallelContractAlgorithm.h"
#include "./Graph/MatrixGraph.h"

#define DEFAULT_THREAD_NUM 1

using namespace std;

// Code found on the internet released to public domain :)
// http://typethinker.blogspot.ca/2010/05/timing-section-of-code-in-c.html

class TimedSection {
    char const *d_name;
    timespec d_start;
public:

    TimedSection(char const *name) :
    d_name(name) {
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

int timeColour(Algorithm* a) {
    TimedSection t("Colouring Time Elapsed:");

    int numColors = a->colourGraph();
    return numColors;
}

void printUsage(char* pname) {
    printf("USAGE: %s [GRAPH_FILE_NAME] [-c|-pc|-b|-pb] [THREAD_NUMBER] \n", pname);
    printf("-c:   Contraction algorithm\n");
    printf("-pc:  Parallel contraction algorithm\n");
    printf("-b:   Backtracking algorithm\n");
    printf("-pb:  Parallel Backtracking algorithm\n");
}

// Takes in 3 inputs
// First Input: the name of the file defining the graph 
// Second Input: the name of the algorithm implementation  
// Third Input: the number of thread
// Output: Output the graph colouring to console
//         Std err will give the time, and number of colours used

int main(int argc, char* argv[]) {
    if (argv[1] == NULL || argv[2] == NULL) {
        printUsage(argv[0]);
        return 5;
    }
    char * fname, *option_str, *thread_num_str = NULL;
    fname = argv[1];
    option_str = argv[2];

    // get thread number argument if parallel algorithm is chosen
    if (((strcmp(option_str, "-pc") == 0) || strcmp(option_str, "-pb") == 0) && argc >= 4) {
        thread_num_str = argv[3];
    }
    // if thread_no not specified, default 1 thread
    int thread_no = (thread_num_str == NULL) ? DEFAULT_THREAD_NUM : atoi(thread_num_str);


    ifstream graph(fname);
    int* c;
    Graph* g;
    Algorithm* algo;

    int numNodes = 0;
    int numEdges = 0;

    // read in the graph
    if (graph.is_open()) {
        graph >> numNodes;
        graph >> numEdges;
        g = new MatrixGraph(numNodes);

        string line;
        while (graph.good()) {
            getline(graph, line);
            stringstream ss(line);
            int v1, v2;

            // Read in the node      
            ss >> v1;

            // Read in the neighbours 
            while (ss.good()) {
                ss >> v2;
                g->addNeighbour(v1, v2);
            }
        }
        graph.close();
    } else {
        printf("Can't open file %s\n", fname);
        return 4;
    }

    // Run algorithm based on option argument
    if (strcmp(option_str, "-c") == 0) {
        algo = new ContractAlgorithm(g);
        cerr<<"Colouring graph using \"Sequential contraction algorithm." << endl;        
    } else if (strcmp(option_str, "-pc") == 0) {
        algo = new ParallelContractAlgorithm(g, thread_no);
        cerr<<"Colouring graph using \"Parallel contraction algorithm\" with " << thread_no << " thread(s)." << endl;
    } else if (strcmp(option_str, "-b") == 0) {
        //a = new BSCAlgorithm(g);
    } else if (strcmp(option_str, "-pb") == 0) {
        // a = new ParallelBCAlgorithm(g,thread_no);
    } else {
        printUsage(argv[0]);
        return 16;
    }
    
    cerr<<"Number of colours used: "<<timeColour(algo) << endl;
    algo->printResults();

    delete g;
    delete algo;

    return 0;
}

