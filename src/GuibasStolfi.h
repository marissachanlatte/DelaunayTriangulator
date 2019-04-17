#ifndef _GUIBASSTOLFI_H_
#define _GUIBASSTOLFI_H_

#include "Sites.h"
#include <vector>
#include <array>

using namespace std;

class GuibasStolfi{
  public:
    GuibasStolfi();
    GuibasStolfi(Sites sites);
    /// The triangles produced by the GS algorithm
    vector<array<int, 3>> triangles;
};
#endif
