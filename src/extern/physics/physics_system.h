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
    std::vector<std::unique_ptr<Tetrahedron>> tetrahedrons;
    std::vector<std::unique_ptr<Point>> points;
    std::vector<std::unique_ptr<Face>> faces;

    std::vector<std::unique_ptr<PhysicsMaterial>> Ks;
    std::vector<std::unique_ptr<PhysicsMaterial>> Ds;

    // TODO: lookup table ?
    // TODO: AABB

    bool isPlaying;
    float deltaTime;
    int step;

    PhysicsSystem();

    virtual void play();
    virtual void pause();
    virtual void stepOnce();

    virtual void init();
    virtual void update();

    void createUnitCube(Vector3f pos, Vector3f ext, float density, const PhysicsMaterial &K,
                        const PhysicsMaterial &D);

    Point& getPoint(Vector3f position);

    void makeFace(std::unique_ptr<Tetrahedron> &tet);
  };
}

#endif
