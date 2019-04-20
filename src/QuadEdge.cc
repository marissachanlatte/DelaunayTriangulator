#include "QuadEdge.h"

using namespace std;

/// Initialize Empty Quad Edge
QuadEdge::QuadEdge(){
  edges[0].setID(Edge::lastID + 1);
  edges[1].setID(Edge::lastID + 2);
  edges[2].setID(Edge::lastID + 3);
  edges[3].setID(Edge::lastID + 4);
  /// Update last used id to keep track of global IDs
  Edge::lastID = Edge::lastID + 4;

  /// Set local indices
  edges[0].setIndex(0);
  edges[1].setIndex(1);
  edges[2].setIndex(2);
  edges[3].setIndex(3);

  /// Set next
  edges[0].setNext(edges+0);
  edges[1].setNext(edges+3);
  edges[2].setNext(edges+2);
  edges[3].setNext(edges+1);
};
