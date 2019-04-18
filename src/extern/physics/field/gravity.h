#ifndef GRAVITY_H
#define GRAVITY_H

#include "../core/field.h"

namespace nash {
  class Gravity : public Field {
  public:
    static const Vector3f DEFAULT_DIRECTION;
    static const float G;

    Vector3f direction;
    float g;

    Gravity();
    Gravity(float g);
    Gravity(Vector3f direction, float g);

    virtual void applyForceTo(Particle &particle);
  };
}

#endif