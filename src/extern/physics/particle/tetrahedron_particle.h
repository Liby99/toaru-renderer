#ifndef TETRAHEDRON_PARTICLE_H
#define TETRAHEDRON_PARTICLE_H

#include "physics/core/particle.h"

namespace nash {
  class TetrahedronParticle : public Particle {
  public:
    TetrahedronParticle(Point &p1, Point &p2, Point &p3, Point &p4);
    TetrahedronParticle(Point &p1, Point &p2, Point &p3, Point &p4, float mass);
  };
}

#endif