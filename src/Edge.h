#ifndef _EDGE_H_
#define _EDGE_H_

#include "Node.h"
using namespace std;

class Node;

class Edge{
  public:
    Edge();
    Edge(Node *org, Node *dest);
    Edge* makeEdge();
    void setNext(Edge *next);
    void setIndex(unsigned int index);
    unsigned int getIndex();
    Node* org();
    Node* dest();
    void setOrg(Node *org);
    void setDest(Node *dest);
    Edge* Onext();
    Edge* Lnext();
    Edge* rot();
    Edge* rotInv();
    Edge* sym();
    void splice(Edge *b);
    Edge* connect(Edge *b);
  private:
    Node *m_origin;
    Node *m_destination;
    Edge *m_next;
    unsigned int m_index;

};
#endif
