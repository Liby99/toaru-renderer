#include "physics/point.h"

using namespace toaru;

Point::Point(Vector3f position, int index) {
  this->position = position;
  this->mass = 0;
  this->index = index;
}

void Point::addForce(const Vector3f &force) {
  this->force += force;
}

void Point::addMass(float mass) {
  this->mass += mass;
}

bool Point::updateInvMass() {
  if (abs(mass) <= std::numeric_limits<float>::epsilon()) {
    return false;
  }
  this->invMass = 1.0 / mass;
  return true;
}

void Point::update(float deltaTime) {
  // a_i = m^{-1} * f;
  Vector3f acc = force * invMass;

  // v_{i+1} = v_i + a_i * dt
  Vector3f deltaVel = acc * deltaTime;
  velocity += deltaVel;

  // r_{i+1} = r_i + v_{i+1} * dt
  Vector3f deltaPos = velocity * deltaTime;
  position += deltaPos;

  force = Vector3f(0, 0, 0);
}

bool Point::operator==(const Point &other) const {
  return this == &other;
}
