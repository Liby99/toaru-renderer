#ifndef INTEGRATER_H
#define INTEGRATER_H

#include "physics_system.h"

namespace nash {
  class Integrator {
  public:
    virtual void integrate(PhysicsSystem &system, float timestep);
  };
}

#endif