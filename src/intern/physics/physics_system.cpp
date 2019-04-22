#include "physics/physics_system.h"

using namespace toaru;

PhysicsSystem::PhysicsSystem() {}

void PhysicsSystem::update() {
  double deltaTime = context().getDeltaTime();
  for (auto element : tetrahedrons) {
    element->update(deltaTime);
  }
  for (auto element : points) {
    element->update(deltaTime);
  }
}
