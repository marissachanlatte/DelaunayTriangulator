#include "GuibasStolfi.h"
#include "Edge.h"
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

GuibasStolfi::GuibasStolfi(){

};

GuibasStolfi::GuibasStolfi(Sites sites){
  /// Sort sites lexicographically
  vector<Node*> nodes = sites.getNodes();
  vector<Node*> vertices = sites.sortNodes(nodes);

  if (vertices.size() == 2){
    this->triangles.push_back({vertices[0]->getID() + 1,
                        vertices[1]->getID() + 1,
                        vertices[0]->getID() + 1});
  }
  else if (vertices.size() == 3){
    Edge* a;
    a = a->makeEdge();
    Edge* b;
    b = b->makeEdge();
    a->sym()->splice(b);

    a->setOrg(vertices[0]);
    a->setDest(vertices[1]);

    b->setOrg(vertices[1]);
    b->setDest(vertices[2]);

    Edge* c = b->connect(a);
    this->triangles.push_back({a->org()->getID() + 1,
                               b->org()->getID() + 1,
                               c->org()->getID() + 1});
  }
};
