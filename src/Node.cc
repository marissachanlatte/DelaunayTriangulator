#include <utility>
#include <math.h>
#include <float.h>
#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(double* position, const int& nodeID){
  m_position[0] = position[0];
  m_position[1] = position[1];
  m_id = nodeID;
};


const int& Node::getID() const{
  return m_id;
};


double* Node::getPosition(){
  return m_position;
};

void Node::addEdge(Edge *edge){
  m_edge = edge;
};
