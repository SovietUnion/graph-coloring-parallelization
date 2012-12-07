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
		 for(int i=getSize();i>0;i--){
		   for(int j=0;j<i-1;j++){
              if(AllDegree[B[j]]<AllDegree[B[j+1]]){
                 temp=B[j];
                 B[j]=B[j+1];
                 B[j+1]=temp;
             }
           }
		 }
		 return B;
}   

//Maintain a sequence of vertex by non-increasing DSATUR
void
MatrixGraph::SortbyDSATUR(vector<int> &B){
	   
	       int temp;
		   int k=0;
		   while(colours_[B[k]]!=0){
			   k++;
		   }
		   for(int i=getSize()-k;i>0;i--){
		    for(int j=k;j<i-1;j++){
              if(getVertexDSATUR(B[j])<getVertexDSATUR(B[j+1]) ){
               temp=B[j];
               B[j]=B[j+1];
               B[j+1]=temp;
              }
            } 
		  }
}

//Swap the element in a vector.
void
MatrixGraph::swap(int a, int b, vector<int> &A){
	vector<int>::iterator ita;
	vector<int>::iterator itb;
	int temp;
	
	temp=*ita;
	*ita=*itb;
	*itb=temp;
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
MatrixGraph::removeColour(int k,set<int> &U){
/*	
	set<int> ::iterator it;
	it=U.begin();
	it=U.erase(it);
*/  set<int> ::iterator it;
    it=U.find(k);
	it=U.erase(it);	
/*  
	for(it=U.begin();it!=U.end();){
					if(k==*it){
						 it=U.erase(it);
					}
                   			
					else{ 
						++it;
					}
    }
*/
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

