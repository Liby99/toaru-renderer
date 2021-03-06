#ifndef TOARU_PHYSICS_SYSTEM_H
#define TOARU_PHYSICS_SYSTEM_H

#include "aabb_tree.h"
#include "core/component.h"
#include "physics_object.h"
#include <vector>

namespace toaru {
  class PhysicsSystem : public Component {
  public:
    std::vector<std::unique_ptr<PhysicsObject>> objects;
    std::vector<std::unique_ptr<Tetrahedron>> tetrahedrons;
    std::vector<std::unique_ptr<Point>> points;
    std::vector<std::unique_ptr<Face>> faces;

    bool isPlaying;
    float deltaTime;
    int step;

    PhysicsSystem();

    void play();
    void pause();

  protected:
    virtual void init();
    virtual void update();

    // Initialize stage functions
    void initRestStates();
    void buildAABBTrees();

    // Update stage functions
    void updateObjects();
    void processCollisions();
    void stepOnce();

    int addObject(const PhysicsMaterial &mat);
    int addPoint(const Vector3f &pos, bool isFixed = false);
    int addTetrahedron(int obj, int i1, int i2, int i3, int i4);

    int createBox(const PhysicsMaterial &mat, Vector3f center, Vector3f size,
                  Vector3u sub = Vector3u(1, 1, 1));

  private:
    std::map<std::string, Face *> degenerateFaces;
    const Face &getFace(int i1, int i2, int i3, int opposite);
  };
}

#endif
