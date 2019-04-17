#ifndef _EDGE_H_
#define _EDGE_H_

#include "Node.h"
using namespace std;

class Edge{
  public:
    Edge();
    Edge(Node org, Node dest);
  private:
    Node m_origin;
    Node m_destination;
};
#endif
