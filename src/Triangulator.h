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
    Triangulator(Sites problem_sites, unsigned int alg_number);
    array<Edge*, 2> verticalCuts(vector<Node*> nodes);
    array<Edge*, 4> alternatingCuts(vector<Node*> vertices, bool vertical_flag);
    Edge* mergeLoop(Edge* basel);
    void computeTriangles(Edge* le);
    bool valid(Edge* edge, Edge* basel);
    Node* kthSmallest(vector<Node*> &arr, int l, int r, int k, bool (*less_than)(Node*,Node*));
    int partition(vector<Node*> &arr, int l, int r, bool (*less_than)(Node*,Node*));
    // Node* qselect(vector<Node*> pArray, int k, int li, int hi, bool vertical_flag);
    vector<array<int, 3>> triangles;
    Sites sites;
};
#endif
