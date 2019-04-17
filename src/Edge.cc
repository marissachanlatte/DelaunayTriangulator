#include "Edge.h"

using namespace std;

/// Empty edge starting and ending at infinity
Edge::Edge(){
  m_origin = Node();
  m_destination = Node();
};


Edge::Edge(Node org, Node dest){
  m_origin = org;
  m_destination = dest;
};
