#ifndef _Kruskal_h
#define _Kruskal_h

#include <iostream>

struct node{
    int name;
};
typedef struct node mynode;
class edge
{
    public :
    mynode *start,*end;
    double cost;
    edge()
    {
        start=NULL;
        end=NULL;
        cost=0;
    }
};


void *kruskal(edge *e,int n,int m,int *size,edge *ans);

#endif
