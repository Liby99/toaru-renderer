#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "object/object.h"
#include "physics/core/field.h"

namespace nash {
  class PhysicsSystem : public Object {
  public:
    PhysicsSystem();

    void addField(const Field &field);
    void removeField(unsigned int i);

    virtual void start();
    virtual void update();
    virtual void render();

  protected:
    std::vector<Field *> fields;
  };
}

#endif