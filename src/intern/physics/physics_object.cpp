#include "physics/physics_object.h"

using namespace toaru;

PhysicsObject::PhysicsObject(const PhysicsMaterial &mat) : mat(mat) {}

void PhysicsObject::addTetrahedron(const Tetrahedron &tetra) {
  tetrahedrons.push_back(&tetra);
}

void PhysicsObject::buildAABBTree() {
  aabbTree = make_unique<AABBTree>(tetrahedrons);
}