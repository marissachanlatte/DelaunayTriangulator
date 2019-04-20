#ifndef _NODE_H_
#define _NODE_H_
#include <utility>
#include "Edge.h"
using namespace std;

class Edge;

class Node{
  public:
    Node();
    Node(pair<double, double> position, const int& nodeID);
    const int& getID() const;
    const pair<double, double> getPosition();
    void addEdge(Edge *edge);
  private:
    int m_id;
    pair<double, double> m_position;
    Edge *m_edge;
};
#endif
