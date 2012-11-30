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

bool
MatrixGraph::isNeighbours(int a, int b) {
    if (a >= size || b >= size || a < 0 || b < 0) {
        throw "invalid index\n";
    }
    return (graph[a][b] == 1 && graph[b][a] == 1);
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
vector<int>
MatrixGraph::getFreeColours(int x,int i,vector<int> &colors){
	       vector<int> U;
		   for(int k=1;k<colors[i]+2;i++){
			   for(int j=0;j<getSize();j++){
				   if(isNeighbours(j,x) && colours_[j]==k) {;}//If the colour appears at the adjacent vertex, skip.					    
				   else{ U.push_back(k);}
			   }            
		   }
           /*
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
		  */
			      sort(U.begin(),U.end());
					/*
					vector<int> ::iterator iter;
					for(iter=U.begin();iter!=U.end();iter++){
		            cout<<*iter<<" "<<endl;
					}
					*/
                   return U;
}

//Get the degree of saturation  vertex x
int
MatrixGraph::getVertexDSATUR(int x){
	vector<int> neighbours;
	neighbours=getNeighbours(x);
	//Count the number of colours around x
	int count=0;
	sort(neighbours.begin(),neighbours.end());
	
	for(int i=0;i<(int)neighbours.size()-1;i++){
		if(neighbours[i]!=neighbours[i+1] && neighbours[i]!=0){
			count++;
		}
	}
	    count=count+1;
		return count;
}

//Get the vertex with maximum degree of saturation
int
MatrixGraph::getMaxDSATURvertex(vector<int> &A){
	//Scan all the vertex to find a uncolored vertex with the Max degree of saturation
	int MaxDSATUR=0;
	int MaxDSATURvertex=0;
	int i;
  //Using A[i] to index the vertex is to return vertex according to sequence in A[i] when MaxDSATURvertexs are more than one.
	for(i=0;i<getSize();i++){
		if(colours_[A[i]]=0 && getVertexDSATUR(A[i])>MaxDSATUR){
         MaxDSATUR=getVertexDSATUR(i);
		 MaxDSATURvertex=A[i];
		}		
	}
	return MaxDSATURvertex;
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

