#ifndef EMITTER_H
#define EMITTER_H

#include "object/adhoc/point_cloud.h"
#include "physics/core/physics_system.h"
#include "physics/particle/point_particle.h"

namespace nash {
  class Emitter : public PointCloud, PhysicsSystem {
  public:
    float frequency;

    Emitter();
    Emitter(float frequency);

    virtual void update();

  private:
    std::vector<PointParticle *> particles;
  };
}

#endif