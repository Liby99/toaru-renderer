#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "object/object.h"
#include "physics/core/field.h"

namespace nash {
  class PhysicsSystem : Object {
    std::vector<Field *> fields;
  };
}

#endif