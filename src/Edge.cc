#include "Edge.h"
#include "QuadEdge.h"

using namespace std;

/// Empty edge starting and ending at infinity
Edge::Edge(){
  m_origin = new Node();
  m_destination = new Node();
};


Edge::Edge(Node *org, Node *dest){
  m_origin = org;
  m_destination = dest;
};


Edge* Edge::makeEdge(){
  return (new QuadEdge())->edges;
};


void Edge::setNext(Edge *next){
  m_next = next;
};


void Edge::setIndex(unsigned int index){
  m_index = index;
};

unsigned int Edge::getIndex(){
  return m_index;
};


Edge* Edge::rot(){
  if(m_index < 3){
    return this+1;
  }
  else{
    return this-3;
  }
};


Edge* Edge::rotInv(){
  if(m_index < 3){
    return this+3;
  }
  else{
    return this-1;
  }
};



Edge* Edge::sym(){
  if (m_index < 2){
    return this+2;
  }
  else{
    return this-2;
  }
};


Node* Edge::org(){
  return m_origin;
};


void Edge::setOrg(Node *org){
  org->addEdge(this);
  m_origin = org;
};



Node* Edge::dest(){
  return m_destination;
};


void Edge::setDest(Node *dest){
  dest->addEdge(this->sym());
  m_destination = dest;
};


Edge* Edge::Onext(){
  return m_next;
};


Edge* Edge::Lnext(){
  return this->rotInv()->Onext()->rot();
}

Edge* Edge::connect(Edge *b){
  Edge* e;
  e = e->makeEdge();
  e->setOrg(m_destination);
  e->setDest(b->org());
  e->splice(this->Lnext());
  e->sym()->splice(b);
  return e;
};


void Edge::splice(Edge *b){
  Edge* alpha = this->Onext()->rot();
  Edge* beta = b->Onext()->rot();

  this->setNext(b->Onext());
  b->setNext(this->Onext());
  alpha->setNext(beta->Onext());
  beta->setNext(alpha->Onext());
};
