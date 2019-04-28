#ifndef TOARU_PHYSICS_OBJECT_H
#define TOARU_PHYSICS_OBJECT_H

#include "aabb_tree.h"
#include "tetrahedron.h"

namespace toaru {
  class PhysicsObject {
  public:
    const PhysicsMaterial &mat;
    std::unique_ptr<AABBTree> aabbTree;
    std::vector<Tetrahedron *> tetrahedrons;
    PhysicsObject(const PhysicsMaterial &mat);
    void addTetrahedron(Tetrahedron &tetra);
    void buildAABBTree();
    void update();
  };
}

#endif