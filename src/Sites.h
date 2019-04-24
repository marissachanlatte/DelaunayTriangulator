#ifndef _SITES_H_
#define _SITES_H_
#include <vector>
#include "Node.h"

class Sites{
  public:
    Sites();
    /// Constructor by reading from file
    Sites(const std::string& nodeFile);
    int numNodes();
    std::vector<double*> getPositions(bool vertical);
    std::vector<Node*> sortNodes(std::vector<Node*> nodes, bool vertical);
    std::vector<Node*> getNodes();
    Node* findNode(const int id);

  private:
    unsigned int m_numNodes;
    std::vector<Node*> m_nodes;

};
#endif
