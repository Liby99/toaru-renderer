#ifndef TOARU_POINT_H
#define TOARU_POINT_H

#include "utility/math.h"

#include <vector>

namespace toaru {
  class Point : public std::enable_shared_from_this<Point> {
  public:
    Vector3f position;
    Vector3f velocity;
    Vector3f force;
    float mass;
    float invMass;
    int index;

    Point(Vector3f position, int index);

    virtual void addForce(const Vector3f &force);
    virtual void addMass(float mass);
    virtual bool updateInvMass();
    virtual void update(float deltaTime);
    virtual bool operator==(const Point &other) const;
  };
}

#endif
