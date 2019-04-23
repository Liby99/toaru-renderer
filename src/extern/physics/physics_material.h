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
    Matrix<float, 6, 6> mat;

    // Upper K
    Matrix<float, 3, 3> upper;

    // Lower K
    Matrix<float, 3, 3> lower;

    PhysicsMaterial(float e, float v, bool calculateLame = true);
  };
}

#endif
