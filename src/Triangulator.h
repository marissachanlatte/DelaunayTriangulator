#ifndef _TRIANGULATOR_H_
#define _TRIANGULATOR_H_

#include "Sites.h"
#include <vector>
#include <array>

extern "C" {
  double orient2d(double*, double*, double*);
  double incircle(double*, double*, double*, double*);
  void exactinit();
};

using namespace std;

class Triangulator{
  public:
    Triangulator(Sites sites, unsigned int alg_number);
    array<Edge*, 2> verticalCuts(vector<Node*> nodes);
    Edge* mergeLoop(Edge* basel);
    void computeTriangles(Edge* le, Edge* re);
    bool valid(Edge* edge, Edge* basel);
    /// The triangles produced by the GS algorithm
    vector<array<int, 3>> triangles;
};
#endif
