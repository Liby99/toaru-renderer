#ifndef TOARU_PHYSICS_SYSTEM_H
#define TOARU_PHYSICS_SYSTEM_H

#include "core/component.h"
#include "physics/tetrahedron.h"
#include <vector>

namespace toaru {
  class Face;
  class Tetrahedron;
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

    void createUnitCube(Vector3f pos, Vector3f ext, float density, float e, float v, bool flipX, bool flipY, bool flipZ);

    std::shared_ptr<Point> getPoint(Vector3f position);

    std::tuple<bool, std::shared_ptr<Face>> getFace(
      std::initializer_list<std::shared_ptr<Point>> points);
  };
}

#endif
