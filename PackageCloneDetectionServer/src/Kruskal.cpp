/*
http://orcik.net/programming/kruskal%E2%80%99s-algorithm/

Krushkal Algorithm
Input defined as follow
First give number of nodes n
starting end of edge and then ending end of edge and then cost of edge
 
example:
graph is as follows
1-2 2-5 3-4 4-2 2-3
then
input is
5
1 2 1
2 5 1
3 4 1
4 2 1
2 3 1
 
0  to end the input
*/
 
#include "Kruskal.h"

#define N 100  //N is max nodes
#define M 10000 // M is max edges
#include <iostream>
#include <cstdlib>
using namespace std;
// UNION FIND DATA STRUCURE STARTS
struct data{
    int name;
    int size;
    struct data *home;
};
typedef struct data mydata;
 
class makeunionfind
{
    public :
    mydata S[N];
public:
    makeunionfind(int n)
    {
        for(int i=0;i<n;i++)
        {
            S[i].name=i+1;
            S[i].size=0;
 
            S[i].home=&S[i];
        }
 
    }
 
    void myunion(int a, int b)
    {
        int sizea,sizeb;
        sizea=S[a-1].home->size;
        sizeb=S[b-1].home->size;
        if(sizea>sizeb)
        {
            (S[b-1].home)->home=S[a-1].home;
            S[a-1].size=sizea+sizeb;
 
        }
        else
        {
            (S[a-1].home)->home=S[b-1].home;
            S[b-1].size=sizea+sizeb;
 
        }
 
    }
    int myfind(int a)
    {
        mydata *temp,*temp2,*stoppoint;
        int result;
        temp2=&S[a-1];
        temp=S[a-1].home;
        while(temp2!=temp)
        {
            temp2=temp;
            temp=(*temp2).home;
        }
        result=temp2->name;
        stoppoint=temp2;
        temp2=&S[a-1];
//path compression
        do{
            temp=temp2->home;
            (*temp2).home=stoppoint;
            temp2=temp;
        }while(temp2!=stoppoint);
        return result;
    }
};
//UNION FIND DATA STRUCURE ends
//Krushkal Algo starts
struct edges{
    edge e;
};
int compare(const void *a,const void *b )
{
    edge *a1,*b1;
    a1=(edge*)a;
    b1=(edge*)b;
    if(a1->cost<b1->cost)
        return -1;
    else if (a1->cost>b1->cost)
        return 1;
    else
        return 0;
 
}
void *kruskal(edge *e,int n,int m,int *size,edge *ans)
{
    makeunionfind list(n);
    int (*comp)(const void *a,const void *b );
    int k=0;
    comp=compare;
    qsort((void*)e,m,sizeof(e[0]),comp);
    for(int i=0;i<m;i++)
    {
        int s,f;
        s=(e[i].start)->name;
        f=(e[i].end)->name;
 
        if(list.myfind(s)==list.myfind(f))
        {
            continue;
        }
        else
        {
            list.myunion(s,f);
            ans[k]=e[i];
            k++;
        }
 
    }
    *size=k;
    return ans;
 
}
 
#if 0
int main()
{
    mynode nodes[N];
    edge e[M];
    int n,m; // n is the number of nodes , m is no. of nodes
    cin>>n;
    for(int i=0;i<n;i++){
        nodes[i].name=i+1;
    }
    // temp1 is starting node temp2 is ending node temp3 is cosr
    int temp1,i;
    cin>>temp1;
    for (i=0;temp1!=0;i++)
    {
        int temp2;
        double temp3;
        cin>>temp2;
        cin>>temp3;
        e[i].start=&nodes[temp1-1];
        e[i].end=&nodes[temp2-1];
        e[i].cost=temp3;
        cin>>temp1;
    }
    m=i;
 
    edge ans[M];
    int size;
    kruskal(e,n,m,&size,ans);
 
    for(int p=0;p<size;p++)
    {
        cout<<p+1<<")  start "<<(ans[p].start)->name<<"  end "<<(ans[p].end)->name<<endl;
    }
    return 0;
 
}
#endif
