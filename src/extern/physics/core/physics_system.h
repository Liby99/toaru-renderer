#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "particle.h"
#include "field.h"

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
    std::vector<Particle *> particles;
    std::vector<Field *> fields;
  };
}

#endif