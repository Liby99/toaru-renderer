#ifndef POINT_PARTICLE_H
#define POINT_PARTICLE_H

#include "physics/core/particle.h"

namespace nash {
  class PointParticle : public Particle {
  public:
    PointParticle(const Point &p);
    PointParticle(const Point &p, float m);
  };
}

#endif