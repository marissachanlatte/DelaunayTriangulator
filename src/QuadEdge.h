#include <array>
#include "Edge.h"

using namespace std;

class QuadEdge{
  public:
    QuadEdge();
    array<Edge, 4> edges;
};
