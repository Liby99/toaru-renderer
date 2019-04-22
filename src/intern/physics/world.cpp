#include "physics/world.h"

using namespace toaru;

World::World() {
  
}

void World::update() {
  double deltaTime = context().getDeltaTime();
  for (auto element : tetrahedrons) {
    element->update(deltaTime);
  }
  for (auto element : points) {
    element->update(deltaTime);
  }
}
