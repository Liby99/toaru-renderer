#ifndef SEGMENT_PARTICLE_H
#define SEGMENT_PARTICLE_H

#include "physics/core/particle.h"

namespace nash {
  class SegmentParticle : public Particle {
  public:
    SegmentParticle(Point &p1, Point &p2);
    SegmentParticle(Point &p1, Point &p2, float mass);
  };
}

#endif