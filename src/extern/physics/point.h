#ifndef TOARU_PHYSICS_POINT_H
#define TOARU_PHYSICS_POINT_H

#include "utility/math.h"

#include <vector>

namespace toaru {
  class Point : public std::enable_shared_from_this<Point> {
  public:
    Vector3f position, velocity, force;
    float mass, invMass;
    bool isFixed;
    int index;

    Point(Vector3f position, int index, bool isFixed = false);

    void addForce(const Vector3f &force);
    void addMass(float mass);
    bool updateInvMass();
    void update(float deltaTime);
    void integrate(float deltaTime);
    bool operator==(const Point &other) const;
  };
}

#endif
