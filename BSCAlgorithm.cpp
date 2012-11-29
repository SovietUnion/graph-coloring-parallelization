//BSCAlgorithm.cpp
#include<algorithm>
#include "BSCAlgorithm.h"
#include"Algorithm.h"
#include<vector>
#include<iostream>
using namespace std;

//Constructor
BSCAlgorithm::BSCAlgorithm(Graph* g) {
    g_ = g;
    for(int i=0;i<g_->getSize();i++){
	g_->setColour((int) i,0);
  }
}

// Print out the results
void
BSCAlgorithm::printResults() {
    int *colour = g_->getColours();
    for (int i = 0; i < g_->getSize(); i++) {
        cout << "vertex " << i << " colour: " << (colour[i]) << endl;
    }
}
/*
int
BSCAlgorithm::colourGraph() {

    return 0;
}
*/

//Test BSC
int
BSCAlgorithm::colourGraph(){
    
    int k ;//colour number
    int Fopt=0;//Optimal colour number
    int start=0;//Starting index
    int optColourNum=g_->getSize()+1;//Optimal number of colours
    vector<int> A;//Set used to store the vertex index
    vector<int> AllDegree;//Set used to store the vertex degree
	vector<int> U;//Variable for the set(sequence) of free colours
	vector<int> colors(g_->getSize());//Number of used colours
    A=g_->SortbyDegree(AllDegree);//Sort the vertexs according to non-decreasing degree
    int x=A[0];//Current vertex to be coloured 
    //int usedColourNum =g_->getUsedColourNum(-1,U);//Number of used colours 
    U.push_back(1);//Initialize the set of free colour with 1
	U.push_back(2);
	U.push_back(3);
	U.push_back(4);
	U.push_back(5);
	


    void getfreeColours(int x,vector<int> U);//set of free colours of x
    bool back; 
	int i=0;

    //Test the initialization before colouring
	/************************************/
	cout<<"A[i]:"<<endl;
    vector<int>::iterator itt;
	  for(itt=A.begin();itt!=A.end();itt++){
		 cout<<*itt<<" ";
	  }
    cout<<endl;
	cout<<"x:"<<endl;
	cout<<x<<endl;
    cout<<"U[i]:"<<endl;
	for(itt=U.begin();itt!=U.end();itt++){
		 cout<<*itt<<" ";
	  }
	cout<<endl;
	cout<<"start1: "<<start<<endl;

	/*************************************/

	//Test Colouring
	  while(start>=0){
       
	   
       //x is coloured in the following for-loop. Backtracking is
       //necessary, if U =0; or if an improved colouring has been found
       back=false;//boolean variable for backtracking
       for(i=start;i<g_->getSize();i++){

         if(i>start){    //for i = start x and U are already available find an uncoloured vertex x of maximal degree of saturation
         g_->getFreeColours(x,U);//get the set of free colours of x,
         g_->sortU(U);//Sort the set of colour(U) according to non-decreasing order
         }

		 cout<<"U.size():"<<U.size();
		 if(U.size()>0){
         k=U[0];        //selected free colour
         g_->setColour(x,k);// Current setting x colour
         g_->removeColour(k,U);//Remove k from U
         //l=g_->getUsedColourNum(i-1);
		 int l=colors[i];
         colors[i+1]=g_->getMax(k,l);
         }
		 else {           //U=0,backtrack one position
         start=i-1;
         back=true;
         break;          //leaving the for-loop
         }
		 cout<<"i="<<i<<endl;
	  }
	     
	   /*******************************
	     cout<<"A[i] before if(back):"<<endl;
         vector<int>::iterator itt;
	     for(itt=A.begin();itt!=A.end();itt++){
		 cout<<*itt<<" ";
	     }
		 cout<<endl;
	   ********************************/
	  // cout<<"back: "<<back<<endl;
	   if(back){
         if(start>=0){
            cout<<"start2: "<<start<<endl;
            x=A[start];  //new starting vertex
            g_->unColour(x);//uncolour x
            g_->getFreeColours(x,U);
         }
       }
	   else{ //in this case the above for-loop has been passed without a break
         Fopt=k;         //storing the currently optimal colouring
         optColourNum=colors[g_->getSize()];
		 int least; 
         for(least=0;least<(int)A.size();least++){//i=least index with F(A[i]) = optColourNumber;
           if(g_->getColour(A[least])==optColourNum)
               break;
         }
		 
         i=least;              
         start=i-1;
         if(start<0){
           break;       //leaving the while-loop
         }
		 for(i=start;i<g_->getSize();i++){//uncolour all vertices A[i] with i>=start;
           g_->unColour(A[i]);
         }
         for(i=0;i<=start;i++){
           x=A[i];
           g_->getFreeColours(x,U);
		   vector<int>::iterator it;
           for(it=U.begin();it<U.end();it++){//remove from U all colours>=optColourNumber;
             if(*it>=optColourNum){
             g_->removeColour(k,U);
             }
           }//the current colouring is to be improved
         }
	   }
     }
    return Fopt;   
}







void main() {
    unsigned int gSize = 7;
    vector < vector<bool> > testMatrix;
    testMatrix.resize(gSize, vector<bool>(gSize, 0));

    //initialize adjacency matrix graph
    Graph* g;
    g = new MatrixGraph(testMatrix, gSize);
    /*
      0 1 2 3 4 5 6
    0 0 1 1 0 1 1 0
    1 1 0 0 1 0 1 1
    2 1 0 0 1 1 1 0
    3 0 1 1 0 0 1 1
    4 1 0 1 0 0 0 1
    5 1 1 1 1 0 0 0
    6 0 1 0 1 1 0 0
     */
    g->addNeighbour(0, 1);
    g->addNeighbour(0, 2);
    g->addNeighbour(0, 4);
    g->addNeighbour(0, 5);
    g->addNeighbour(1, 3);
    g->addNeighbour(1, 5);
    g->addNeighbour(1, 6);
    g->addNeighbour(2, 3);
    g->addNeighbour(2, 4);
    g->addNeighbour(2, 5);
    g->addNeighbour(3, 5);
    g->addNeighbour(3, 6);
    g->addNeighbour(4, 6);

	
    Algorithm *a = new BSCAlgorithm(g);
    //run algorithm
    cout << "number of color: " << a->colourGraph() << endl;
    a->printResults();
	
	cout<<"Graph:  "<<endl;
	g->printGraph();


/************************************************************************/
//The following tests all the needed functions written specially for BSC
/*
	//Test getAllDegree(vector<int> &AllDegree)
	cout<<"Test getAllDegree"<<endl;
	vector<int> AllDegree;//Set used to store the vertex degree
	g->getAllDegree(AllDegree);
	for(int i=0;i<AllDegree.size();i++){
	cout<<AllDegree[i]<<endl;
	}
	
	//Test SortbyDegree(vector<int> &AllDegree)
	cout<<"Test SortbyDegree"<<endl;
	vector<int> B;
	B=g->SortbyDegree(AllDegree);
	for(int i=0;i<B.size();i++){
	cout<<B[i]<<endl;
	}

	//Test setColour
	cout<<"Test setColour"<<endl;
	for(int v=0;v<(int)gSize;v++){
	g->setColour(v, v+1);
	}
	a->printResults();

	//Test getFreeColours
	vector<int> U;
	//Initialized the U set with colours
	vector<int>::iterator itt;
	for(int i=1;i<((int)gSize+2);i++){
	   U.push_back(i);
	}
	//Start to test
	cout<<"Test getFreeColours"<<endl;
	cout<<"The Original U set is:"<<endl;
	for(itt=U.begin();itt!=U.end();itt++){
		 cout<<*itt<<" ";

	}
	     cout<<endl;
	  g->getFreeColours(4,U);
	  cout<<"The Final Result of getFreeColours is:"<<endl;
	  //Print the Final Result of getFreeColours
	  for(itt=U.begin();itt!=U.end();itt++){
		 cout<<*itt<<" ";
	  }
    //cout<<"xxxxx"<<U[0]<<" "<<U[1]<<"  "<<*U.begin();
    
	//Test getUsedColourNum
	cout<<"Test getUsedColourNum"<<endl;
	cout<<"Number of Used Colours  "<<g->getUsedColourNum(4,U)<<endl;

	//Test removeColour
	cout<<"Test removeColour"<<endl;
	g->removeColour(7,U);
	//Print the Final Result of getFreeColours
	cout<<"The Final Result of removeColour is:"<<endl;
	  for(itt=U.begin();itt!=U.end();itt++){
		 cout<<*itt<<" ";
	  }
    cout<<endl;

	//Test unColour
	cout<<"Test unColour"<<endl;
	g->unColour(3);
	g->unColour(5);
	g->unColour(6);
    cout<<"The Final Result of unColour is:"<<endl;
	a->printResults();

	//Test unColour
	cout<<"Test getColour"<<endl;
	cout<<"The colour of 3# is: "<<g->getColour(3)<<endl;
    cout<<"The colour of 4# is: "<<g->getColour(4)<<endl;
	
	*/

	system("pause");
}
