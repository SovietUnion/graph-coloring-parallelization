#include "MatrixGraph.h"

using namespace std;

MatrixGraph::MatrixGraph(unsigned int mSize) {
    if (mSize <= 0) {
        throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize;
    graph.resize(size, vector<bool>(size, 0));
    colours_ = new unsigned int[mSize];
    degrees_ = new unsigned int[mSize];
}

MatrixGraph::MatrixGraph(const vector<vector<bool> > &g, unsigned int mSize) {
    if (mSize <= 0) {
        throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize;
    graph = g;
    colours_ = new unsigned int[mSize];
    degrees_ = new unsigned int[mSize];
}

bool
MatrixGraph::isNeighbours(int a, int b) {
    if (a >= size || b >= size || a < 0 || b < 0) {
        throw "invalid index\n";
    }
    return (graph[a][b] == 1 && graph[b][a] == 1);
}

void
MatrixGraph::neighbours(int a, vector<unsigned int> &neighbours) {
    // Only find neighbours if vertex has positive degree
    if (degrees_[a] > 0) {
        for (int i = 0; i < size; i++) {
	    if (graph[a][i] == 1) {
	    neighbours.push_back(i);
	    }
        }
    }
}

void
MatrixGraph::nonNeighbours(int a, vector<unsigned int> &nonNeighbours) {
    for (int i = 0; i < size; i++) {
        if (graph[a][i] == 0 && a != i && colours_[i] == 0) {
            nonNeighbours.push_back(i);
        }
    }
}

void
MatrixGraph::addNeighbour(int a, int b) {
    graph[a][b] = graph[b][a] = 1;
    degrees_[a]++;
    degrees_[b]++;
}

bool
MatrixGraph::removeNeighbour(int a, int b) {
    if (graph[a][b] == 0 || graph[b][a] == 0) {
        return false;
    }
    graph[a][b] = graph[b][a] = 0;
    degrees_[a]--;
    degrees_[b]--;
    return true;
}

int 
MatrixGraph::getCommonNeighboursCount(int a, int b) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (graph[a][i] == 1 && graph[b][i] == 1) {
            count++;
        }
    }
    return count;
}

int
MatrixGraph::getDegree(int vertex) {
    return degrees_[vertex];
}

unsigned int
MatrixGraph::getSize() {
    return size;
}

int 
MatrixGraph::getMaxDegreeVertex() {
    int maxDegree = -1;
    int maxVertex = 0;
    int tempDegree = 0;
    for (int i = 0; i < size; i++) {
        if (colours_[i] == 0 && (tempDegree = degrees_[i]) > maxDegree) {
            maxDegree = tempDegree;
            maxVertex = i;
        }
    }
    return maxVertex;

}

int 
MatrixGraph::getMaxDegreeVertex(vector<unsigned int> & nn) {
    int maxDegree = -1;
    int maxVertex = 0;
    int tempDegree = 0;

    vector<unsigned int>::iterator it;
    for (it = nn.begin(); it < nn.end(); it++) {
        if (colours_[*it] == 0 && (tempDegree = degrees_[*it]) > maxDegree) {
            maxDegree = tempDegree;
            maxVertex = *it;
        }
    }
    return maxVertex;

}

void 
MatrixGraph::contract(int a, int b) {
    // Grab all b's edges and contract into a
    for (int i = 0; i < size; i++) {
        if (graph[b][i] == 1) {
            graph[a][i] = graph[i][a] = 1;
            graph[b][i] = graph[i][b] = 0;
	    degrees_[a]++;
	    degrees_[b]--;
        }
    }
}

void 
MatrixGraph::setColour(int v, int colour) {
    colours_[v] = colour;
}

unsigned int *
MatrixGraph::getColours() {
    return colours_;
}

void 
MatrixGraph::resetColours() {
    for (int i = 0; i < size; i++)
    {
        colours_[i] = 0;
    }
}


ostream& 
MatrixGraph::print(ostream &os) const
{
    os<<size<<" by "<<size<<" Graph:"<<endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            os << graph[i][j] << " ";
        }
        os << endl;
    }
    return os;
}


