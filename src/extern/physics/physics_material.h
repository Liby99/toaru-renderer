#ifndef TOARU_PHYSICSMATERIAL_H
#define TOARU_PHYSICSMATERIAL_H

#include "utility/math.h"

namespace toaru {
  class PhysicsMaterial {
  public:
    // Constant
    float e, v;

    // Lame constants
    float lambda, mu, c;

    // Stiffness tensor K
    Matrix<float, 6, 6> K;

    // Upper K
    Matrix<float, 3, 3> uK;

    // Lower K
    Matrix<float, 3, 3> lK;

    PhysicsMaterial(float e, float v);
  };
}

#endif
