#ifndef TOARU_PHYSICS_COLLISION_H
#define TOARU_PHYSICS_COLLISION_H

#include "tetrahedron.h"

namespace toaru {
  class Collision {
  public:
    Vector3f center;
    float volume;
    Tetrahedron *t1, *t2;

    Collision(Tetrahedron &t1, Tetrahedron &t2);
  };
}

#endif