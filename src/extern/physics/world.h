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

    World();

    virtual void update();
  };
}

#endif
