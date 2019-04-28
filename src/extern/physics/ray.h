#ifndef TOARU_PHYSICS_RAY_H
#define TOARU_PHYSICS_RAY_H

#include "utility/math.h"

namespace toaru {
  class Ray {
  public:
    Vector3f origin, direction;
    Ray(Vector3f origin, Vector3f direction);

    // return: origin + t * direction
    Vector3f getPoint(float t) const;
  };
}

#endif