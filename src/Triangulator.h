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

class Triangulator{
  public:
    Triangulator(Sites problem_sites, unsigned int alg_number);
    std::array<Edge*, 2> verticalCuts(std::vector<Node*> nodes, bool vertical);
    std::array<Edge*, 2> alternatingCuts(std::vector<Node*> nodes, bool vertical);
    Edge* mergeLoop(Edge* basel, bool vertical);
    void computeTriangles(Edge* le);
    bool valid(Edge* edge, Edge* basel, bool vertical);
    Node* kthSmallest(std::vector<Node*> &arr, int l, int r, int k, bool (*less_than)(Node*,Node*));
    int partition(std::vector<Node*> &arr, int l, int r, bool (*less_than)(Node*,Node*));
    void baseCases(std::vector<Node*> &vertices, bool vertical, std::array<Edge*, 2> &edges);
    std::vector<std::array<int, 3>> triangles;
    Sites sites;
};
#endif
