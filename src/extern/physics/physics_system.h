#ifndef TOARU_PHYSICS_SYSTEM_H
#define TOARU_PHYSICS_SYSTEM_H

#include "core/component.h"
#include "tetrahedron.h"
#include <vector>

namespace toaru {
  class Face;
  class Tetrahedron;
  class PhysicsSystem : public Component {
  public:
    std::vector<std::unique_ptr<Tetrahedron>> tetrahedrons;
    std::vector<std::unique_ptr<Point>> points;
    std::vector<std::unique_ptr<Face>> faces;

    bool isPlaying;
    float deltaTime;
    int step;

    PhysicsSystem();

    virtual void play();
    virtual void pause();
    virtual void stepOnce();

    virtual void init();
    virtual void update();

    void createUnitCube(Vector3f pos, Vector3f ext, const PhysicsMaterial &mat);

    Point& getPoint(Vector3f position);

    void makeFace(std::unique_ptr<Tetrahedron> &tet);
  };
}

#endif
