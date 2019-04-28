#include "physics/physics_object.h"

using namespace toaru;

PhysicsObject::PhysicsObject(const PhysicsMaterial &mat) : mat(mat) {}

void PhysicsObject::addTetrahedron(Tetrahedron &tetra) {
  tetrahedrons.push_back(&tetra);
}

void PhysicsObject::buildAABBTree() {
  aabbTree.reset();
  aabbTree = make_unique<AABBTree>(tetrahedrons);
}

void PhysicsObject::update() {
  aabbTree->refit();
}