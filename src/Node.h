#ifndef _NODE_H_
#define _NODE_H_
#include <utility>
using namespace std;

class Node{
  public:
    Node();
    Node(pair<double, double> position, const int& nodeID);
    const int& getID() const;
    const pair<double, double> getPosition();
  private:
    int m_id;
    pair<double, double> m_position;
};
#endif
