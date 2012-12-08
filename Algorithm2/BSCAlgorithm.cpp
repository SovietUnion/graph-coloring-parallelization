//BSCAlgorithm.cpp
#include<algorithm>
#include"BSCAlgorithm2.h"
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
	int size=g_->getSize();
    vector<int> A;//Set of coloured vertex
	vector<set<int>> U(size);//
	vector<set<int>> freeColors(size);//Variable for the set(sequence) of free colours
	vector<int> colors(size+1);//Number of used colours
	A.push_back(g_->getMaxDegreeVertex());//The first vertex to color is the MAX degree vertex
    int x=A[0];//Current vertex to be coloured 
    //int usedColourNum =g_->getUsedColourNum(-1,U);//Number of used colours 
	vector<int> OptColour(size);//Store the Colouring results

    bool back; 
	int i=0;
	U[i].insert(1);//Initialize the set of free colour with 1
	freeColors[x]=U[i];

	//a heap of uncoloured vertex
	//Initializing B
	vector<pair <int,int>> B(size);
	for(int i=0;i<size;i++){//Except x in B
		B[i].first=g_->getVertexDSATUR(i);
		B[i].second=i;
	}
	
	vector<pair <int,int>>::iterator itB;
    for(itB=B.begin();itB!=B.end();){
		if((*itB).second==x){itB=B.erase(itB);}
	    else ++itB;
	}

    make_heap(B.begin(),B.end());

    //remember A and B in each step
	vector<vector<int>> AA(size,vector<int>(size));
	vector<vector<pair<int,int>>> BB(size,vector<pair<int,int>>(size));

	//Initialize AA & BB
	AA[i]=A;
	BB[i]=B;

    //Test the initialization before colouring
	cout<<"Test the initialization before colouring"<<endl;
	cout<<"OptColour: ";
	for(int n=start;n<size;n++){
		cout<<OptColour[n]<<" ";
	}
	cout<<endl;
	cout<<"A[i]:";
    vector<int>::iterator itt;
	set<int>::iterator it;
	for(itt=A.begin();itt!=A.end();itt++){
		 cout<<*itt<<" ";
	}
    cout<<endl;
	cout<<"x:"<<x<<" "<<endl;
    cout<<"U[i]:";
	for(it=U[0].begin();it!=U[0].end();it++){
		 cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"start="<<start<<endl;
	cout<<"====Coloring Started===="<<endl;

	while(start>=0){
       //x is coloured in the following for-loop. Backtracking is
       //necessary, if U =0; or if an improved colouring has been found
       back=false;//boolean variable for backtracking
       for(i=start;i<size;i++){
	     if(i>start){
           g_->getNextVertex(A,B);
		   x=A[i];
		   g_->getFreeColours(x,U[i]);//get the set of free colours of x, the set U
		   AA[i]=A;//Remember the sequence in each state
		   BB[i]=B;//Remember the sequence in each state
	       //Print A[i]
		   cout<<"A[i]:"<<endl;
           vector<int>::iterator itt;
	       for(itt=A.begin();itt!=A.end();itt++){cout<<*itt<<" ";}
		   cout<<endl;
           //Print i,x,U
		   set<int>::iterator it;
           cout<<"i="<<i<<"  x="<<x<<endl;
		   cout<<"U: ";
		   for(it=U[i].begin();it!=U[i].end();it++){cout<<*it<<" ";}
		   cout<<endl;
		   cout<<"start="<<start<<endl;
		 }

		 if(U[i].size()>0){
		   k=*(U[i].begin());       //selected free colour
           g_->setColour(x,k);   // Set x with colour k
           g_->removeColour(k,U[i]);//Remove k from U	
		   freeColors[x]=U[i];
		   int l=colors[i];
           colors[i+1]=g_->getMax(k,l);

		   //Print colors(i) 
		   cout<<"colors("<<i<<")="<<colors[i+1]<<endl;
		   //Print color used
		   cout<<"vertex color: "<<k<<endl;
		   //Print U
		   set<int>::iterator it;
		   cout<<"U(after remove): ";
		   for(it=U[i].begin();it!=U[i].end();it++){cout<<*it<<" ";}
		   cout<<endl;
		   cout<<"**********"<<endl;
        }
		
		  else {           //U=0,backtrack one position
           start=i-1;
           back=true;
           break;          //leaving the for-loop
          }
	   }//end of for-loop
   
        if(back){
         if(start>=0){
            //cout<<"start2: "<<start<<endl;
            x=A[start];  //new starting vertex
			//U[start].erase(U[start].find(g_->getColour(x)));
			g_->unColour(x);//uncolour x
		    U[i]=freeColors[x];
         }
		}
	        
		else{//in this case the above for-loop has been passed without a break
         Fopt=colors[size];//storing the currently optimal colour number
		 for(int n=0;n<size;n++){ OptColour[n]=g_->getColour(n);}//storing the currently optimal colouring			

         optColourNum=colors[size];
		 cout<<"optColourNum="<<optColourNum<<endl;
		 int least; 
         for(least=0;least<(int)A.size();least++){//i=least index with F(A[i]) = optColourNumber;
			 if(g_->getColour(A[least])==optColourNum) {break;}
         } 

         i=least;   
		 cout<<"least="<<i<<endl;
		 
         start=i-1;
		 cout<<"start="<<start<<endl;
		 
         if(start<0){break;}//leaving the while-loop

		 for(int n=start;n<size;n++){g_->unColour(A[n]);} //uncolour all vertices A[i] with y>=start;
           
		 int m;
         for( m=0;m<=start;m++){
           x=A[m];
           U[m]=freeColors[x];		  
		   //remove from U all colours>=optColourNumber;
		   set<int>::iterator it;
		   for(it=U[m].begin();it!=U[m].end();){
               if(*it>=optColourNum){it=U[m].erase(it);}
			   else ++it;
           }//the current colouring is to be improved
		   freeColors[x]=U[m];
         }//for loop
		  //notice: here we have x = A[start]; U = freeColors(x)
		  //A and B are re to the backtracking state
		   A=AA[start];
		   B=BB[start];
	    }//else branch 
		/*
		cout<<"AA: "<<endl;
		for(int i=0;i<size;i++){		
			for(int j=0;j<(int)AA[i].size();j++){
				cout<<AA[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<"BB: "<<endl;
		for(int i=0;i<size;i++){		
			for(int j=0;j<(int)BB[i].size();j++){
				cout<<BB[i][j].second<<" ";
			}
			cout<<endl;
		}
		break;
		*/
     }//while loop 
          for(int i=0;i<size;i++){g_->setColour(i,OptColour[i]);}
          return Fopt;   
}//BSCAlgorithm


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
	/*
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
	*/

 // Make it complete graph K7,7
    g->addNeighbour(0, 3);
    g->addNeighbour(0, 6);
    g->addNeighbour(1, 2);
    g->addNeighbour(1, 4);
  //  g->addNeighbour(2, 6);
    g->addNeighbour(3, 4);
    g->addNeighbour(4, 5);
    g->addNeighbour(0, 1);
    g->addNeighbour(0, 2);
    g->addNeighbour(0, 4);
    g->addNeighbour(0, 5);
    g->addNeighbour(1, 3);
    g->addNeighbour(1, 5);
    g->addNeighbour(1, 6);
   // g->addNeighbour(2, 3);
    g->addNeighbour(2, 4);
    g->addNeighbour(2, 5);
    g->addNeighbour(3, 5);
   // g->addNeighbour(3, 6);
    g->addNeighbour(4, 6);
    g->addNeighbour(5, 6);
	
    Algorithm *a = new BSCAlgorithm(g);
   //run algorithm
	
    cout<<"Number of Color Used: "<<a->colourGraph()<< endl;
    a->printResults();
	cout<<"Graph:  "<<endl;
	g->printGraph();

 /*  
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
     //Test SortbyDSATUR(&B)
     cout<<"Test SortbyDSATUR(&B)"<<endl;
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
     
     /*  
     //Test SortbyDSATUR()
     
       cout<<"getNextVertex"<<endl;
	   Algorithm *a = new BSCAlgorithm(g);
       g->setColour(0,1);//i=0 x=0
    // g->setColour(1,2);//i=1 x=1
	// g->setColour(5,3);//i=2 x=5
	// g->setColour(2,2);//i=3 x=2
	// g->setColour(3,1);//i=4 x=3
	// g->setColour(4,3);//i=5 x=4
	// g->setColour(6,4);//i=6 x=6
	  
	   a->printResults();
	   cout<<"DSATUR"<<endl;
	   for(int x=0;x<g->getSize();x++){
	     cout<<x<<": "<<g->getVertexDSATUR(x)<<endl;
	   }
	   //g->SortbyDSATUR();
	   vector<int> A;
	   A.push_back(0);
	   A.push_back(5);
	   vector<pair <int,int>> B(g->getSize());
	   for(int i=0;i<g->getSize();i++){
		B[i].first=g->getVertexDSATUR(i);
		B[i].second=i;
	   }
	     make_heap(B.begin(),B.end());
		 g->getNextVertex(A,B);
		for(int i=0;i<(int)A.size();i++){   
         cout<<"A["<<i<<"]"<<": "<<A[i]<<endl;
	   }
	   */
	   system("pause");
}
