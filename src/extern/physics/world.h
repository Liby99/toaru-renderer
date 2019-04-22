#ifndef TOARU_WORLD_H
#define TOARU_WORLD_H

#include "core/component.h"
#include "physics/tetrahedron.h"
#include <vector>

namespace toaru {
  class World : public Component {
  public:
    std::vector<std::shared_ptr<Tetrahedron>> tetrahedrons;
    // TODO: lookup table ?
    // TODO: AABB
    std::vector<std::shared_ptr<Point>> points;
    std::vector<std::shared_ptr<Face>> faces;

    virtual void init() {
    }

    virtual void update() {
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
  };
}

#endif
