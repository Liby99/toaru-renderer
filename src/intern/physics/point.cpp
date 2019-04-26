#include "physics/point.h"

using namespace toaru;

Point::Point(Vector3f position, int index, bool isFixed)
  : mass(0), index(index), isFixed(isFixed), velocity(Vector3f::Zero()), force(Vector3f::Zero()) {
  this->position = position;
}

void Point::addForce(const Vector3f &force) {
  this->force += force;
}

void Point::addMass(float mass) {
  this->mass += mass;
  updateInvMass();
}

bool Point::updateInvMass() {
  if (abs(mass) <= std::numeric_limits<float>::epsilon()) {
    return false;
  }
  this->invMass = 1.0 / mass;
  return true;
}

void Point::update(float deltaTime) {
  // TODO: better style to add gravity
  addForce(Vector3f(0, -9.8f * mass, 0));

  // Do the integration
  integrate(deltaTime);

  // Clear the force
  force = Vector3f(0, 0, 0);

  // TODO: Add ground as a physics object
  if (position.y() < -2) {
    position.y() = -2;
    velocity.y() = abs(velocity.y()) * 0.50;
    //velocity.x() = velocity.x() * 0.99;
    //velocity.z() = velocity.z() * 0.99;

    // TODO: Add friction 
    force += Vector3f(-velocity.x(), 0, -velocity.z());
  }
}

void Point::integrate(float deltaTime) {
  if (!isFixed) {
    // a_i = m^{-1} * f;
    Vector3f acc = force * invMass;

    // v_{i+1} = v_i + a_i * dt
    Vector3f deltaVel = acc * deltaTime;
    velocity += deltaVel;

    // r_{i+1} = r_i + v_{i+1} * dt
    Vector3f deltaPos = velocity * deltaTime;
    position += deltaPos;
  }
}

bool Point::operator==(const Point &other) const {
  return this == &other;
}
