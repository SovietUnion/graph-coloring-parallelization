//BSCAlgorithm.cpp
#include<algorithm>
#include"BSCAlgorithm2.h"
#include"Algorithm2.h"
#include<vector>
#include<set>
#include<time.h>
#include<math.h>
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
        vector<set<int>> U(size);//     Set of free colors
        vector<int> colors(size+1);//Number of used colours
        A.push_back(g_->getMaxDegreeVertex());//The first vertex to color is the MAX degree vertex
    int x=A[0];//Current vertex to be coloured 
        vector<int> OptColour(size);//Store the Colouring results
    bool back; 
        int i=0;
        U[i].insert(1);//Initialize the set of free colour with 1

        //a heap of uncoloured vertex
        //Initializing B
        vector<pair <int,int>> B(size);
        for(int i=0;i<size;i++){//Except x in B
                B[i].first=g_->getVertexDSATUR(i);
                B[i].second=i;
        }       
        vector<pair <int,int>>::iterator itB;//Delete the element which is already put in A
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
        cout<<"A[i]:";
    vector<int>::iterator itt;
        set<int>::iterator it;
        for(itt=A.begin();itt!=A.end();itt++){cout<<*itt<<" ";}
    cout<<endl;
        cout<<"x:"<<x<<" "<<endl;
    cout<<"U[i]:";
        for(it=U[0].begin();it!=U[0].end();it++){cout<<*it<<" ";}
        cout<<endl;
        cout<<"start="<<start<<endl;
        cout<<"====Coloring Started===="<<endl;

        while(start>=0){        
       //x is coloured in the following for-loop. Backtracking is
       //necessary, if U =0; or if an improved colouring has been found
       back=false;//boolean variable for backtracking
       for(i=start;i<size;i++){
             if(i>start){
                         if(i==start+1 && start>0){
                           A=AA[start];
                       B=BB[start]; //A and B are reset to the backtracking state
                         }
           g_->getNextVertex(A,B);
                   cout<<"Asize="<<A.size()<<endl;
                   cout<<"Bsize="<<B.size()<<endl;
                   x=A[i];
                   g_->getFreeColours(x,U[i],colors[i]);//get the set of free colours of x, the set U
                     AA[i]=A;//Remember the sequence in each state
                     BB[i]=B;//Remember the sequence in each state
                   
               //Print A[i]
                   cout<<"A[i]:"<<endl;
           vector<int>::iterator itt;
               for(itt=A.begin();itt!=A.end();itt++){cout<<*itt<<" ";}
                   cout<<endl;
                   
           //Print i,x,U 
           cout<<"i="<<i<<"  x="<<x<<endl; 
                   set<int>::iterator it;
                   cout<<"U: ";
                   for(it=U[i].begin();it!=U[i].end();it++){cout<<*it<<" ";}
                   cout<<endl;
                   cout<<"start="<<start<<endl;           
                 }
                 
                 if(U[i].size()>0){
                   k=*(U[i].begin());    //selected free colour
           g_->setColour(x,k);   // Set x with colour k
           g_->removeColour(k,U[i]);//Remove k from U                     
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
                
                else{           //U=0,backtrack one position
           start=i-1;
           back=true;
           break;          //leaving the for-loop
        }
         }//end of for-loop
   
        if(back){
         if(start>=0){
            x=A[start];  //new starting vertex
                        g_->unColour(x);//uncolour x    
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
                   //remove from U all colours>=optColourNumber;
                   set<int>::iterator it;
                   for(it=U[m].begin();it!=U[m].end();){
               if(*it>=optColourNum){it=U[m].erase(it);}
                           else ++it;
           }//the current colouring is to be improved              
         }//for loop    
            }//else branch
                  //A=AA[start];
                  //B=BB[start]; //A and B are reset to the backtracking state
                  cout<<"*****Backtracking Starts*****"<<endl;
                            /*                      
                    //Check before loop back
                  //  cout<<"A[start] before Backtracking: "<<A[start]<<endl;
                    cout<<"x before Backtracking: "<<x<<endl;
                    cout<<"start before break: "<<start<<endl;
                                        
                    cout<<"U[start] before Backtracking: "<<endl;
                    set<int>::iterator itt;
                    for(itt=U[start].begin();itt!=U[start].end();itt++){
                      cout<<*itt<<" ";
                    }
                    cout<<endl;
                    */                                         
     }//while loop 
              cout<<"final start: "<<start<<endl;
          for(int i=0;i<size;i++){g_->setColour(i,OptColour[i]);}
          return Fopt;   
}//BSCAlgorithm


void main() {
    unsigned int gSize =50;
        double density=0.2;
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
        /*
 // Make it complete graph K7,7
    g->addNeighbour(0, 3);
    g->addNeighbour(0, 6);
    g->addNeighbour(1, 2);
    g->addNeighbour(1, 4);
    g->addNeighbour(2, 6);
    g->addNeighbour(3, 4);
    g->addNeighbour(4, 5);
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
    g->addNeighbour(5, 6);
        */

   //Generating a larger Graph for test
  
   int num_edges=(gSize*(gSize-1)/2);
   pair<int,int> *edges=new pair<int,int>[num_edges];
   pair<int,int> p;
   int ctr=0;
        
   //initialize the vector
   for (int i=0;i<(int)gSize;i++) {
     for (int j=i+1;j<(int)gSize;j++) {
       p.first = i;
       p.second = j;
       edges[ctr] = p;
       ctr++;  
     }
   }

   //initialize random seed
    srand (time(NULL));

   // Start swapping edges randomly
   for (int i = 0; i < num_edges; i++) {
     int j = rand() % num_edges;
     p = edges[j];
     edges[j] = edges[i];
     edges[i] = p;
   } 
        
   // Print out the number of edges in the graph
   int e=ceil(density * num_edges);
   cout <<"Number of Edges="<<e<<endl;

   // Print out the results
     for (int k=0;k<e;k++) {
                g->addNeighbour(edges[k].first,edges[k].second);
     }
   delete [] edges;
   

    Algorithm *a = new BSCAlgorithm(g);
   //run algorithm
        
    cout<<"Number of Color Used: "<<a->colourGraph()<< endl;
    a->printResults();
        //cout<<"Graph:  "<<endl;
        //g->printGraph();
        //Check the coloring

        cout<<endl;
        if(g->checkGraph()==1){cout<<"This colouring is RIGHT!!!!!"<<endl;}
        else{cout<<"This colouring is WRONG!!!!!"<<endl;}

        system("pause");
}