#include "Triangulator.h"
#include "Edge.h"
#include "QuadEdge.h"
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdlib.h>
#include <iterator>
#include <array>

using namespace std;

bool less_than_xy(Node* x, Node* y){
  array<double, 2> point1 = {x->getPosition(true)[0], x->getPosition(true)[1]};
  array<double, 2> point2 = {y->getPosition(true)[0], y->getPosition(true)[1]};
  return point1 <= point2;
}

bool less_than_yx(Node* x, Node* y){
  array<double, 2> point1 = {x->getPosition(true)[1], x->getPosition(true)[0]};
  array<double, 2> point2 = {y->getPosition(true)[1], y->getPosition(true)[0]};
  return point1 <= point2;
}

// bool less_than_flipped(Node* x, Node* y){
//   array<double, 2> point1 = {x->getPosition(true)[1], -(x->getPosition(true)[0])};
//   array<double, 2> point2 = {y->getPosition(true)[1], -(y->getPosition(true)[0])};
//   return point1 <= point2;
// }

Triangulator::Triangulator(Sites problem_sites, unsigned int alg_number){
  sites = problem_sites;
  vector<Node*> nodes = sites.getNodes();
  if (alg_number == 1){
    array<Edge*, 2> edges;
    edges = verticalCuts(nodes, true);
    computeTriangles(edges[0]);
  }
  else if (alg_number == 2){
    array<Edge*, 2> edges;
    edges = alternatingCuts(nodes, false);
    computeTriangles(edges[0]);
  }
};

void compute_state(array<Edge*, 2> leftD, array<Edge*, 2> rightD, bool vertical,
  Edge* &ldo, Edge* &rdo, Edge* &basel){
    ldo = leftD[0];
    Edge* ldi = leftD[1];
    Edge* rdi = rightD[0];
    rdo = rightD[1];

    /// Compute the lower common tangent of leftD and rightD
    while (true){
      if (orient2d(rdi->org()->getPosition(vertical),
                   ldi->org()->getPosition(vertical),
                   ldi->dest()->getPosition(vertical)) > 0){ldi = ldi->Lnext();}
      else if (orient2d(ldi->org()->getPosition(vertical),
                        rdi->dest()->getPosition(vertical),
                        rdi->org()->getPosition(vertical)) > 0){rdi = rdi->Rprev();}
      else{break;}

    }

    basel = rdi->sym()->connect(ldi);
    /// Create a first cross edge basel from rdi.org to ldi.org
    if (ldi->org() == ldo->org()){
      ldo = basel->sym();
    }
    if (rdi->org() == rdo->org()){
      rdo = basel;
    }
}


void Triangulator::baseCases(vector<Node*> &vertices,
                             bool vertical,
                             array<Edge*, 2> &edges){
  vertices = sites.sortNodes(vertices, vertical);
  if (vertices.size() == 2){
    Edge* a = Edge::makeEdge();
    a->setOrg(vertices[0]);
    a->setDest(vertices[1]);

    edges[0] = a;
    edges[1] = a->sym();
  }
  else if (vertices.size() == 3){
    Edge* a = Edge::makeEdge();
    Edge* b = Edge::makeEdge();
    a->sym()->splice(b);

    a->setOrg(vertices[0]);
    a->setDest(vertices[1]);

    b->setOrg(vertices[1]);
    b->setDest(vertices[2]);

    /// CCW Test
    double ccw = orient2d(vertices[0]->getPosition(vertical),
                          vertices[1]->getPosition(vertical),
                          vertices[2]->getPosition(vertical));

    if (ccw > 0){
      b->connect(a);
      edges[0] = a;
      edges[1] = b->sym();
    }
    else if (ccw < 0){
      Edge* c = b->connect(a);
      edges[0] = c->sym();
      edges[1] = c;
    }
    else{
      edges[0] = a;
      edges[1] = b->sym();
    }
  }
}

void Triangulator::computeTriangles(Edge* le){
  vector<Edge*> stack;
  vector<vector<Edge*>> visited;

  /// Initialize stack and current
  stack.push_back(le);

  while(stack.size() > 0){
    bool vis_bool = false;
    /// Pop off of stack to current
    Edge* curr_edge = stack.back();
    stack.pop_back();
    /// Check if visited (if visited repeat above)
    for (auto const& face: visited){
      if(find(face.begin(), face.end(), curr_edge) != face.end()){
        vis_bool = true;
        break;
      }
    }
    if (vis_bool){
      continue;
    }
    vector<Edge*> curr_face;
    curr_face.push_back(curr_edge);
    stack.push_back(curr_edge->sym());
    Edge* next_edge = curr_edge->Lnext();
    /// Traverse inside of face & add syms to stack
    while(next_edge->org() != curr_edge->org()){
      curr_face.push_back(next_edge);
      stack.push_back(next_edge->sym());
      next_edge = next_edge->Lnext();
    }
    /// Add inside edges to visited
    visited.push_back(curr_face);
  }
  /// Convert from edges to triangles
  for (auto const& face: visited){
    if (face.size() == 3){
      this->triangles.push_back({face[0]->org()->getID() + 1,
                                 face[1]->org()->getID() + 1,
                                 face[2]->org()->getID() + 1});
    }
  }
};

array<Edge*, 2> Triangulator::verticalCuts(vector<Node*> vertices, bool vertical){
  array<Edge*, 2> edges;
  if (vertices.size() <= 3){
    baseCases(vertices, vertical, edges);
  }
  else{
    /// Split in half with vertical cut
    kthSmallest(vertices, 0, vertices.size() - 1, vertices.size()/2, &less_than_xy);

    vector<Node*> left_half(vertices.begin(),
                            vertices.begin() + vertices.size() / 2);
    vector<Node*> right_half(vertices.begin() + vertices.size() / 2,
                             vertices.end());

    array<Edge*, 2> leftD = verticalCuts(left_half, vertical);
    array<Edge*, 2> rightD = verticalCuts(right_half, vertical);

    Edge *ldo, *rdo,* basel;
    compute_state(leftD, rightD, vertical, ldo, rdo, basel);

    basel = mergeLoop(basel, vertical);

    edges[0] = ldo;
    edges[1] = rdo;
  }
  return edges;
};

array<Edge*, 2> Triangulator::alternatingCuts(vector<Node*> vertices, bool vertical){
  array<Edge*, 2> edges;
  if (vertices.size() <= 3){
    baseCases(vertices, vertical, edges);
  }
  else{
    vertical = !vertical;
    /// Split in half with vertical cut
    auto less_than = vertical ? &less_than_xy : &less_than_yx;
    kthSmallest(vertices, 0, vertices.size() - 1, vertices.size()/2, less_than);

    vector<Node*> left_half(vertices.begin(),
                            vertices.begin() + vertices.size() / 2);
    vector<Node*> right_half(vertices.begin() + vertices.size() / 2,
                             vertices.end());

    array<Edge*, 2> leftD = verticalCuts(left_half, vertical);
    array<Edge*, 2> rightD = verticalCuts(right_half, vertical);

    Edge *ldo, *rdo,* basel;
    compute_state(leftD, rightD, vertical, ldo, rdo, basel);

    basel = mergeLoop(basel, vertical);

    /// Update rdo and ldo to be the convex hull handles of the rotated triangulation
    /// Need to use opposite comparison
    less_than = vertical ?  &less_than_yx : &less_than_xy;
    /// Take current ldo and walk around until it becomes rotated ldo
    while (true){
      /// if node before and after org are greater than org then we're done
      /// less than should be opposite of what we used above
      if (less_than(ldo->org(), ldo->dest()) &&
          less_than(ldo->org(), ldo->Oprev()->dest())){
            break;
          }
      ldo = ldo->sym()->Onext();
    }
    /// Take current rdo and walk around until it becomes rotated rdo
    while (true){
      /// if node before and after org are less than org then we're done
      /// less than should be opposite of what we used above
      if (less_than(rdo->dest(), rdo->org()) &&
          less_than(rdo->Onext()->dest(), rdo->org())){
            break;
          }
      rdo = rdo->sym()->Oprev();
    }
    edges[0] = ldo;
    edges[1] = rdo;
  }
  return edges;
};


bool Triangulator::valid(Edge* edge, Edge* basel, bool vertical){
  return (orient2d(edge->dest()->getPosition(vertical),
                  basel->dest()->getPosition(vertical),
                  basel->org()->getPosition(vertical)) > 0);
};


Edge* Triangulator::mergeLoop(Edge* basel, bool vertical){
  /// Merge loop
  while (true){
    /// Locate the first L point to be encountered by the rising bubble
    /// and delete L edges out of basel.dest() that fail the circle test
    Edge* lcand = basel->sym()->Onext();
    if (valid(lcand, basel, vertical)){
      while(incircle(basel->dest()->getPosition(vertical),
                     basel->org()->getPosition(vertical),
                     lcand->dest()->getPosition(vertical),
                     lcand->Onext()->dest()->getPosition(vertical)) > 0){
                       Edge* tmp = lcand->Onext();
                       lcand->deleteEdge();
                       lcand = tmp;
                      }
                   }
    /// Symmetrically locate the first R point to be hit, and delete R edges
    Edge* rcand = basel->Oprev();
    if (valid(rcand, basel, vertical)){
      while(incircle(basel->dest()->getPosition(vertical),
                     basel->org()->getPosition(vertical),
                     rcand->dest()->getPosition(vertical),
                     rcand->Oprev()->dest()->getPosition(vertical)) > 0){
                       Edge* tmp = rcand->Oprev();
                       rcand->deleteEdge();
                       rcand = tmp;
                      }
                  }
    /// If both lcand and r cand are invalid then basel is upper common tangent
    if ((!valid(lcand, basel, vertical)) && (!valid(rcand, basel, vertical))){
      break;
    }
    /// The next cross edge is to be connected to either lcand.dest or rcand.dest
    /// if both are valid, then choose appropriate one via incircle test.
    if (!valid(lcand, basel, vertical) ||
        (valid(rcand, basel, vertical) && (incircle(lcand->dest()->getPosition(vertical),
                                         lcand->org()->getPosition(vertical),
                                         rcand->org()->getPosition(vertical),
                                         rcand->dest()->getPosition(vertical)) > 0))){
                                           /// Add cross edge basel from
                                           /// rcand.dest to basel.dest
                                           basel = rcand->connect(basel->sym());
                                         }
    else{
      /// Add cross edge basel from basel.org to lcand.dest
      basel = basel->sym()->connect(lcand->sym());
    }
  }
  return basel;
};

int Triangulator::partition(vector<Node*> &arr, int l, int r, bool (*less_than)(Node*,Node*))
/// Taken from https://www.geeksforgeeks.org/quickselect-algorithm/ and slightly modified
{
    Node* x = arr[r];
    int i = l;
    for (int j = l; j <= r - 1; j++) {
      if (less_than(arr[j], x)){
        swap(arr[i], arr[j]);
        i++;
      }
    }
    swap(arr[i], arr[r]);
    return i;
};


Node* Triangulator::kthSmallest(vector<Node*> &arr, int l, int r, int k,
  bool (*less_than)(Node*,Node*)){
  /// Taken from https://www.geeksforgeeks.org/quickselect-algorithm/ and slightly modified
  // Partition the array around last
  // element and get position of pivot
  // element in sorted array
  int index = partition(arr, l, r, less_than);

  // If position is same as k
  if (index - l == k - 1)
      return arr[index];

  // If position is more, recur
  // for left subarray
  if (index - l > k - 1)
      return kthSmallest(arr, l, index - 1, k, less_than);

  // Else recur for right subarray
  return kthSmallest(arr, index + 1, r,
                      k - index + l - 1, less_than);
};
