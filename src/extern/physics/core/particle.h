#ifndef PARTICLE_H
#define PARTICLE_H

#include "point.h"

namespace nash {
  class Particle {
  public:
    const static DEFAULT_MASS;

    Particle();
    Particle(float mass);

    Point &getPoint(int i);
    int getNumPoints();
    void setPosition(int i, Vector3f pos);
    void setVelocity(int i, Vector3f vel);

    float getMass();
    void setMass(float m);

    Vector3f getCenterOfMass();
  protected:

    std::vector<Point *> points;
    float mass;
  };
}

#endif