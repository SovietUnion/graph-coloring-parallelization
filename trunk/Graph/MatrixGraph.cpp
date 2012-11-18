
#include "MatrixGraph.h"

using namespace std;

MatrixGraph::MatrixGraph(unsigned int mSize) {
    if (mSize <= 0) {
        throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize;
    graph.resize(size, vector<bool>(size, 0));
    colours_ = new unsigned int[mSize];
}

MatrixGraph::MatrixGraph(const vector<vector<bool> > &g, unsigned int mSize) {
    if (mSize <= 0) {
        throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize;
    graph = g;
    colours_ = new unsigned int[mSize];
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
    for (int i = 0; i < size; i++) {
        if (graph[a][i] == 1) {
            neighbours.push_back(i);
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
}

bool
MatrixGraph::removeNeighbour(int a, int b) {
    if (graph[a][b] == 0 || graph[b][a] == 0) {
        return false;
    }
    graph[a][b] = graph[b][a] = 0;
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
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (graph[vertex][i] == 1) {
            count++;
        }
    }
    return count;
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
        if (colours_[i] == 0 && (tempDegree = getDegree(i)) > maxDegree) {
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
        if (colours_[*it] == 0 && (tempDegree = getDegree(*it)) > maxDegree) {
            maxDegree = tempDegree;
            maxVertex = *it;
        }
    }
    return maxVertex;

}

void 
MatrixGraph::contract(int a, int b) {
    for (int i = 0; i < size; i++) {
        if (graph[b][i] == 1) {
            graph[a][i] = graph[i][a] = 1;
            graph[b][i] = graph[i][b] = 0;
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

ostream& operator<<(ostream &os, const MatrixGraph &g) {
    for (int i = 0; i < g.size; i++) {
        for (int j = 0; j < g.size; j++) {
            os << g.graph[i][j] << " ";
        }
        os << endl;
    }
    return os;
}
