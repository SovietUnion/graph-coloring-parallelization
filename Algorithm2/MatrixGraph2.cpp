#include "MatrixGraph2.h"
#include<vector>
#include<set>

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

vector<int> 
MatrixGraph::getAllDegree(vector<int> &AllDegree){
	   vector<int> B;
       for(int i=0;i<getSize();i++){
       B.push_back(i);
       AllDegree.push_back(getDegree(i));
       } 
	   return B;
}  
//Sort the vertices according to non-increasing degrees

vector<int> 
MatrixGraph::SortbyDegree(vector<int> &AllDegree){
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

//Maintain a sequence of vertex by non-increasing DSATUR
void
MatrixGraph::SortbyDSATUR(vector<int> &B){
	       int temp;
		   for(int i=0;i<(getSize()-1);i++){
           if(getVertexDSATUR(B[i])<getVertexDSATUR(B[i+1])){
              temp=B[i];
              B[i]=B[i+1];
              B[i+1]=temp;
           }
       } 
}
/*

//Get colors(i)
int
MatrixGraph::colors(int i,vector<int> &B){
	 vector<int> Used;
			  int count=0;
			  for(int k=0;k<getSize();k++){
				  if(colours_[k]!=0){
                    Used.push_back(colours_[k]);
				  }
			  }

			 
	          for(int i=0;i<(int)Used.size()-1;i++){
		        if(Used[i]!=Used[i+1]){
			            count++;
		        }
	          }
	                count=count+1;
}
*/
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
	//Count the number of colours around x
	int count=0;
	vector<int> adjacentColor;
	for(int i=0;i<(int)neighbours.size();i++){
	 adjacentColor.push_back(colours_[neighbours[i]]);
	}
	
	sort(adjacentColor.begin(),adjacentColor.end());
	
	for(int i=0;i<(int)adjacentColor.size()-1;i++){
		if(adjacentColor[i]!=adjacentColor[i+1] && adjacentColor[i]!=0){
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
	int m;
	//Using A[i] to index the vertex is to return vertex according to sequence in A[i] when MaxDSATURvertexs are more than one.
	for(m=0;m<getSize();m++){
		if(colours_[A[m]]==0 && getVertexDSATUR(A[m])>MaxDSATUR ){
	    

         MaxDSATUR=getVertexDSATUR(m);
		 MaxDSATURvertex=A[m];
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

