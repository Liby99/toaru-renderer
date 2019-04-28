#ifndef TOARU_PHYSICS_AABB_TREE_H
#define TOARU_PHYSICS_AABB_TREE_H

#include "aabb_tree_node.h"

namespace toaru {
  class AABBTree {
  public:
    std::unique_ptr<AABBTreeNode> root;
    AABBTree(std::vector<Tetrahedron *> &allTetras);
    void handleCollision(Tetrahedron &tetra);
    void refit();
  };
}

#endif