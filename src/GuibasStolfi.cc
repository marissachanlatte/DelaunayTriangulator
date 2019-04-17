#include "GuibasStolfi.h"
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

GuibasStolfi::GuibasStolfi(){

};

GuibasStolfi::GuibasStolfi(Sites sites){
  /// Sort sites lexicographically
  vector<pair<double, double>> vertices = sites.getPositions();
  sort(vertices.begin(), vertices.end());
};
