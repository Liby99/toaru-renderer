#ifndef FIELD_H
#define FIELD_H

#include "particle.h"

namespace nash {
  class Field {
  public:
    virtual void applyForceTo(Particle &particle);
  };
}

#endif