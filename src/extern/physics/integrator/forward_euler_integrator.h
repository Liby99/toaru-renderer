#ifndef FORWARD_EULER_INTEGRATER_H
#define FORWARD_EULER_INTEGRATER_H

#include "physics/core/integrator.h"

namespace nash {
  class ForwardEulerIntegrator : public Integrator {
  public:
    virtual void integrate(PhysicsSystem &system, float timestep);
  };
}

#endif