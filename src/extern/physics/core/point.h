#ifndef POINT_H
#define POINT_H

#include "utility/math.h"

namespace nash {
  class Point {
  public:
    Vector3f position;
    Vector3f velocity;
    Vector3f force;

    void clearForce();
    void addForce(const Vector3f &f);
  };
}

#endif