/*
http://www.syntax-example.com/Code/prims-algorithm-minimum-spanning-tree-1027.aspx
*/

#include "Prims.h"
#include <iostream>
#include <limits.h>

#define infi INT_MAX //infi for infinity

typedef int *intPtr;

prims :: prims(int x, int y) : ROW(x), COL(y){
      graph = new intPtr[x];
      for (int i = 0; i < x; i++)
          graph[i] = new int[y];
     for(int i=0;i<ROW;i++)
       for(int j=0;j<COL;j++)
     graph[i][j]=0;
}

void prims :: createGraph(){
    int i,j;
    std::cout<<"Enter Total Nodes : ";
    std::cin>>nodes;
    std::cout<<"\n\nEnter Adjacency Matrix : \n";
    for(i=0;i<nodes;i++)
        for(j=0;j<nodes;j++)
        std::cin>>graph[i][j];

    //Assign infinity to all graph[i][j] where weight is 0.
    for(i=0;i<nodes;i++){
        for(j=0;j<nodes;j++){
           if(graph[i][j]==0)
          graph[i][j]=infi;
        }
    }
}

void prims :: done()
{
   int i, j;
    //Assign infinity to all graph[i][j] where weight is 0.
    for(i=0;i<nodes;i++){
        for(j=0;j<nodes;j++){
           if(graph[i][j]==0)
          graph[i][j]=infi;
        }
    }
}


void prims :: primsAlgo(std::vector<std::pair<int, int> > &edges){
    int selected[ROW],i,j,ne; //ne for no. of edges
    int _false=0,_true=1,min,x,y;

    for(i=0;i<nodes;i++)
       selected[i]=_false;

    selected[0]=_true;
    ne=0;

    while(ne < nodes-1){
       min=infi;

       for(i=0;i<nodes;i++)
       {
          if(selected[i]==_true){
         for(j=0;j<nodes;j++){
            if(selected[j]==_false){
               if(min > graph[i][j])
               {
               min=graph[i][j];
               x=i;
               y=j;
               }
            }
         }
          }
       }
       selected[y]=_true;
	edges.push_back(std::pair<int, int>(x, y));
       ne=ne+1;
    }
}

#if 0
void main(){
    prims MST;
    clrscr();
    cout<<"\nPrims Algorithm to find Minimum Spanning Tree\n";
    MST.createGraph();
    MST.primsAlgo();
    getch();
} 
#endif
