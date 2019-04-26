#ifndef TOARU_MATERIAL_TENSOR_H
#define TOARU_MATERIAL_TENSOR_H

#include "utility/math.h"

namespace toaru {
  class MaterialTensor {
  public:
    // Constant
    float e, v;

    // Lame constants
    float lambda, mu, c;

    MaterialTensor(float e, float v, bool calculateLame = true);
  };
}

#endif
