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

// typedef struct AlternationState {
//   array<Edge*, 2> vEdges;
//   array<Edge*, 2> hEdges;
// } AlternationState;


class Triangulator{
  public:
    Triangulator(Sites problem_sites, unsigned int alg_number);
    array<Edge*, 2> verticalCuts(vector<Node*> nodes, bool vertical);
    array<Edge*, 2> alternatingCuts(vector<Node*> nodes, bool vertical);
    Edge* mergeLoop(Edge* basel, bool vertical);
    void computeTriangles(Edge* le);
    bool valid(Edge* edge, Edge* basel, bool vertical);
    Node* kthSmallest(vector<Node*> &arr, int l, int r, int k, bool (*less_than)(Node*,Node*));
    int partition(vector<Node*> &arr, int l, int r, bool (*less_than)(Node*,Node*));
    void baseCases(vector<Node*> &vertices, bool vertical, array<Edge*, 2> &edges);
    vector<array<int, 3>> triangles;
    Sites sites;
};
#endif
