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
            graph[a][i] = 1;
            degrees_[a]++;
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

/**Parallel functions*/
int 
MatrixGraph::getMaxDegreeVertex(int from, int to) {
    int maxDegree = -1;
    int maxVertex = 0;
    int tempDegree = 0;
    for (int i = from; i < to; i++) {
        if (colours_[i] == 0 && (tempDegree = degrees_[i]) > maxDegree) {
            maxDegree = tempDegree;
            maxVertex = i;
        }
    }
    return maxVertex;

}


void
MatrixGraph::nonNeighbours(int a, vector<unsigned int> &nonNeighbours, int from, int to) {
    for (int i = from; i < to; i++) {
        if (graph[a][i] == 0 && a != i && colours_[i] == 0) {
            nonNeighbours.push_back(i);
        }
    }
}

/**Below parallel functions might be removed, but need more testing*/
int 
MatrixGraph::getCommonNeighboursCount(int a, int b, int from, int to) {
    int count = 0;
    for (int i = from; i < to; i++) {
        if (graph[i][a] == 1 && graph[i][b] == 1) {
            count++;
        }
    }
    return count;
}

void 
MatrixGraph::contract(int a, int b, int from, int to) {
    // Grab all b's edges and contract into a
    for (int i = from; i < to; i++) {
        if (graph[b][i] == 1) {
            graph[a][i] = 1;
            degrees_[a]++;
        }
    }
}

int
MatrixGraph::getDegree(int vertex, int from, int to) {
    int count = 0;
    for (int i = from; i < to; i++) {
        if (graph[i][vertex] == 1)
            count++;
    }
    return count;
}

void
MatrixGraph::backupVertex(int a, vector<bool> &b) {
   b = graph[a];
}

void
MatrixGraph::restoreVertex(int a, vector<bool> b) {
   graph[a] = b;
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


/************************************************************************/
//For BSC by Yao
#include "MatrixGraph.h"
#include<vector>
#include<algorithm>
using namespace std;


MatrixGraph::MatrixGraph(const vector<vector<bool> > &g, int mSize) {
    if (mSize <= 0) {
        throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize;
    graph = g;
    colours_ = new int[mSize];
}

void
MatrixGraph::addNeighbour(int a, int b) {
    graph[a][b] = graph[b][a] = 1;
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

int
MatrixGraph::getSize() {
    return size;
}

void
MatrixGraph::setColour(int v, int colour) {
    colours_[v] = colour;
}

int *
MatrixGraph::getColours() {

    return colours_;
}
/*
ostream& operator<<(ostream &os, const MatrixGraph &g) {
    for (int i = 0; i < g.size; i++) {
        for (int j = 0; j < g.size; j++) {
            os << g.graph[i][j] << " ";
        }
        os << endl;
    }
    return os;
}*/

void 
MatrixGraph::printGraph() {
 for(int i=0;i<getSize();i++){
		for(int j=0;j<getSize();j++){
		    cout<<graph[i][j]<<" ";
		}
     cout<<endl;
	}
}

	//For BSC by Yao
//Get all the degrees

vector<int> MatrixGraph::getAllDegree(vector<int> &AllDegree){
	   vector<int> B;
       for(int i=0;i<getSize();i++){
       B.push_back(i);
       AllDegree.push_back(getDegree(i));
       } 
	   return B;
}  
//Sort the vertices according to non-increasing degrees

vector<int> MatrixGraph::SortbyDegree(vector<int> &AllDegree){
	       vector<int> B;
		   B=getAllDegree(AllDegree);
           int temp;
		   for(int i=0;i<(getSize()-1);i++){
           if(AllDegree[B[i]]<AllDegree[B[i+1]]){
              temp=B[i];
              B[i]=B[i+1];
              B[i+1]=temp;
           }
       } 
		   return B;
}     
//Get the set of free colours, which used but not present in neighbor of x
void
MatrixGraph::getFreeColours(int x,vector<int> &U){
	        
           for(int i=0;i<=x;i++){
			 vector<int> ::iterator it;
		     for(it=U.begin();it!=U.end();){
					
					if(colours_[i]==*it){
						 it=U.erase(it);
					}
                   			
					else{ 
						++it;
					}
					//Print Current U
                    vector<int> ::iterator iter;
					for(iter=U.begin();iter!=U.end();iter++){
		            cout<<*iter<<" ";
					}
					cout<<endl;
		     }
		  }         
			        sort(U.begin(),U.end());
					/*
					vector<int> ::iterator iter;
					for(iter=U.begin();iter!=U.end();iter++){
		            cout<<*iter<<" "<<endl;
					}
					*/
           
}
			
	
                  
//Get the number of used colour
int 
MatrixGraph::getUsedColourNum(int i,vector<int> &U){
     int l,k;
     if(i>0){
       l=getUsedColourNum(i-1,U);
       k=U[0];
       if(l>k){
       return l;
       }
       else{
       return k;
       }
     }
	 else {
	   return 0;
    }
}
int
MatrixGraph::getMax(int a,int b){
	if(a>b){
	  return a;
	}
	else{
	  return b;
	}
}
//Remove colour k
void
MatrixGraph::removeColour(int k,vector<int> &U){
	vector<int> ::iterator it;
	for(it=U.begin();it!=U.end();){
					if(k==*it){
						 it=U.erase(it);
					}
                   			
					else{ 
						++it;
					}
    }
}
//Sort the set of colour(U) according to non-decreasing order
void
MatrixGraph::sortU(vector<int> &U){
   sort(U.begin(),U.end());
}
//Uncolour a vertex
void
MatrixGraph::unColour(int x){
   colours_[x]=0;
}
//Get the colour of vertex with index x
int 
MatrixGraph::getColour(int x) {
	return colours_[x];
}


/***************************************************************/
