#include "MatrixGraph2.h"
#include<vector>
#include<set>
#include <stdlib.h>
#include<utility>
#include<algorithm>
using namespace std;


MatrixGraph::MatrixGraph(const vector<vector<bool> > &g, int mSize) {
    if (mSize <= 0) {
        throw "Matrix size needs to be greater than 1.\n";
    }
    size = mSize;
    graph = g;
    colours_ = new int[mSize];
	degrees_ = new int[mSize];
	for(int i=0;i<mSize;i++){
		degrees_[i]=0;
	}
}

//Judge if two vertex are neighbour to each other
bool
MatrixGraph::isNeighbours(int a, int b) {
    if (a >= size || b >= size || a < 0 || b < 0) {
        throw "invalid index\n";
    }
    return (graph[a][b] == 1 && graph[b][a] == 1);
}

//add a neighbour when constructing a graph
void
MatrixGraph::addNeighbour(int a, int b) {
    graph[a][b] = graph[b][a] = 1;
	degrees_[a]++;
    degrees_[b]++;
}

//Get the degree of the vertex
int
MatrixGraph::getDegree(int vertex) {
    return degrees_[vertex];
}

//Get the vertex with MAX degree
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

//Get the size of the graph
int
MatrixGraph::getSize() {
    return size;
}

//Color a vertex v with a colour
void
MatrixGraph::setColour(int v, int colour) {
    colours_[v] = colour;
}

//Get the current colouring
int *
MatrixGraph::getColours() {
    return colours_;
}

//Print the Graph
void 
MatrixGraph::printGraph() {
 for(int i=0;i<getSize();i++){
		for(int j=0;j<getSize();j++){
		    cout<<graph[i][j]<<" ";
		}
     cout<<endl;
	}
}

//Get the neighbours of a vertex
vector<int>
MatrixGraph::getNeighbours(int a) {
	vector<int> neighbours;
    // Only find neighbours if vertex has positive degree
    if (getDegree(a) > 0) {
        for (int i = 0; i < size; i++) {
            if (graph[a][i] == 1) {
            neighbours.push_back(i);
            }
        }
    }
	return neighbours;
}

//Decide which vertex to color next
void
MatrixGraph::getNextVertex(vector<int> &A,vector<pair <int,int>> &B){
	for(int k=0;k<(int)B.size();k++){
		if(isNeighbours(A.back(),B[k].second)==1){
		  B[k].first=getVertexDSATUR(B[k].second);
		}
	}
	make_heap(B.begin(),B.end());
	A.push_back(B[0].second);
	pop_heap(B.begin(),B.end());
	B.pop_back();
}

//Get the set of free colours, which used but not present in neighbor of x
void
MatrixGraph::getFreeColours(int x, set<int> &U){
	          int count=0;
			  set<int> UsedinNeighbour;
			  for(int k=0;k<getSize();k++){
				  if(colours_[k]!=0){
				     U.insert(colours_[k]); 
				  }
				  if((colours_[k]!=0) && isNeighbours(k,x)==1 ){
					 UsedinNeighbour.insert(colours_[k]);
				  }
			  }
              
			  set<int>::iterator iter;
			  if(U.size()>0){
			      iter=U.end();
			      U.insert(*(--iter)+1);
				  if(UsedinNeighbour.size()>0){
                    for(iter=UsedinNeighbour.begin();iter!=UsedinNeighbour.end();iter++){
                         U.erase(U.find(*iter));
                    }
				  }
				  else{;}
			  }
			  else{
				  U.insert(1);
			  }
}
//Get the degree of saturation  vertex x
int
MatrixGraph::getVertexDSATUR(int x){
	vector<int> neighbours;
	neighbours=getNeighbours(x);
	
	set<int> adjacentColor;
	for(int i=0;i<(int)neighbours.size();i++){
		if(colours_[neighbours[i]]!=0){
			adjacentColor.insert(colours_[neighbours[i]]);
		}
	}
		return adjacentColor.size();
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
MatrixGraph::removeColour(int k,set<int> &U){
    set<int> ::iterator it;
    it=U.find(k);
	it=U.erase(it);	
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

