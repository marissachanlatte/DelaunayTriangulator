#ifndef _EDGE_H_
#define _EDGE_H_

#include "Node.h"
using namespace std;

class Node;

class Edge{
  public:
    static Edge* makeEdge();
    void setNext(Edge *next);
    void setIndex(unsigned int index);
    void setID(unsigned int id);
    unsigned int getIndex();
    unsigned int getID();
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
    /// last global edge id used
    static unsigned int lastID;
  private:
    Node *m_origin;
    Edge *m_next;
    unsigned int m_index;
    unsigned int m_id;
};
#endif
