#include "QuadEdge.h"

using namespace std;

/// Initialize Empty Quad Edge
QuadEdge::QuadEdge(){
  edges[0].setNext(edges+0);
  edges[1].setNext(edges+3);
  edges[2].setNext(edges+2);
  edges[3].setNext(edges+1);

  edges[0].setIndex(0);
  edges[1].setIndex(1);
  edges[2].setIndex(2);
  edges[3].setIndex(3);
};
