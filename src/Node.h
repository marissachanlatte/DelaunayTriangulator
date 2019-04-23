#ifndef _NODE_H_
#define _NODE_H_
#include <utility>
#include "Edge.h"
using namespace std;

class Edge;

class Node{
  public:
    Node(double* position, const int& nodeID);
    const int& getID() const;
    double* getPosition(bool vertical);
    void addEdge(Edge *edge);
  private:
    int m_id;
    double m_position[2];
    double m_position_rot[2];
    Edge *m_edge;
};
#endif
