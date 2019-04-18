#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "point.h"
#include "field.h"

namespace nash {
  class PhysicsSystem : public Object {
  public:
    PhysicsSystem();

    void addField(const Field &field);
    void removeField(unsigned int i);

    int getNumDOFs();
    void fetchPositions(std::vector<Vector3f> &positions);
    void fetchVelocities(std::vector<Vector3f> &velocities);

  protected:
    std::vector<Point *> points;
    std::vector<Field *> fields;
  };
}

#endif