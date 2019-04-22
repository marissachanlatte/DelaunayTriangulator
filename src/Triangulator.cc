#include "Triangulator.h"
#include "Edge.h"
#include "QuadEdge.h"
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stdlib.h>

using namespace std;

Triangulator::Triangulator(Sites sites, unsigned int alg_number){

  vector<Node*> nodes = sites.getNodes();
  /// Sort Lexicographically
  vector<Node*> vertices = sites.sortNodes(nodes);
  array<Edge*, 2> edges;
  if (alg_number == 1){
    edges = verticalCuts(vertices);
  }
  else if (alg_number == 2){
    cout << "not implemented" << endl;
  }
  computeTriangles(edges[0], edges[1]);
};

void Triangulator::computeTriangles(Edge* le, Edge* re){
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

    array<Edge*, 2> leftD = verticalCuts(left_half);
    array<Edge*, 2> rightD = verticalCuts(right_half);

    Edge* ldo = leftD[0];
    Edge* ldi = leftD[1];
    Edge* rdi = rightD[0];
    Edge* rdo = rightD[1];

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
