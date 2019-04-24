#ifndef TOARU_PHYSICS_AABB_TREE_NODE_H
#define TOARU_PHYSICS_AABB_TREE_NODE_H

#include "aabb.h"
#include "tetrahedron.h"

namespace toaru {
  class AABBTreeNode {
  public:
    AABB aabb;
    int startIndex, tetraAmount;
    std::vector<Tetrahedron *> &allTetras;
    std::unique_ptr<AABBTreeNode> left, right;
    bool leafFlag;

    AABBTreeNode(std::vector<Tetrahedron *> &allTetras);
    AABBTreeNode(std::vector<Tetrahedron *> &allTetras, int start, int amount);

    void refit();

    bool isLeaf();
    const AABBTreeNode &getLeft();
    const AABBTreeNode &getRight();
    const AABB &getBoundingBox();
    bool isLeftRightIntersecting();
  };
}

#endif