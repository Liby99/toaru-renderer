#ifndef TOARU_PHYSICS_OBJECT_H
#define TOARU_PHYSICS_OBJECT_H

#include "tetrahedron.h"

namespace toaru {
  class PhysicsObject {
  public:
    const PhysicsMaterial &mat;
    std::vector<const Tetrahedron *> tetrahedrons;
    PhysicsObject(const PhysicsMaterial &mat);
    void addTetrahedron(const Tetrahedron &tetra);
  };
}

#endif