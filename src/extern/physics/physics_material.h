#ifndef TOARU_PHYSICS_MATERIAL_H
#define TOARU_PHYSICS_MATERIAL_H

#include "material_tensor.h"

namespace toaru {
  class PhysicsMaterial {
  public:
    const float density;
    const MaterialTensor &k, d;
    PhysicsMaterial(float density, const MaterialTensor &k, const MaterialTensor &d);
  };
}

#endif