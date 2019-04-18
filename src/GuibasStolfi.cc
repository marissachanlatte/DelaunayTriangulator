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
  else if (vertices.size() == 3){
    Edge a = Edge(vertices[0], vertices[1]);
    Edge b = Edge(vertices[1], vertices[2]);
    double s1[2] = {vertices[0].getPosition().first,
                 vertices[0].getPosition().second};
    double s2[2] = {vertices[1].getPosition().first,
                 vertices[1].getPosition().second};
    double s3[2] = {vertices[2].getPosition().first,
                 vertices[2].getPosition().second};

    double ccw = orient2d(s1, s2, s3);
    cout << ccw << endl;
  }
};
