#ifndef TRIANGLE_PARTICLE_H
#define TRIANGLE_PARTICLE_H

#include "physics/core/particle.h"

namespace nash {
  class TriangleParticle : public Particle {
  public:
    TriangleParticle(Point &p1, Point &p2, Point &p3);
    TriangleParticle(Point &p1, Point &p2, Point &p3, float mass);
  };
}

#endif