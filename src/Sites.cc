#include "Sites.h"
#include "Node.h"
#include <fstream>
#include <iostream>
#include <vector>
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
    pair<double, double> x;
    nodes>>id>>x.first>>x.second;
    m_nodes[id - 1] = Node(x, id - 1);
  }
};

int Sites::numNodes(){
  return m_numNodes;
};

vector<pair<double, double>> Sites::getPositions(){
  vector<pair<double, double>>  positions;
  for(auto &node : m_nodes){
    positions.push_back(node.getPosition());
  }
  return positions;
};
