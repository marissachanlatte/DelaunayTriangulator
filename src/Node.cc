#include <utility>
#include <math.h>
#include <float.h>
#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(){
  m_position.first = DBL_MAX;
  m_position.second = DBL_MAX;
};


Node::Node(pair<double, double> position, const int& nodeID){
  m_position = position;
  m_id = nodeID;
};


const int& Node::getID() const{
  return m_id;
};


const pair<double, double> Node::getPosition(){
  return m_position;
};

void Node::addEdge(Edge *edge){
  m_edge = edge;
};
