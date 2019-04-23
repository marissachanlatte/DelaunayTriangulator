#include <utility>
#include <math.h>
#include <float.h>
#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(double* position, const int& nodeID){
  m_position[0] = position[0];
  m_position[1] = position[1];
  m_position_rot[0] = position[1];
  m_position_rot[1] = -position[0];
  m_id = nodeID;
};


const int& Node::getID() const{
  return m_id;
};


double* Node::getPosition(bool vertical){
  if (vertical){
  return m_position;
  }
  else{
    return m_position_rot;
  }
};

void Node::addEdge(Edge *edge){
  m_edge = edge;
};
