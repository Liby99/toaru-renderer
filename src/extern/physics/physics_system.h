#ifndef TOARU_PHYSICS_SYSTEM_H
#define TOARU_PHYSICS_SYSTEM_H

#include "core/component.h"
#include "physics/tetrahedron.h"
#include <vector>

namespace toaru {
  class PhysicsSystem : public Component {
  public:
    std::vector<std::shared_ptr<Tetrahedron>> tetrahedrons;
    std::vector<std::shared_ptr<Point>> points;
    std::vector<std::shared_ptr<Face>> faces;
    // TODO: lookup table ?
    // TODO: AABB

    bool isPlaying;
    float deltaTime;
    int step;

    PhysicsSystem();

    virtual void play();
    virtual void pause();
    virtual void stepOnce();

    virtual void update();
  };
}

#endif
