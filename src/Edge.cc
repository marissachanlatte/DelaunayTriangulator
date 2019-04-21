#include "Edge.h"
#include "QuadEdge.h"
#include <iostream>
#include <assert.h>

using namespace std;

Edge* Edge::makeEdge(){
  return (new QuadEdge())->edges;
};


void Edge::deleteEdge(){
  this->splice(this->Oprev());
  this->sym()->splice(this->sym()->Oprev());
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
  assert (m_index >= 0);
  assert (m_index <= 3);
  if(m_index > 0){
    return this-1;
  }
  else{
    return this+3;
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
  m_origin = org;
  org->addEdge(this);
};



Node* Edge::dest(){
  return this->sym()->org();
};


void Edge::setDest(Node *dest){
  this->sym()->setOrg(dest);
  dest->addEdge(this->sym());
};


Edge* Edge::Onext(){
  return m_next;
};


Edge* Edge::Lnext(){
  return this->rotInv()->Onext()->rot();
}


Edge* Edge::Rprev(){
  return this->sym()->Onext();
};


Edge* Edge::Oprev(){
  return this->rot()->Onext()->rot();
};


Edge* Edge::connect(Edge *b){
  Edge* e = Edge::makeEdge();
  e->setOrg(this->dest());
  e->setDest(b->org());
  e->splice(this->Lnext());
  e->sym()->splice(b);
  return e;
};


void Edge::splice(Edge *b){
  Edge* alpha = this->Onext()->rot();
  Edge* beta = b->Onext()->rot();

  /// Interchange a & b and alpha & beta
  Edge* tmp1 = b->Onext();
  Edge* tmp2 = this->Onext();
  Edge* tmp3 = beta->Onext();
  Edge* tmp4 = alpha->Onext();

  this->setNext(tmp1);
  b->setNext(tmp2);
  alpha->setNext(tmp3);
  beta->setNext(tmp4);
};


unsigned int Edge::getID(){
  return m_id;
};

void Edge::setID(unsigned int id){
  m_id = id;
};

unsigned int Edge::lastID = -1;
