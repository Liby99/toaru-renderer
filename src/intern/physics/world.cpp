#include "physics/world.h"

using namespace toaru;

void World::init() {
}

void World::update() {
  double deltaTime = context().getDeltaTime();
  std::for_each(tetrahedrons.begin(), tetrahedrons.end(),
                [&](const std::shared_ptr<Tetrahedron> &tetrahedron)
                {
                  tetrahedron->update(deltaTime);
                });
  std::for_each(points.begin(), points.end(),
                [&](const std::shared_ptr<Point> &point)
                {
                  point->update(deltaTime);
                });
}
