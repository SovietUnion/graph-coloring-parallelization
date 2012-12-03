//BSCAlgorithm.cpp
#include<algorithm>
#include "BSCAlgorithm2.h"
#include"Algorithm2.h"
#include<vector>
#include<set>
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
    vector<int> A;//Set used to store the vertex index (in non-decreasing degree sequence)
    vector<int> AllDegree;//Set used to store the vertex degree
	  vector<set<int>> U(g_->getSize());//Variable for the set(sequence) of free colours
	  vector<int> colors(g_->getSize()+1);//Number of used colours
    A=g_->SortbyDegree(AllDegree);//Sort the vertexs according to non-decreasing degree
    int x=A[0];//Current vertex to be coloured 
    //int usedColourNum =g_->getUsedColourNum(-1,U);//Number of used colours 
    U[0].insert(1);//Initialize the set of free colour with 1
    bool back; 
	  int i=0;

    //Test the initialization before colouring
	
	  cout<<"A[i]:"<<endl;
    vector<int>::iterator itt;
	  set<int>::iterator it;
	  for(itt=A.begin();itt!=A.end();itt++){
		 cout<<*itt<<" ";
	  }
     cout<<endl;
	   cout<<"x:"<<endl;
	   cout<<x<<endl;
     cout<<"U[i]:"<<endl;
	  //it=U[0].begin();
	  //cout<<*it<<" "<<endl;
	
	  for(it=U[0].begin();it!=U[0].end();it++){
		 cout<<*it<<" ";
	   }
	   cout<<"*(U[0].begin())= "<<*(U[0].begin())<<endl;
	  cout<<"start="<<start<<" ======================"<<endl;
     
	  while(start>=0){
       //x is coloured in the following for-loop. Backtracking is
       //necessary, if U =0; or if an improved colouring has been found
       back=false;//boolean variable for backtracking
       for(i=start;i<g_->getSize();i++){
	      if(i>start){
		     g_->SortbyDSATUR(A);
		     x=A[i];
		     cout<<"A[i]:"<<endl;
         vector<int>::iterator itt;
	       for(itt=A.begin();itt!=A.end();itt++){
		       cout<<*itt<<" ";
	       }
           cout<<endl;
           g_->getFreeColours(x,U[x]);//get the set of free colours of x, the set U
       }

           //Print i,x,U
		       set<int>::iterator it;
           cout<<"i="<<i<<"  x="<<x<<endl;
		       cout<<"U: ";
		       for(it=U[x].begin();it!=U[x].end();it++){
	         cout<<*it<<" ";
	         }
		       cout<<endl;


		 if(U.size()>0){
		   k=*(U[x].begin());        //selected free colour
       g_->setColour(x,k);// Set x with colour k
       g_->removeColour(k,U[x]);//Remove k from U		    
		   int l=colors[i];
       colors[i+1]=g_->getMax(k,l);
		   //Print colors(i) 
		   cout<<"colors("<<i<<")="<<colors[i+1]<<endl;
		   //Print U
		   set<int>::iterator it;
		   cout<<"U: ";
		   for(it=U[x].begin();it!=U[x].end();it++){
	       cout<<*it<<" ";
	      }
	     cout<<endl;
		   cout<<"**********"<<endl;
		
     }
		
		 else {           //U=0,backtrack one position
           start=i-1;
           back=true;
           break;          //leaving the for-loop
     }
	 }//end of for-loop
	   // cout<<"back: "<<back<<endl;
    
      if(back){
         if(start>=0){
            //cout<<"start2: "<<start<<endl;
            x=A[start];  //new starting vertex
            g_->unColour(x);//uncolour x
			      g_->getFreeColours(x,U[x]);
         }
      }
	   
	    else{ //in this case the above for-loop has been passed without a break
         Fopt=k;         //storing the currently optimal colouring
         optColourNum=colors[g_->getSize()];
		     cout<<"optColourNum="<<optColourNum<<endl;
		     int least; 
         for(least=0;least<(int)A.size();least++){//i=least index with F(A[i]) = optColourNumber;
           if(g_->getColour(A[least])==optColourNum)
              break;
         } 
		
         i=least;   
		     cout<<"least="<<i<<endl;
		 
         start=i-1;
		     cout<<"start="<<start<<endl;
		 
         if(start<0){
           break;       //leaving the while-loop
         }
		 
		     for(int y=start;y<g_->getSize();y++){//uncolour all vertices A[i] with y>=start;
           g_->unColour(A[y]);
         }
		   
         for(int n=0;n<=start;n++){
           x=A[n];
           g_->getFreeColours(x,U[x]);
		 
		      set<int>::iterator it;
		      for(it=U[x].begin();it!=U[x].end();){//remove from U all colours>=optColourNumber;
             if(*it>=optColourNum){
			          it=U[x].erase(it);
             }
			       else
			       ++it;
          }//the current colouring is to be improved
         }//end of for loop
	    }//end of else branch 
	    //break;
    }//end of while loop
         return Fopt;   
}//end of BSCAlgorithm



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
/*
   	
	cout<<"Graph:  "<<endl;
	g->printGraph();

  //Test getNeighbours(int a)
	vector<int> neighbours;
	neighbours=g->getNeighbours(1);
	cout<<"Test getNeighbours(int a)"<<endl;
	//print each vertex's neighbours
	for(int i=0;i<(int)neighbours.size();i++){
	 cout<<neighbours[i]<<" ";
	}
	cout<<endl;
	//print all color set
	cout<<"setColour"<<endl;
	
	g->setColour(0,1);//i=0
	g->setColour(1,2);//i=1
	g->setColour(5,3);//i=2
	g->setColour(2,2);//i=3
	g->setColour(3,1);//i=4
	g->setColour(4,3);//i=5
	
	a->printResults();
	cout<<endl;
	vector<int> AllDegree;

	//print all degree
	g->getAllDegree(AllDegree);
	for(int i=0;i<(int)AllDegree.size();i++){
	cout<<i<<": "<<AllDegree[i]<<endl;
	}

    //Test getVertexDSATUR(int x)
	for(int v=0;v<(int)gSize;v++){
      cout<<"DSATUR("<<v<<"): "<<g->getVertexDSATUR(v)<<endl;
	}
	
	//Test SortbyDegree(vector<int> &AllDegree)
	cout<<"Test SortbyDegree"<<endl;
	vector<int> B;
	B=g->SortbyDegree(AllDegree);
	for(int i=0;i<(int)B.size();i++){
	cout<<B[i]<<endl;
	}
	
	//Test getMaxDSATURvertex
	cout<<"Test getMaxDSATURvertex:"<<endl;
	cout<<"MaxDSATURvertex: "<<g->getMaxDSATURvertex(B)<<endl;
	
    //Test getFreeColours(int x,int i,vector<int> &colors)
	cout<<"Test getFreeColours"<<endl;
	//print all color set
	cout<<"print all color set:"<<endl;

	  //g->setColour(0,1);//i=0 x=0
	  //g->setColour(1,2);//i=1 x=1
	 // g->setColour(5,3);//i=2 x=5
	 // g->setColour(2,2);//i=3 x=2
	 // g->setColour(3,1);//i=4 x=3
	 // g->setColour(4,3);//i=5 x=4
	  //g->setColour(6,4);//i=6 x=6

	a->printResults();
	cout<<endl;
	
	int x=0; 
	set<int> U;
	set<int> ::iterator it;
	int i=0;
	vector<int> colors;
	
	colors.push_back(0);//colors(-1)
	colors.push_back(1);//colors(0)
    colors.push_back(2);//colors(1)
	colors.push_back(3);//colors(2)
	colors.push_back(3);//colors(3)
	colors.push_back(3);//colors(4)
	colors.push_back(3);//colors(5)
	
	g->getFreeColours(x,U);
	cout<<"U set is:"<<endl;
	for(it=U.begin();it!=U.end();it++){
	cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"U.size()="<<U.size()<<endl;
	
	
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

  /*  
     //Test SortbyDSATUR
     cout<<"Test SortbyDSATUR"<<endl;
   //  g->setColour(0,1);//i=0 x=0
    // g->setColour(1,2);//i=1 x=1
	// g->setColour(5,3);//i=2 x=5
	// g->setColour(2,2);//i=3 x=2
	// g->setColour(3,1);//i=4 x=3
	// g->setColour(4,3);//i=5 x=4
	// g->setColour(6,4);//i=6 x=6
/*
	 a->printResults();

	 vector<int> AllDegree;//Set used to store the vertex degree
	 g->getAllDegree(AllDegree);	
	 vector<int> A;
	 vector<int> B;
	 A=g->SortbyDegree(AllDegree);
	 B=A;
	    g->setColour(0,1);//i=0 x=0
		g->SortbyDSATUR(B);
		cout<<"x=0:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;

	    g->setColour(1,2);//i=1 x=1
	    g->SortbyDSATUR(B);
		cout<<"x=1:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;
	    
		g->setColour(5,3);//i=2 x=5
		  cout<<"x=5:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;

		g->setColour(2,2);//i=3 x=2
		  cout<<"x=2:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;

        g->setColour(3,1);//i=4 x=3
		  cout<<"x=3:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;

        g->setColour(4,3);//i=5 x=4
		  cout<<"x=4:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;

        g->setColour(6,4);//i=6 x=6
		  cout<<"x=6:";
		for(int i=0;i<(int)B.size();i++){
	      cout<<B[i]<<" ";
	    }
	      cout<<endl;

		a->printResults();
     */
	 system("pause");
}
