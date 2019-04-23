#include "Sites.h"
#include "Node.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <utility>

using namespace std;

Sites::Sites(){
  m_numNodes = 0;
};

Sites::Sites(const string& nodeFile){
  /// Read in node file
  ifstream nodes(nodeFile.c_str());
  int ncount, dim, attributes, boundaryMarkers;
  nodes>>ncount>>dim>>attributes>>boundaryMarkers;

  m_numNodes = ncount;
  m_nodes.resize(m_numNodes);
  for(int i=0; i<ncount; i++){
    int id;
    double x[2];
    nodes>>id>>x[0]>>x[1];
    m_nodes[id - 1] = new Node(x, id - 1);
  }
};

int Sites::numNodes(){
  return m_numNodes;
};

vector<double*> Sites::getPositions(){
  vector<double*>  positions;
  for(auto &node : m_nodes){
    positions.push_back(node->getPosition());
  }
  return positions;
};

vector<Node*> Sites::sortNodes(vector<Node*> nodes){
  vector<array<double, 3>>  positions_and_ids;
  for(auto &node : nodes){
    array<double, 3> pid  = {node->getPosition()[0],
                             node->getPosition()[1],
                             (double) node->getID()};
    positions_and_ids.push_back(pid);
  }
  sort(positions_and_ids.begin(), positions_and_ids.end());
  vector<Node*> sorted_ids;
  for(auto &ids : positions_and_ids){
    sorted_ids.push_back(this->findNode(int(ids[2])));
  }
  return sorted_ids;
};

vector<Node*> Sites::getNodes(){
  return m_nodes;
};

Node* Sites::findNode(const int id){
  return m_nodes[id];
};
