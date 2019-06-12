#ifndef MPM_CELL_H
#define MPM_CELL_H

#include "utility/math.h"

namespace toaru::mpm {
  class Cell {
  public:
    Vector3f momentum;
    Vector3f velocity;
    float mass;

    void reset();
  };
}

#endif