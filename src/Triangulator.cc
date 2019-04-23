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
  array<double, 2> point1 = {x->getPosition()[0], x->getPosition()[1]};
  array<double, 2> point2 = {y->getPosition()[0], y->getPosition()[1]};
  return point1 <= point2;
}

bool less_than_yx(Node* x, Node* y){
  array<double, 2> point1 = {x->getPosition()[1], x->getPosition()[0]};
  array<double, 2> point2 = {y->getPosition()[1], y->getPosition()[0]};
  return point1 <= point2;
}

bool less_than_flipped(Node* x, Node* y){
  array<double, 2> point1 = {x->getPosition()[1], -(x->getPosition()[0])};
  array<double, 2> point2 = {y->getPosition()[1], -(y->getPosition()[0])};
  return point1 <= point2;
}

Triangulator::Triangulator(Sites problem_sites, unsigned int alg_number){
  sites = problem_sites;
  vector<Node*> nodes = sites.getNodes();
  if (alg_number == 1){
    array<Edge*, 2> edges;
    /// Sort Lexicographically
    vector<Node*> vertices = sites.sortNodes(nodes);
    edges = verticalCuts(vertices);
    computeTriangles(edges[0]);
  }
  else if (alg_number == 2){
    array<Edge*, 4> edges;
    edges = alternatingCuts(nodes, false);
    computeTriangles(edges[0]);
  }
};

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


array<Edge*, 2> Triangulator::verticalCuts(vector<Node*> vertices){
  array<Edge*, 2> edges;

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
    double ccw = orient2d(vertices[0]->getPosition(),
                          vertices[1]->getPosition(),
                          vertices[2]->getPosition());

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
  else{
    /// Split in half with vertical cut
    vector<Node*> left_half(vertices.begin(),
                            vertices.begin() + vertices.size() / 2);
    vector<Node*> right_half(vertices.begin() + vertices.size() / 2,
                             vertices.end());

    cout << "left half" << vertices[0]->getID() << " " << vertices[1]->getID() << endl;
    cout << "right half" << vertices[2]->getID() << " " << vertices[3]->getID() << endl;
    array<Edge*, 2> leftD = verticalCuts(left_half);
    array<Edge*, 2> rightD = verticalCuts(right_half);

    Edge* ldo = leftD[0];
    Edge* ldi = leftD[1];
    Edge* rdi = rightD[0];
    Edge* rdo = rightD[1];

    cout << ldo->org()->getID() << " " << ldo->dest()->getID() << endl;
    cout << ldi->org()->getID() << " " << ldi->dest()->getID() << endl;
    cout << rdo->org()->getID() << " " << rdo->dest()->getID() << endl;
    cout << rdi->org()->getID() << " " << rdi->dest()->getID() << endl;

    /// Compute the lower common tangent of leftD and rightD
    while (true){
      if (orient2d(rdi->org()->getPosition(),
                   ldi->org()->getPosition(),
                   ldi->dest()->getPosition()) > 0){ldi = ldi->Lnext();}
      else if (orient2d(ldi->org()->getPosition(),
                        rdi->dest()->getPosition(),
                        rdi->org()->getPosition()) > 0){rdi = rdi->Rprev();}
      else{break;}

    }
    /// Create a first cross edge basel from rdi.org to ldi.org
    Edge* basel = rdi->sym()->connect(ldi);
    if (ldi->org() == ldo->org()){
      ldo = basel->sym();
    }
    if (rdi->org() == rdo->org()){
      rdo = basel;
    }
    basel = mergeLoop(basel);
    edges[0] = ldo;
    edges[1] = rdo;
  }
  return edges;
};

array<Edge*, 4> Triangulator::alternatingCuts(vector<Node*> vertices, bool vertical_flag){
  /// Output four edges: left ccw, right cw, bottom ccw, top cw
  array<Edge*, 4> edges;
  if (vertices.size() == 2){
    /// Sort Lexicographically
    vertices = sites.sortNodes(vertices);
    //cout << "sorted vertices " << vertices[0]->getID() << vertices[1]->getID() << endl;
    Edge* a = Edge::makeEdge();
    a->setOrg(vertices[0]);
    a->setDest(vertices[1]);

    edges[0] = a;
    edges[1] = a->sym();

    // cout << "in function" << endl;
    // cout << edges[0]->org()->getID() << " " << edges[0]->dest()->getID() << endl;
    // cout << edges[1]->org()->getID() << " " << edges[1]->dest()->getID() << endl;

    if (vertices[0]->getPosition()[1] <= vertices[1]->getPosition()[1]){
      edges[2] = a->sym();
      edges[3] = a;
    }
    else {
      edges[2] = a;
      edges[3] = a->sym();
    }
  }
  else if (vertices.size() == 3){
    /// Sort Lexicographically
    vertices = sites.sortNodes(vertices);
    Edge* a = Edge::makeEdge();
    Edge* b = Edge::makeEdge();
    a->sym()->splice(b);

    a->setOrg(vertices[0]);
    a->setDest(vertices[1]);

    b->setOrg(vertices[1]);
    b->setDest(vertices[2]);


    /// Sort flipped Lexicographically
    int order[3];
    if (less_than_flipped(vertices[0], vertices[1])){
      if (less_than_flipped(vertices[2], vertices[0])){
        order[0] = 2; order[1] = 0; order[2] = 1;
      }
      else if (less_than_flipped(vertices[2], vertices[1])){
        order[0] = 0; order[1] = 2; order[2] = 1;
      }
      else {
        order[0] = 0; order[1] = 1; order[2] = 2;
      }
    }
    else{
      if (less_than_flipped(vertices[2], vertices[1])){
        order[0] = 2; order[1] = 1; order[2] = 0;
      }
      else if (less_than_flipped(vertices[2], vertices[0])){
        order[0] = 1; order[1] = 2; order[2] = 0;
      }
      else{
        order[0] = 1; order[1] = 0; order[2] = 2;
      }
    }

    /// CCW Test
    double ccw = orient2d(vertices[0]->getPosition(),
                          vertices[1]->getPosition(),
                          vertices[2]->getPosition());


    if (ccw > 0){
      Edge* c = b->connect(a);
      edges[0] = a;
      edges[1] = b->sym();
      Edge* cw_edges[3] = {c->sym(), a->sym(), b->sym()};
      Edge* ccw_edges[3] = {a, b, c};
      edges[2] = ccw_edges[order[0]];
      edges[3] = cw_edges[order[2]];

    }
    else if (ccw < 0){
      Edge* c = b->connect(a);
      edges[0] = c->sym();
      edges[1] = c;
      Edge* ccw_edges[3] = {c->sym(), a->sym(), b->sym()};
      Edge* cw_edges[3] = {a, b, c};
      edges[2] = ccw_edges[order[0]];
      edges[3] = cw_edges[order[2]];

    }
    else{
      edges[0] = a;
      edges[1] = b->sym();
      Edge* colinear_edges[3] = {a, b, b->sym()};
      edges[2] = colinear_edges[order[0]];
      edges[3] = colinear_edges[order[2]];
    }
  }
  else {
    vertical_flag = !vertical_flag;

    /// Sort into halves
    if (vertical_flag){
      kthSmallest(vertices, 0, vertices.size() - 1, vertices.size()/2, &less_than_xy);
    }
    else{
      kthSmallest(vertices, 0, vertices.size() - 1, vertices.size()/2, &less_than_yx);
    }

    vector<Node*> first_half(vertices.begin(),
                            vertices.begin() + vertices.size() / 2);
    vector<Node*> second_half(vertices.begin() + vertices.size() / 2,
                             vertices.end());

    // cout << "first half" << endl;
    // for (const auto& i : first_half){
    //   cout << i->getID() << endl;
    // }
    // cout << "second half" << endl;
    // for (const auto& i : second_half){
    //   cout << i->getID() << endl;
    // }


    array<Edge*, 4> firstD = alternatingCuts(first_half, vertical_flag);
    array<Edge*, 4> secondD = alternatingCuts(second_half, vertical_flag);
    //// Set ldo, ldi, rdo, rdi based on whether it was a vertical or horizontal cut
    Edge* ldo;
    Edge* ldi;
    Edge* rdi;
    Edge* rdo;
    Edge* bdo;
    Edge* bdi;
    Edge* tdi;
    Edge* tdo;
    if (vertical_flag){
      ldo = firstD[0];
      ldi = firstD[1];
      rdi = secondD[1];
      rdo = secondD[0];
      bdo = firstD[2];
      bdi = firstD[3];
      tdi = secondD[2];
      tdo = secondD[3];
      cout << "==== second level ====" << endl;
      cout << "ldo " << ldo->org()->getID() << " " << ldo->dest()->getID() << endl;
      cout << "ldi " << ldi->org()->getID() << " " << ldi->dest()->getID() << endl;
      cout << "rdo " << rdo->org()->getID() << " " << rdo->dest()->getID() << endl;
      cout << "rdi " << rdi->org()->getID() << " " << rdi->dest()->getID() << endl;
      cout << "bdo " << bdo->org()->getID() << " " << bdo->dest()->getID() << endl;
      cout << "bdi " << bdi->org()->getID() << " " << bdi->dest()->getID() << endl;
      cout << "tdo " << tdo->org()->getID() << " " << tdo->dest()->getID() << endl;
      cout << "tdi " << tdi->org()->getID() << " " << tdi->dest()->getID() << endl;
    }
    else{
      bdo = firstD[0];
      bdi = firstD[1];
      tdi = secondD[1];
      tdo = secondD[0];
      ldo = firstD[2];
      ldi = firstD[3];
      rdi = secondD[2];
      rdo = secondD[3];
      cout << "==== first level ====" << endl;
      cout << "ldo " << ldo->org()->getID() << " " << ldo->dest()->getID() << endl;
      cout << "ldi " << ldi->org()->getID() << " " << ldi->dest()->getID() << endl;
      cout << "rdo " << rdo->org()->getID() << " " << rdo->dest()->getID() << endl;
      cout << "rdi " << rdi->org()->getID() << " " << rdi->dest()->getID() << endl;
      cout << "bdo " << bdo->org()->getID() << " " << bdo->dest()->getID() << endl;
      cout << "bdi " << bdi->org()->getID() << " " << bdi->dest()->getID() << endl;
      cout << "tdo " << tdo->org()->getID() << " " << tdo->dest()->getID() << endl;
      cout << "tdi " << tdi->org()->getID() << " " << tdi->dest()->getID() << endl;
    }

    /// Update ldo & rdo
    /// Compute the lower common tangent of leftD and rightD
    while (true){
      if (orient2d(rdi->org()->getPosition(),
                   ldi->org()->getPosition(),
                   ldi->dest()->getPosition()) > 0){ldi = ldi->Lnext();}
      else if (orient2d(ldi->org()->getPosition(),
                        rdi->dest()->getPosition(),
                        rdi->org()->getPosition()) > 0){rdi = rdi->Rprev();}
      else{break;}

    }
    /// Create a first cross edge basel from rdi.org to ldi.org
    Edge* basel = rdi->sym()->connect(ldi);
    if (ldi->org() == ldo->org()){
      ldo = basel->sym();
    }
    if (rdi->org() == rdo->org()){
      rdo = basel;
    }

    /// update bdo & tdo
    while (true){
      if (orient2d(tdi->org()->getPosition(),
                   bdi->org()->getPosition(),
                   bdi->dest()->getPosition()) > 0){bdi = bdi->Lnext();}
      else if (orient2d(bdi->org()->getPosition(),
                        tdi->dest()->getPosition(),
                        tdi->org()->getPosition()) > 0){tdi = tdi->Rprev();}
      else{break;}

    }
    /// Create a first cross edge basel from rdi.org to ldi.org
    Edge* sidel = tdi->sym()->connect(bdi);
    if (bdi->org() == bdo->org()){
      bdo = sidel->sym();
    }
    if (tdi->org() == tdo->org()){
      tdo = sidel;
    }

    basel = mergeLoop(basel);

    cout << "==== update ====" << endl;
    cout << "ldo " << ldo->org()->getID() << " " << ldo->dest()->getID() << endl;
    cout << "ldi " << ldi->org()->getID() << " " << ldi->dest()->getID() << endl;
    cout << "rdo " << rdo->org()->getID() << " " << rdo->dest()->getID() << endl;
    cout << "rdi " << rdi->org()->getID() << " " << rdi->dest()->getID() << endl;
    cout << "bdo " << bdo->org()->getID() << " " << bdo->dest()->getID() << endl;
    cout << "bdi " << bdi->org()->getID() << " " << bdi->dest()->getID() << endl;
    cout << "tdo " << tdo->org()->getID() << " " << tdo->dest()->getID() << endl;
    cout << "tdi " << tdi->org()->getID() << " " << tdi->dest()->getID() << endl;
    edges[0] = bdo;
    edges[1] = tdo;
    edges[2] = rdo;
    edges[3] = ldo;
  }
  return edges;
};


bool Triangulator::valid(Edge* edge, Edge* basel){
  return (orient2d(edge->dest()->getPosition(),
                  basel->dest()->getPosition(),
                  basel->org()->getPosition()) > 0);
};

Edge* Triangulator::mergeLoop(Edge* basel){
  /// Merge loop
  while (true){
    /// Locate the first L point to be encountered by the rising bubble
    /// and delete L edges out of basel.dest() that fail the circle test
    Edge* lcand = basel->sym()->Onext();
    if (valid(lcand, basel)){
      while(incircle(basel->dest()->getPosition(),
                     basel->org()->getPosition(),
                     lcand->dest()->getPosition(),
                     lcand->Onext()->dest()->getPosition()) > 0){
                       Edge* tmp = lcand->Onext();
                       lcand->deleteEdge();
                       lcand = tmp;
                      }
                   }
    /// Symmetrically locate the first R point to be hit, and delete R edges
    Edge* rcand = basel->Oprev();
    if (valid(rcand, basel)){
      while(incircle(basel->dest()->getPosition(),
                     basel->org()->getPosition(),
                     rcand->dest()->getPosition(),
                     rcand->Oprev()->dest()->getPosition()) > 0){
                       Edge* tmp = rcand->Oprev();
                       rcand->deleteEdge();
                       rcand = tmp;
                      }
                  }
    /// If both lcand and r cand are invalid then basel is upper common tangent
    if ((!valid(lcand, basel)) && (!valid(rcand, basel))){
      break;
    }
    /// The next cross edge is to be connected to either lcand.dest or rcand.dest
    /// if both are valid, then choose appropriate one via incircle test.
    if (!valid(lcand, basel) ||
        (valid(rcand, basel) && (incircle(lcand->dest()->getPosition(),
                                         lcand->org()->getPosition(),
                                         rcand->org()->getPosition(),
                                         rcand->dest()->getPosition()) > 0))){
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
