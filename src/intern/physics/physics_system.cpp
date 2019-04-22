#include "physics/physics_system.h"

using namespace toaru;

PhysicsSystem::PhysicsSystem() : isPlaying(false), deltaTime(0.01), step(2) {}

void PhysicsSystem::play() {
  isPlaying = true;
}

void PhysicsSystem::pause() {
  isPlaying = false;
}

void PhysicsSystem::stepOnce() {
  for (auto element : tetrahedrons) {
    element->update(deltaTime);
  }
  for (auto element : points) {
    element->update(deltaTime);
  }
}

void PhysicsSystem::update() {
  //double deltaTime = context().getDeltaTime();
  if (isPlaying) {
    for (int i = 0; i < step; i++) {
      stepOnce();
    }
  }
}
