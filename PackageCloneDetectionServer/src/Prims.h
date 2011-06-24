#ifndef _Prims_h
#define _Prims_h

#include <vector>

class prims
{
   public:
   prims(int x, int y);

   void createGraph();
   void primsAlgo(std::vector<std::pair<int, int> > &edges);
   void done();
   int **graph;
   int nodes;
private:
   int ROW,COL;
};

#endif
