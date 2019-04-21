#ifndef _GUIBASSTOLFI_H_
#define _GUIBASSTOLFI_H_

#include "Sites.h"
#include <vector>
#include <array>

extern "C" {
  double orient2d(double*, double*, double*);
  double incircle(double*, double*, double*, double*);
  void exactinit();
};

using namespace std;

class GuibasStolfi{
  public:
    GuibasStolfi(Sites sites);
    array<Edge*, 2> Delaunay(vector<Node*> nodes);
    void computeTriangles(Edge* le, Edge* re);
    bool valid(Edge* edge, Edge* basel);
    /// The triangles produced by the GS algorithm
    vector<array<int, 3>> triangles;
};
#endif
