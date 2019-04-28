#ifndef TOARU_PHYSICS_MATERIAL_H
#define TOARU_PHYSICS_MATERIAL_H

#include "material_tensor.h"

namespace toaru {
  class PhysicsMaterial {
  public:
    const float density;

    // Elastic limit
    const float gamma1;

    // Plastic limit
    const float gamma2;

    // Stiffness matrix
    const MaterialTensor &k;

    // Damping matrix
    const MaterialTensor &d;

    PhysicsMaterial(float density, float gamma1, float gamma2, const MaterialTensor &k,
                    const MaterialTensor &d);
  };
}

#endif