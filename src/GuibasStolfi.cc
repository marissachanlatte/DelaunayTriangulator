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
  vector<Node> nodes = sites.getNodes();
  vector<Node> vertices = sites.sortNodes(nodes);

  if (vertices.size() == 2){
    Edge a = Edge(vertices[0], vertices[1]);
    this->triangles.push_back({vertices[0].getID() + 1,
                        vertices[1].getID() + 1,
                        vertices[0].getID() + 1});
  }
};
