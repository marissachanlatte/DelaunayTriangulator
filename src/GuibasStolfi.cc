#include "GuibasStolfi.h"
#include "Edge.h"
#include "QuadEdge.h"
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

GuibasStolfi::GuibasStolfi(Sites sites){
  array<Edge*, 2> edges = Delaunay(sites);
  computeTriangles(edges[0], edges[1]);
};

void GuibasStolfi::computeTriangles(Edge* le, Edge* re){
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

array<Edge*, 2> GuibasStolfi::Delaunay(Sites sites){
  /// Sort sites lexicographically
  vector<Node*> nodes = sites.getNodes();
  vector<Node*> vertices = sites.sortNodes(nodes);
  array<Edge*, 2> edges;

  if (vertices.size() == 2){
    QuadEdge qa;
    Edge* a = qa.edges;
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

    Edge* c = b->connect(a);
    /// CCW Test
    double s1[2] = {vertices[0]->getPosition().first,
                 vertices[0]->getPosition().second};
    double s2[2] = {vertices[1]->getPosition().first,
                 vertices[1]->getPosition().second};
    double s3[2] = {vertices[2]->getPosition().first,
                 vertices[2]->getPosition().second};
    double ccw = orient2d(s1, s2, s3);

    if (ccw >= 0){
      edges[0] = a;
      edges[1] = b->sym();
    }
    else{
      edges[0] = c->sym();
      edges[1] = c;
    }
  }
  return edges;
};
