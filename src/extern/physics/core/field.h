#ifndef FIELD_H
#define FIELD_H

#include "particle.h"

namespace nash {
  class Field {
  public:
    virtual Vector3f getForceAt(Vector3f position);
  };
}

#endif