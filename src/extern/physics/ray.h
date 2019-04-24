#ifndef TOARU_PHYSICS_RAY_H
#define TOARU_PHYSICS_RAY_H

#include "utility/math.h"

namespace toaru {
  class Ray {
  public:
    Vector3f start, direction;
    Ray(Vector3f start, Vector3f direction);
  };
}

#endif